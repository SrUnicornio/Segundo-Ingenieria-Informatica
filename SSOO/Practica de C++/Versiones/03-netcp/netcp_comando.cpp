#include "netcp_comando.hpp"

/**
 * @brief Comprueba si el proceso hijo esta vivo.
 * @return true si el proceso hijo esta vivo, false en caso contrario.
 */
bool subprocess::is_alive() {
  if (pid_ == -1) {
    return false;
  }
  int status;
  pid_t result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {
    return false;
  } else if (result > 0) {
    if (WIFEXITED(status)) {
      return false;
    } else if (WIFSIGNALED(status)) {
      return false;
    }
  }
  return result;
}

/**
 * @brief Ejecuta el comando.
 * @return std::error_code con el codigo de error.
 */
std::error_code subprocess::exec() {
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
    
    exit(EXIT_FAILURE);
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

/**
 * @brief Espera a que el proceso hijo termine.
 * @return std::error_code con el codigo de error.
 */
std::error_code subprocess::wait() {
  if (pid_ == -1) {
    return std::error_code(errno, std::system_category());
  }
  int status;
  pid_t result = waitpid(pid_, &status, WNOHANG);
  if (result == -1) {
    return std::error_code(errno, std::system_category());
  } else if (result > 0) {
    if (WIFEXITED(status)) {
      return std::error_code(WEXITSTATUS(status), std::system_category());
    } else if (WIFSIGNALED(status)) {
      return std::error_code(WTERMSIG(status), std::system_category());
    }
  }
  return std::error_code(0, std::system_category());
}

/**
 * @brief Mata al proceso hijo.
 * @return std::error_code con el codigo de error.
 */
std::error_code subprocess::killprocess() {
  if (pid_ == -1) {
    return std::error_code(errno, std::system_category());
  }

  int result = ::kill(pid_, SIGTERM);
  if (result == -1) {
    return std::error_code(errno, std::system_category());
  }
  return std::error_code(0, std::system_category());
}
