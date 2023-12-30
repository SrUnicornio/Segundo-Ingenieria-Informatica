/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Asignatura: Sistemas Operativos (SSOO)
 * Curso: 2º
 * Práctica 2. Programación de sockets
 * @author Álvaro Pérez Ramos
 * @file netcp.cpp
 * @brief Programa que copia el contenido de un archivo a un puerto UDP.
 *        El programa se ejecutará de la siguiente forma: netcp [-h] ORIGEN
 *        Donde ORIGEN es la ruta del archivo a copiar y -h muestra la ayuda.
 *        El programa se conectará al puerto 8080 del localhost y enviará el contenido del archivo ORIGEN.
 * @bug No hay bugs conocidos
 * Antes de compilar atualiza el comilador a g++-13:
 *    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
 *    sudo apt-get update
 *    sudo apt-get install g++-13
 *    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 130 --slave /usr/bin/g++ g++ /usr/bin/g++-13 --slave /usr/bin/gcov gcov /usr/bin/gcov-13    
 * * Compilar con: g++ -std=c++23 netcp.cpp -o netcp
*/

#include <iostream>
#include <vector>
#include <string> 
// Gestion de errores          
#include <cerrno>
#include <expected>
#include <format>
// Creacion/gestion de sockets
#include <sys/types.h>
#include <sys/socket.h>       
#include <netinet/in.h>
// Gestion de archivos
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <experimental/scope>
// Creacion y gestion de direciones
#include <arpa/inet.h>

using open_file_result = std::expected<int, std::error_code>;
using make_socket_result = std::expected<int, std::error_code>;
using std::experimental::scope_exit;

struct program_options {
  bool show_help = false;
  std::string input_filename;
};

/**
 * @brief Parsea los argumentos de la linea de comandos.
 * @param argc Numero de argumentos.
 * @param argv Argumentos.
 * @return Opciones de la linea de comandos.
 * @note Si se pasa un argumento -h o --help, se muestra la ayuda y se devuelve un objeto con el campo show_help a true.
*/
std::optional<program_options> parse_args(int argc, char *argv[]) {
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;

  if (args.empty() || args[0] == "-h" || args[0] == "--help") {
    options.show_help = true;
    return options;
  }

  options.input_filename = std::string(args[0]);
  return options;
}

/**
 * @brief Muestra la ayuda del programa.
*/
void Usage() {
  const std::string kHelperText = "Modo de empleo : netcp [-h] ORIGEN\n"
                                  "Copiar el contenido de un archivo a un puerto UDP.\n"
                                  "Opciones:\n"
                                  "-h, --help Muestra esta ayuda.\n";
  std::cout << kHelperText;
}

/**
 * @brief Abre un fichero.
 * @param path Ruta del fichero.
 * @param flags Flags de apertura.
 * @param mode Modo de apertura.
 * @return Descriptor de fichero abierto.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
open_file_result open_file(const std::string &path, int flags, mode_t mode) {
  int fd = open(path.c_str(), flags, mode);
  if (fd == -1) {
    std::error_code error(errno, std::system_category());
    return std::unexpected(error);
  }
  return fd;
}

/**
 * @brief Lee un fichero.
 * @param fd Descriptor de fichero.
 * @param buffer Buffer donde se almacena el contenido del fichero.
 * @return Codigo de error.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
std::error_code read_file(int fd, std::vector<uint8_t> &buffer) {
  ssize_t bytes_read = read(fd, buffer.data(), buffer.size());
  if (bytes_read < 0 || static_cast<size_t>(bytes_read) > buffer.size()) {
    return std::error_code(errno, std::system_category());
  }
  buffer.resize(bytes_read);
  return std::error_code(0, std::system_category());
}

/**
 * @brief Crear una direccion IP.
 * @param ip_address Direccion IP.
 * @param port Puerto.
 * @return Direccion IP.
 * @note Si no se especifica una direccion IP, se devuelve un objeto vacio.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
std::optional<sockaddr_in> make_ip_address(const std::optional<std::string> &ip_address, uint16_t port = 0) {
  if (!ip_address) {
    return std::nullopt;
  }
  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  inet_pton(AF_INET, ip_address->c_str(), &address.sin_addr);
  return address;
}

/**
 * @brief Crea un socket.
 * @param address Direccion IP.
 * @return Descriptor de fichero del socket.
 * @note Si no se especifica una direccion IP, se crea un socket de tipo AF_INET.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
make_socket_result make_socket(std::optional<sockaddr_in> address = std::nullopt) {
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    std::error_code error(errno, std::system_category());
    return std::unexpected(error);
  }
  return fd;
}

/**
 * @brief Envia un mensaje a una direccion IP.
 * @param fd Descriptor de fichero del socket.
 * @param message Mensaje a enviar.
 * @param address Direccion IP.
 * @return Codigo de error.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
std::error_code send_to(int fd, const std::vector<uint8_t> &message, const sockaddr_in &address) {
  int bytes_sent = sendto(fd, message.data(), message.size(), 0, reinterpret_cast<const sockaddr *>(&address), 
                          sizeof(address));
  if (bytes_sent < 0) {
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}

int main(int argc, char *argv[]) {
  std::cout << "Empezando programa\n";
  auto options = parse_args(argc, argv);
  if (!options || options->show_help || options->input_filename.empty()) {
    Usage();
    return options ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  std::cout << "Abriendo fichero: " << options->input_filename << "\n";
  open_file_result result = open_file(options->input_filename, O_RDONLY, 0);
  if (!result) {
    std::cerr << std::format("Error ({}): {}\n", result.error().value(), result.error().message());
    return EXIT_FAILURE;
  }
  int fd = *result;
  
  auto src_guard = scope_exit([fd]{ close(fd); });

  struct stat file_stat;
  if (fstat(fd, &file_stat) == -1) {
    std::cerr << "Error: El archivo no existe \n";
    return EXIT_FAILURE;
  }
  std::cout << "Creando Socket\n";
  make_socket_result sock_result = make_socket();
  if (!sock_result) {
    std::cerr << std::format("Error al crear el socket ({}): {}\n", sock_result.error().value(), sock_result.error().message());
    return EXIT_FAILURE;
  }
  int sock_fd = *sock_result;
    auto src_guard = scope_exit([sock_fd]{close(sock_fd);});

  std::cout << "Creando dirección\n";
  auto addr_result = make_ip_address("127.0.0.1", 8080);
  if (!addr_result) {
    std::cerr << "Error al crear la dirección\n";
    return EXIT_FAILURE;
  }
  auto address = *addr_result;

  std::cout << "Creando buffer de 1KB \n";
  std::vector<uint8_t> buffer(1024);

  while(!buffer.empty()) {
    std::cout << "Leyendo archivo \n";
    std::error_code read_error = read_file(fd, buffer);
    if (read_error) {
      std::cerr << "Error al leer el archivo: " << read_error.message() << "\n";
      return EXIT_FAILURE;
    }
    if(buffer.empty()){
      break;
    }
    std::cout << "Enviando... \n";
    std::error_code send_result = send_to(sock_fd, buffer, address);
    if (send_result) {
      std::cerr << "Error al enviar el archivo al servidor: " << send_result.message() << "\n";
      return EXIT_FAILURE;
    }
    std::vector<uint8_t> buffer(1024);
  }
  std::cout << "Enviando... \n";
  std::error_code send_result = send_to(sock_fd, buffer, address);
  if (send_result) {
    std::cerr << "Error al enviar el archivo al servidor: " << send_result.message() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}