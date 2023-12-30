#ifndef NETCP_HPP
#define NETCP_HPP

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
#include "netcp_comando.hpp"
// Otras
#include <signal.h>
#include <atomic>

using open_file_result = std::expected<int, std::error_code>;
using make_socket_result = std::expected<int, std::error_code>;
using std::experimental::scope_exit;

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

constexpr unsigned int str2int(const char *str, int h);
std::optional<program_options> parse_args(int argc, char *argv[]);
void Usage();

open_file_result open_file(const std::string &path, int flags, mode_t mode);
std::error_code read_file(int fd, std::vector<uint8_t> &buffer);
std::error_code write_file(int fd, const std::vector<uint8_t>& buffer);
std::error_code copy_file(const std::string& src_path, const std::string& dst_path);

std::optional<sockaddr_in> make_ip_address(const std::optional<std::string> &ip_address, uint16_t port);
make_socket_result make_socket(std::optional<sockaddr_in> address);

std::error_code send_to(int fd, const std::vector<uint8_t> &message, const sockaddr_in& address);
std::error_code receive_from(int sock_fd, std::vector<uint8_t>& message, sockaddr_in& address);

std::string getenv(const std::string& name);
std::error_code netcp_send_file(const std::string& filename);
std::error_code netcp_receive_file(const std::string& filename);

std::error_code netcp_send_command(const std::vector<std::string> args, subprocess::stdio mode);
std::error_code netcp_receive_command(const std::vector<std::string> args, subprocess::stdio mode);

void signal_err(int signal);

#endif //NETCP_HPP