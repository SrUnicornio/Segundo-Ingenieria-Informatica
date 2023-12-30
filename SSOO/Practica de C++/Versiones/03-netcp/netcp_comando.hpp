#ifndef COMANDO_HPP
#define COMANDO_HPP

#include <iostream>
#include <vector>
#include <cstring> 
// Gestion de errores          
#include <cerrno>
#include <sys/types.h>
// Comandos
# include <sys/wait.h>
// Otras
#include <signal.h>
#include <atomic>

class subprocess {
 public:
    enum class stdio {
      in, out, err, out_err
    };
    subprocess( const std::vector<std::string>& args, subprocess::stdio redircted_io) : arg_(args), redircted_io_(redircted_io) {};
    ~subprocess() {};

    bool is_alive();
    std::error_code exec();
    std::error_code wait();
    std::error_code killprocess();

    pid_t pid() { return pid_; }          // Devuelve el pid del proceso hijo
    int stdin_fd() { return in_fd_; }     // Devuelve el descriptor de lectura del pipe
    int stdout_fd() { return out_fd_; }   // Devuelve el descriptor de escritura del pipe

 private:
  const std::vector<std::string> arg_;
  subprocess::stdio redircted_io_;
  pid_t pid_{-1};
  int in_fd_{-1}, out_fd_{-1};
};

#endif // COMANDO_HPP