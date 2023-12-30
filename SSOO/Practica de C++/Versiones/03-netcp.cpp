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
// Comandos
#include <sys/wait.h>
// Otras
#include <signal.h>
#include <atomic>

using open_file_result = std::expected<int, std::error_code>;
using make_socket_result = std::expected<int, std::error_code>;
using std::experimental::scope_exit;

class subprocess {
 public:
  enum class stdio {
    in, out, err, out_err
  };
  subprocess(const std::vector<std::string>& args, subprocess::stdio redircted_io) : arg_(args), redircted_io_(redircted_io) {};
  ~subprocess() {};

  bool is_alive() {
    if (pid_ == -1) {
      return false;
    }
    int status;
    pid_t result = waitpid(pid_, &status, WNOHANG);
    if (result == -1) {
      return false;
    }
    return result;
  }

  std::error_code exec() {
    int fds[2];
    int retur_code = pipe(fds);

    pid_ = fork();
    if (pid_ == 0) {
      switch (redircted_io_) {
        case subprocess::stdio::in:
          dup2(fds[0], STDIN_FILENO); // redirigir la entrada estandar al pipe
          break;
        case subprocess::stdio::out:
          dup2(fds[1], STDOUT_FILENO); // redirigir la salida estandar al pipe
          break;
        case subprocess::stdio::err:
          dup2(fds[1], STDERR_FILENO); // redirigir la salida estandar al pipe
          break;
        case subprocess::stdio::out_err:
          dup2(fds[1], STDOUT_FILENO); // redirigir la salida estandar al pipe
          dup2(fds[1], STDERR_FILENO); // redirigir la salida estandar al pipe
      }
      close(fds[0]);  // cerrar el descriptor de lectura del pipe
      close(fds[1]);  // cerrar el descriptor de escritura del pipe
      // convertir args en argv
      std::vector<char*> argv;
      for (const auto& arg : arg_) {
        if (arg.empty()) {
          continue;
        }
        // Pasar string a char*
        char* token = std::strtok(const_cast<char*>(arg.c_str()), " ");
        while (token != nullptr) {
          argv.push_back(token);
          token = std::strtok(nullptr, " ");
        }
      }
      argv.push_back(nullptr);

      execvp(argv[0], argv.data());

    } else if (pid_ > 0) {
      switch (redircted_io_) {
        case subprocess::stdio::in:
          close(fds[0]);    // cerrar el descriptor de lectura del pipe
          in_fd_ = fds[1];  // guardar el descriptor de escritura del pipe
          break;
        case subprocess::stdio::out:
        case subprocess::stdio::err:
        case subprocess::stdio::out_err:
          close(fds[1]);     // cerrar el descriptor de escritura del pipe
          out_fd_ = fds[0];  // guardar el descriptor de lectura del pipe
          break;
      }
    } else {
      return std::error_code(errno, std::system_category());
    }
    return std::error_code(0, std::system_category());
  }

  std::error_code wait() {
    if (pid_ == -1) {
      return std::error_code(errno, std::system_category());
    }
    int status;
    pid_t result = waitpid(pid_, &status, WNOHANG);
    if (result == -1) {
      return std::error_code(errno, std::system_category());
    } 
    return std::error_code(0, std::system_category());
  }

  std::error_code killprocess() {
    if (pid_ == -1) {
      return std::error_code(errno, std::system_category());
    }

    int result = ::kill(pid_, SIGTERM);
    if (result == -1) {
      return std::error_code(errno, std::system_category());
    }

    pid_ = -1;
    return std::error_code(0, std::system_category());
  }

  pid_t pid() { return pid_; }          // Devuelve el pid del proceso hijo
  int stdin_fd() { return in_fd_; }     // Devuelve el descriptor de lectura del pipe
  int stdout_fd() { return out_fd_; }   // Devuelve el descriptor de escritura del pipe

 private:
  const std::vector<std::string> arg_;
  subprocess::stdio redircted_io_;
  pid_t pid_{-1};
  int in_fd_{-1}, out_fd_{-1};
};

/**
 * @brief Opciones de la linea de comandos.
*/
struct program_options {
  // 01-netcp.cpp
  bool show_help = false;
  std::string input_filename;
  // 02-netcp.cpp
  bool listen = false;
  std::string output_filename;
  // 03-netcp.cpp
  bool command = false;
  bool capture_stout = false;
  bool capture_error = false;
  std::string command_option;
  std::string command_arg;
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
      case ((str2int("-h")) || (str2int("--help") || str2int(it->data()) == 0)):
        options.show_help = true;
        break;
      case (str2int("-l")):
        options.listen = true;
        ++it;
        if (it == end) {
          return std::nullopt;
        }
        for (auto it2 = it; it2 != end; ++it2) {
          switch (str2int(it2->data())) {
            case (str2int("-c")):
              options.command = true;
              ++it2;
              if (it2 != end) {
                options.command_option = *it2;
                ++it2;
                if (it2 == end) {
                  return options;
                } else {
                  for (auto it3 = it2; it3 != end; ++it3) {
                    options.command_arg += *it3;
                    if (it3 + 1 != end) {
                      options.command_arg += " ";
                    }
                    it2 = it3;
                  }
                } 
              }
              it = it2;
              break;
            default:
              if (options.output_filename.empty()) {
                options.output_filename = *it2;
              } else {
                return std::nullopt;
              }
              break;
          }
        }
        break;
      case (str2int("-1")):
        options.capture_stout = true;
        break;
      case (str2int("-2")):
        options.capture_error = true;
        break;
      case (str2int("-c")):
        options.command = true;
        ++it;
        if (it != end) {
          options.command_option = *it;
          ++it;
          if (it == end) {
            return options;
          } else {
            for (auto it2 = it; it2 != end; ++it2) {
              options.command_arg += *it2;
              if (it2 + 1 != end) {
                options.command_arg += " ";
              }
              it = it2;
            }
          } 
        }
        break;
      default:
        if (options.input_filename.empty()) {
          options.input_filename = *it;
        } else {
          return std::nullopt;
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
  const std::string kHelperText = " Modo de empleo: \n"
                                  " netcp -h \n"
                                  " netcp Archivo\n"
                                  " netcp [-1] [-2] -c COMANDO [ARG...] \n"
                                  " netcp -l Archivo\n"
                                  " netcp -l -c COMANDO [ARG...]\n";
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
make_socket_result make_socket(std::optional<sockaddr_in> address = std::nullopt) {
  int fd;
  fd = socket(AF_INET, SOCK_DGRAM, 0);
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
  open_file_result result = open_file(filename, O_RDONLY, 0);
  if (!result) {
    return result.error();
  }
  int fd = *result;
  auto src_guard = scope_exit([fd]{ close(fd); });

  struct stat file_stat;
  if (fstat(fd, &file_stat) == -1) {
    return std::error_code(errno, std::system_category());
  }

  make_socket_result sock_result = make_socket();
  if (!sock_result) {
    return std::error_code(errno, std::system_category());
  }
  int sock_fd = *sock_result;
  auto src_guard_2 = scope_exit([sock_fd]{close(sock_fd);});
  
  // Obtener la dirección IP
  std::string ip = std::getenv("NETCP_IP") ? getenv("NETCP_IP") : "127.0.0.1";
  // Obtener el puerto de destino
  uint16_t port = std::getenv("NETCP_PORT") ? std::atoi(getenv("NETCP_PORT")) : 8080;

  auto addr_result = make_ip_address(ip, port);
  if (!addr_result) {
    return std::error_code(errno, std::system_category());
  }
  auto address = *addr_result;

  std::vector<uint8_t> buffer(1024);

  while(true && !quit) {
    std::error_code read_error = read_file(fd, buffer);
    if (read_error) {
      return read_error;
    }
    if(buffer.empty()){
      break;
    }
    if (EINTR == errno) {
      continue;
    }
    std::error_code send_result = send_to(sock_fd, buffer, address);
    if (send_result) {
      return send_result;
    }
    usleep(1000);
  }
  std::error_code send_result = send_to(sock_fd, buffer, address);
  if (send_result) {
    return send_result;
  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Recibe un mensaje de una direccion IP.
 * @param filename Nombre del archivo.
 * @return Codigo de error.
*/
std::error_code netcp_receive_file(const std::string& filename) {
  open_file_result result_fd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); // S_I asegura tener permisos de lectura y estructura
  if (!result_fd) {
    return result_fd.error();
  }
  int fd = *result_fd;
  auto src_guard = scope_exit([fd]{ close(fd); });
  
  make_socket_result sock_result = make_socket();
  if (!sock_result) {
    return std::error_code(errno, std::system_category());
  }
  int sock_fd = *sock_result;
  auto src_guard_2 = scope_exit([sock_fd]{close(sock_fd);});

  uint16_t port = std::getenv("NETCP_PORT") ? std::atoi(getenv("NETCP_PORT")) : 8080;
  auto addr_result = make_ip_address("0.0.0.0", port);
  if (!addr_result) {
    return std::error_code(errno, std::system_category());
  }
  auto address = *addr_result;

  if (bind(sock_fd, reinterpret_cast<const sockaddr *>(&address), sizeof(address)) == -1) {
    return std::error_code(errno, std::system_category());
  }

  std::vector<uint8_t> message(1024);
  while (true && !quit) {
    std::error_code error_received = receive_from(sock_fd, message, address);
    if (error_received) {
      return error_received;
    }
    if (message.empty()) {
      break;
    }
    if (EINTR == errno) {
      continue;
    }
    std::error_code write_msg = write_file(fd, message);
    if (write_msg) {
      return write_msg;
    }

  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Envia un comando a una direccion IP.
 * @param args Argumentos del comando.
 * @param mode Modo de redireccionamiento.
*/
std::error_code netcp_receive_command(std::vector<std::string> args, subprocess::stdio mode) {
  subprocess command(args, mode);
  auto src_guard = scope_exit([&command]{command.wait(); command.killprocess();});
  std::error_code error = command.exec(); 
  if (error) {
    return error;
  }

  pid_t pid = command.pid();

  make_socket_result sock_result = make_socket();
  if (!sock_result) {
    return std::error_code(errno, std::system_category());
  }
  int sock_fd = *sock_result;
  auto src_guard_2 = scope_exit([sock_fd]{close(sock_fd);});
  
  // Obtener la dirección IP
  std::string ip = std::getenv("NETCP_IP") ? getenv("NETCP_IP") : "127.0.0.1";
  // Obtener el puerto de destino
  uint16_t port = std::getenv("NETCP_PORT") ? std::atoi(getenv("NETCP_PORT")) : 8080;

  auto addr_result = make_ip_address(ip, port);
  if (!addr_result) {
    return std::error_code(errno, std::system_category());
  }
  auto address = *addr_result;

  std::vector<uint8_t> buffer(1024);  
  while (true && !quit) {
    std::error_code error_read = read_file(command.stdout_fd(), buffer);
    if (error_read) {
      return error_read;
    }
    if(buffer.empty() && !command.is_alive()){
      break;
    }
    if (EINTR == errno) {
      continue;
    }
    std::error_code error_send = send_to(sock_fd, buffer, address);
    if (error_send) {
      return error_send;
    }
    usleep(1000);
  }
  std::error_code error_send = send_to(sock_fd, buffer, address);
  if (error_send) {
    return error_send;
  }

  return std::error_code(0, std::system_category());
}

/**
 * @brief Recibe un comando de una direccion IP.
 * @param args Argumentos del comando.
 * @param mode Modo de redireccionamiento.
*/
std::error_code netcp_send_command(std::vector<std::string> args, subprocess::stdio mode) {
  subprocess command(args, mode);
  auto src_guard = scope_exit([&command]{command.wait(); command.killprocess();});
  std::error_code error = command.exec();
  if (error) {
    return error;
  }

  pid_t pid = command.pid();

  make_socket_result sock_result = make_socket();
  if (!sock_result) {
    return std::error_code(errno, std::system_category());
  }
  int sock_fd = *sock_result;
  auto src_guard_2 = scope_exit([sock_fd]{close(sock_fd);});
  
  // Obtener la dirección IP
  std::string ip = std::getenv("NETCP_IP") ? getenv("NETCP_IP") : "0.0.0.0";
  // Obtener el puerto de destino
  uint16_t port = std::getenv("NETCP_PORT") ? std::atoi(getenv("NETCP_PORT")) : 8080;

  auto addr_result = make_ip_address(ip, port);
  if (!addr_result) {
    return std::error_code(errno, std::system_category());
  }
  auto address = *addr_result;

  if (bind(sock_fd, reinterpret_cast<const sockaddr *>(&address), sizeof(address)) == -1) {
    return std::error_code(errno, std::system_category());
  }

  std::vector<uint8_t> message(1024);
  while (true && !quit) {
    std::error_code bytes_received = receive_from(sock_fd, message, address);
    if (bytes_received) {
      return bytes_received;
    }
    if (message.empty() && !command.is_alive()) {
      break;
    }
    if (EINTR == errno) {
      continue;
    }
    std::error_code write_msg = write_file(command.stdin_fd(), message);
    if (write_msg) {
      return write_msg;
    }
    usleep(1000);
  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Manejador de señales.
 * @param signal Señal.
 */
void signal_err(int signal) {
   // mensge netcp: Terminación por señal <signal> con write
    const char* type_signal = strsignal(signal);
    const std::string kMessage = std::format("netcp: Terminación por señal ({}): {}\n", signal, type_signal);
    
    write(STDERR_FILENO, kMessage.data(), kMessage.size());
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

  std::cout << "Empezando programa\n";
  auto options = parse_args(argc, argv);
  if (!options || options->show_help ) {
    Usage();
    return options ? EXIT_SUCCESS : EXIT_FAILURE;
  }
  if (options->command) {
    std::cout << "Comando\n";
    std::string comando = options->command_option.empty() ? "" : options->command_option ;
    std::string arg = options->command_arg.empty() ? "" : options->command_arg ;
    std::cout << "Comando: " << comando << " arg: " << arg << "\n";
    std::vector<std::string> args = {comando, arg};
    if (options->listen) {
      std::cout << "Escuchando\n";
      std::error_code nsc = netcp_send_command(args, subprocess::stdio::in);
      if (nsc) {
        std::cerr << std::format("Error ({}): {}", nsc.value(), nsc.message()) << "\n";
        return EXIT_FAILURE;
      }
    } else {
      std::cout << "Enviando\n";
      std::error_code nrc;
      if (options->capture_stout && options->capture_error) {
        nrc = netcp_receive_command(args, subprocess::stdio::out_err);
      } else if (options->capture_stout) {
        nrc = netcp_receive_command(args, subprocess::stdio::out);
      } else if (options->capture_error) {
        nrc = netcp_receive_command(args, subprocess::stdio::err);
      } else {
        nrc = netcp_receive_command(args, subprocess::stdio::out);
      }
      if (nrc) {
        std::cerr << std::format("Error ({}): {}", nrc.value(), nrc.message()) << "\n";
        return EXIT_FAILURE;
      }
    }

    std::cout << "Fin\n";
    return EXIT_SUCCESS;
  } else {
    if (options->listen) {
      std::error_code nrf = netcp_receive_file(options->output_filename);
      if (nrf) {
        std::cerr << std::format("Error ({}): {}", nrf.value(), nrf.message()) << "\n";
        return EXIT_FAILURE;
      }
    } else {
      std::error_code nsf = netcp_send_file(options->input_filename);
      if (nsf) {
        std::cerr << std::format("Error ({}): {}", nsf.value(), nsf.message()) << "\n";
        return EXIT_FAILURE;
      }
    }
  }
  return EXIT_SUCCESS;
}
