/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Asignatura: Sistemas Operativos (SSOO)
 * Curso: 2º
 * Práctica 2. Programación de sockets
 * @author Álvaro Pérez Ramos
 * @file 02-netcp.cpp
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
#include <cstring> 
// Gestion de errores          
#include <cerrno>
#include <expected>
#include <optional>
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
// Otras
#include <signal.h>
#include <atomic>

using open_file_result = std::expected<int, std::error_code>;
using make_socket_result = std::expected<int, std::error_code>;
using std::experimental::scope_exit;

struct program_options {
  bool show_help = false;
  bool listen = false;
  std::string input_filename;
  std::string output_filename;
};

std::atomic<bool> quit = false;

/**
 * @brief Parsea un string a un entero.
 * @param str String a parsear.
 * @param h Hash.
 * @return Entero.
*/
constexpr unsigned int str2int(const char *str, int h = 0) {
  return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

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

    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
      switch (str2int(it->data())) {
      case str2int("-h") || str2int("--help"):
        options.show_help = true;
        break;
      case str2int("-l"):
        options.listen = true;
        if (++it != end) {
          options.output_filename = *it;
        } else {
          return std::nullopt;  // -l option requires a filename
        }
        break;
      default:
        if (options.input_filename.empty()) {
          options.input_filename = *it;
        } else {
          return std::nullopt;  // Unexpected argument
        }
        break;
      }
    }
  return options;
}

/**
 * @brief Muestra la ayuda del programa.
*/
void Usage() {
  const std::string kHelperText = " Modo de empleo : \n"
                                  " netcp [-h] ORIGEN\n"
                                  " netcp Archivo\n"
                                  " netcp [-l] Archivo\n";
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
  if (bytes_read < 0) {
    return std::error_code(errno, std::system_category());
  }
  buffer.resize(bytes_read);
  return std::error_code(0, std::system_category());
}

/**
 * @brief Escribe un fichero.
 * @param fd Descriptor de fichero.
 * @param buffer Buffer con los datos a escribir.
 * @return Codigo de error.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el código de error.
*/
std::error_code write_file(int fd, const std::vector<uint8_t>& buffer) {
  ssize_t bytes_written = write(fd, buffer.data(), buffer.size());
  if (bytes_written < 0 || static_cast<size_t>(bytes_written) > buffer.size()) {
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Copia un fichero.
 * @param src_path Ruta del fichero de origen.
 * @param dst_path Ruta del fichero de destino.
 * @return Codigo de error.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el código de error.
*/
std::error_code copy_file(const std::string& src_path, const std::string& dst_path) {
  try {
    open_file_result src_fd_result = open_file(src_path, O_RDONLY, 0);
    int src_fd = *src_fd_result;
    auto src_guard = scope_exit([src_fd]{ close(src_fd); });

    open_file_result dst_fd_result = open_file(dst_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    int dst_fd = *dst_fd_result;
    auto dst_guard = scope_exit([dst_fd]{ close(dst_fd); });

    struct stat file_stat;
    if (fstat(src_fd, &file_stat) == -1 || fstat(dst_fd, &file_stat) == -1) {
      std::cerr << "Error: El archivo de origen no existe\n";
      return std::error_code(errno, std::system_category());
    }

    std::vector<uint8_t> buffer(1024);
    while (true) {
      std::error_code read_error = read_file(src_fd, buffer);
      if(buffer.empty() || read_error){
        break;
      }
      std::error_code write_result = write_file(dst_fd, buffer);
    }
  } catch (const std::exception &e){
    return std::error_code(errno, std::system_category());
  }
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
  in_addr addr;
  if (inet_aton(ip_address->c_str(), &addr) == 0) {
    return std::nullopt;
  }
return address;
}

/**
 * @brief Crea un socket.
 * @param address Direccion IP.
 * @return Descriptor de fichero del socket.
 * @note Si no se especifica una direccion IP, se crea un socket de tipo AF_INET.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
make_socket_result make_socket(bool opt) {
  int fd;
  fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    std::error_code error(errno, std::system_category());
    return std::unexpected(error);
  }
  if (opt) {
    int port;
    const char *netcp_port_env = std::getenv("NETCP_PORT");
    if (netcp_port_env) {
      port = std::atoi(netcp_port_env);
    } else { port = 8080; }
    
    auto addrs_listen = make_ip_address("0.0.0.0", port);
    auto address = *addrs_listen; 
    if (bind(fd, reinterpret_cast<const sockaddr *>(&address), sizeof(address)) == -1) {
      close(fd);
      std::error_code error(errno, std::system_category());
      return std::unexpected(error);
    }
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
std::error_code send_to(int fd, const std::vector<uint8_t> &message, const sockaddr_in& address) {
  ssize_t bytes_sent = sendto(fd, message.data(), message.size(), 0, 
                              reinterpret_cast<const sockaddr *>(&address), 
                              sizeof(address));
  if (bytes_sent < 0) {
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Recibe un mensaje de una direccion IP.
 * @param fd Descriptor de fichero del socket.
 * @param message Mensaje a enviar.
 * @param address Direccion IP.
 * @return Codigo de error.
 * @note Si se produce un error, se devuelve un objeto con el campo error_code con el codigo de error.
*/
std::error_code receive_from(int sock_fd, std::vector<uint8_t>& message, sockaddr_in& address) {
  socklen_t addr_len = sizeof(address);
  ssize_t bytes_received = recvfrom(sock_fd, message.data(), message.size(), 0,
                                    reinterpret_cast<sockaddr*>(&address), &addr_len);

  if (bytes_received <= 0 || static_cast<size_t>(bytes_received) > message.size()) {
    message.resize(bytes_received);
    return std::error_code(errno, std::system_category());
  }

  // Resize the vector to match the actual size of the received data
  message.resize(bytes_received);
  return std::error_code(0, std::system_category());
}

/**
 * @brief Obtiene el valor de una variable de entorno.
 * @param name Nombre de la variable de entorno.
 * @return Valor de la variable de entorno.
*/
std::string getenv(const std::string& name) {
    char* value = std::getenv(name.c_str());
    if (value) {
        return std::string(value);
    } else {
        return std::string();
    }
}

/**
 * @brief Envia un mensaje a una direccion IP.
 * @param filename Nombre del archivo.
 * @return Codigo de error.
*/
std::error_code netcp_send_file(const std::string& filename) {
  std::cout << "Abriendo fichero: " << filename << "\n";
  open_file_result result = open_file(filename, O_RDONLY, 0);
  if (!result) {
    return std::error_code(errno, std::system_category());
  }
  int fd = *result;
  auto src_guard = scope_exit([fd]{ close(fd); });

  struct stat file_stat;
  if (fstat(fd, &file_stat) == -1) {
    return std::error_code(errno, std::system_category());
  }
// Obtener la dirección IP
  std::string ip = std::getenv("NETCP_IP") ? getenv("NETCP_IP") : "127.0.0.1";

  // Obtener el puerto de destino
  int port = std::getenv("NETCP_PORT") ? std::atoi(getenv("NETCP_PORT")) : 8080;


  std::cout << "Creando dirección\n";
  auto addr_result = make_ip_address(ip, port);
  if (!addr_result) {
    return std::error_code(errno, std::system_category());
  }
  auto address = *addr_result;

  std::cout << "Creando Socket\n";
  make_socket_result sock_result = make_socket(false);
  if (!sock_result) {
    return std::error_code(errno, std::system_category());
  }
  int sock_fd = *sock_result;
  auto src_guard_2 = scope_exit([sock_fd]{close(sock_fd);});

  std::cout << "Creando buffer de 1KB \n";
  std::vector<uint8_t> buffer(1024);

  while(true && !quit) {
    std::cout << "Leyendo archivo... \n";
    std::error_code read_error = read_file(fd, buffer);
    if (read_error) {
      return std::error_code(errno, std::system_category());
    }
    if(buffer.empty()){
      std::cout << "Se han leido todos los datos\n";
      break;
    }
    std::cout << "Enviando... \n";
    std::error_code send_result = send_to(sock_fd, buffer, address);
    if (send_result) {
      return std::error_code(errno, std::system_category());
    }
    usleep(1000);
  }
  std::error_code send_result = send_to(sock_fd, buffer, address);
  if (send_result) {
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Recibe un mensaje de una direccion IP.
 * @param filename Nombre del archivo.
 * @return Codigo de error.
*/
std::error_code netcp_receive_file(const std::string& filename) {
  std::cout << "Abriendo archivo: " << filename << "\n";
  open_file_result result_fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); // S_I asegura tener permisos de lectura y estructura
  if (!result_fd) {
    return std::error_code(errno, std::system_category());
  }
  int fd = *result_fd;
  auto src_guard = scope_exit([fd]{ close(fd); });
  
  std::cout << "Creando Socket\n";
  make_socket_result sock_result = make_socket(true);
  if (!sock_result) {
    return std::error_code(errno, std::system_category());
  }
  int sock_fd = *sock_result;
  auto src_guard_2 = scope_exit([sock_fd]{close(sock_fd);});

  std::cout << "Creando dirección\n";
  int port = std::getenv("NETCP_PORT") ? std::atoi(getenv("NETCP_PORT")) : 8080;
  auto addr_result = make_ip_address("0.0.0.0", port);
  if (!addr_result) {
    return std::error_code(errno, std::system_category());
  }
  auto address = *addr_result;

  std::vector<uint8_t> message(1024);

  while (true && !quit) {
    std:: cout << "Reciviendo datos... \n";
    std::error_code bytes_received = receive_from(sock_fd, message, address);
    if (bytes_received) {
      return std::error_code(errno, std::system_category());
    }
    if (message.empty()) {
      std::cout << "Se an pasado todos los datos\n";
      break;
    }
    std:: cout << "Escribiendo datos... \n";
    std::error_code write_msg = write_file(fd, message);
    if (write_msg) {
      return std::error_code(errno, std::system_category());
    }

  }
  return std::error_code(0, std::system_category());
}

void signal_err(int signal) {
    const char* msg = "netcp: terminado... (signal %d: %s)\n";
    const char* signal_name = strsignal(signal);

    dprintf(STDERR_FILENO, msg, signal, signal_name);
    quit = true;

    // Salir con el código de la señal
    std::exit(signal);
}

int main(int argc, char *argv[]) {
  // Configurar manejadores de señales
  struct sigaction term_action = {0};
  term_action.sa_handler = &signal_err;
  sigaction( SIGTERM, &term_action, nullptr ); // SIGTERM: kill -TERM pid o kill -15 pid
  sigaction( SIGINT, &term_action, nullptr );  // SIGINT: kill -INT pid o kill -2 pid
  sigaction( SIGHUP, &term_action, nullptr );  // SIGHUP: kill -HUP pid o kill -1 pid
  sigaction( SIGQUIT, &term_action, nullptr ); // SIGQUIT: kill -QUIT pid o kill -3 pid

  std::cout << "Empezando programa\n";
  auto options = parse_args(argc, argv);
  if (!options || options->show_help || (!options->listen && options->input_filename.empty())) {
    Usage();
    return options ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  if (options->listen) {
    std::cout << "Modo escucha\n";
    std::error_code nrf_result = netcp_receive_file(options->output_filename);
    if (nrf_result) {
      std::cerr << std::format("Error ({}): {}\n", nrf_result.value(), nrf_result.message());
      return EXIT_FAILURE;
    }
  } else {
    std::cout << "Modo envio\n";
    std::error_code nsf_result = netcp_send_file(options->input_filename);
    if (nsf_result) {
      std::cerr << std::format("Error ({}): {}\n", nsf_result.value(), nsf_result.message());
      return EXIT_FAILURE;
    }
  }
  
  std::cout << "Fin del programa\n";
  return EXIT_SUCCESS;
}
