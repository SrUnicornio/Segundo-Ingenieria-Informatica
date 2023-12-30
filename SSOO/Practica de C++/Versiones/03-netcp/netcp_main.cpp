#include "netcp_comando.hpp"
#include "netcp.hpp"

int main(int argc, char *argv[]) {
  // Configurar manejadores de señales
  struct sigaction term_action = {};
  term_action.sa_handler = &signal_err;
  sigaction( SIGTERM, &term_action, NULL ); // SIGTERM: kill -TERM pid o kill -15 pid
  sigaction( SIGINT, &term_action, NULL );  // SIGINT: kill -INT pid o kill -2 pid
  sigaction( SIGHUP, &term_action, NULL );  // SIGHUP: kill -HUP pid o kill -1 pid
  sigaction( SIGQUIT, &term_action, NULL ); // SIGQUIT: kill -QUIT pid o kill -3 pid

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
      std::error_code nsc = netcp_receive_command(args, subprocess::stdio::in);
      if (nsc) {
        std::cerr << std::format("Error ({}): {}", nsc.value(), nsc.message()) << "\n";
        return EXIT_FAILURE;
      }
    } else {
      std::cout << "Enviando\n";
      std::error_code nrc;
      if (options->capture_stout && options->capture_error) {
        nrc = netcp_send_command(args, subprocess::stdio::out_err);
      } else if (options->capture_stout) {
        nrc = netcp_send_command(args, subprocess::stdio::out);
      } else if (options->capture_error) {
        nrc = netcp_send_command(args, subprocess::stdio::err);
      } else {
        nrc = netcp_send_command(args, subprocess::stdio::out);
      }
      if (nrc) {
        std::cerr << std::format("Error ({}): {}", nrc.value(), nrc.message()) << "\n";
        return EXIT_FAILURE;
      }
    }
    std::cout << "Fin\n";
    return EXIT_SUCCESS;
  } else {
    std::cout << "Archivo\n";
    if (options->listen) {
      std::cout << "Escuchando\n";
      std::error_code nrf = netcp_receive_file(options->output_filename);
      if (nrf) {
        std::cerr << std::format("Error ({}): {}", nrf.value(), nrf.message()) << "\n";
        return EXIT_FAILURE;
      }
      std::cout << "Fin\n";
      return EXIT_SUCCESS;
    } else {
      std::cout << "Enviando\n";
      std::error_code nsf = netcp_send_file(options->input_filename);
      if (nsf) {
        std::cerr << std::format("Error ({}): {}", nsf.value(), nsf.message()) << "\n";
        return EXIT_FAILURE;
      }
      std::cout << "Fin\n";
      return EXIT_SUCCESS;
    }
  }
  
  return EXIT_SUCCESS;
}
