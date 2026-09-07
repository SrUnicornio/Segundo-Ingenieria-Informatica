/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * @author 
 * @date 17/09/2025
 * @file main_functions.cc
 * @brief Fichero que contiene las funciones auxiliares del programa.
 *
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código
 *     - Implementación de las funciones de comprobación de argumentos
 *     - Implementación de las opciones del programa
 *     - Uso de std::function para procesar entradas genéricas
 *     - Mejora en la gestión de errores try-catch
 */

#include "../lib/functions_main.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

const std::string kHelpText =
    R"(Uso: ./p06_automata_simulator input.fa input.txt

Simula autómatas finitos (DFA/NFA).

Parámetros:
  input.fa   Archivo con el autómata
  input.txt  Archivo con las cadenas a evaluar

Formato .fa: alfabeto, #estados, estado inicial, estados (ID, final, #transiciones, transiciones)

Ejemplo:
  $ ./p06_automata_simulator automata.fa cadenas.txt
)";

/**
 * @brief Muestra el uso correcto del programa
 * @param argc - Número de argumentos
 * @param argv - Argumentos
 */
void Usage(int argc, char* argv[]) {
  if (argc != 3) {
    if (argc == 2) {
      std::string parameter{argv[1]};
      if (parameter == "--help" || parameter == "-h") {
        std::cout << kHelpText << std::endl;
        exit(EXIT_SUCCESS);
      }
    }
    std::cout << "Modo de empleo: " << argv[0] << " entrada.fa input.txt\n"
              << "Pruebe: " << argv[0] << " --help para más información."
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Comprueba que el fichero de entrada existe y se puede abrir
 * @param filename - Nombre del fichero de entrada
 * @throw std::cerr si el fichero no se puede abrir
 */
void CheckInputFile(const char* filename, Arguments* args) {
  std::string filename_str{filename};
  if (filename_str.substr(filename_str.find_last_of(".") + 1) != "txt") {
    throw FileError("Error: El archivo de entrada debe tener extensión .txt.");
  }
  std::ifstream file_in{filename};
  if (!file_in.is_open()) {
    std::cerr << "Error: No se pudo abrir el fichero de entrada \"" << filename
              << "\"." << std::endl;
    exit(EXIT_FAILURE);
  }
  file_in.close();
  args->input_file = filename;
}

/**
 * @brief Comprueba que el fichero de salida se puede crear o abrir
 * @param filename - Nombre del fichero de salida
 * @throw std::cerr si el fichero no se puede abrir
 */
void CheckInputAutomatonFile(const char* filename, Arguments* args) {
  std::string filename_str{filename};
  if (filename_str.substr(filename_str.find_last_of(".") + 1) != "fa") {
    throw FileError("Error: El archivo de autómata debe tener extensión .fa.");
  }
  std::ifstream file_in{filename};
  if (!file_in.is_open()) {
    std::cerr << "Error: No se pudo abrir el fichero de autómata \"" << filename
              << "\"." << std::endl;
    exit(EXIT_FAILURE);
  }
  file_in.close();
  args->input_automaton_file = filename;
}

/**
 * @brief Comprueba que los argumentos son correctos y los asigna a la
 * estructura Arguments
 * @param argc - Número de argumentos
 * @param argv - Argumentos
 * @param args - Estructura donde se almacenan los argumentos
 * @throw std::cerr si algún argumento no es válido
 */
void CheckArguments(int argc, char* argv[], Arguments* args) {
  Usage(argc, argv);
  CheckInputAutomatonFile(argv[1], args);
  CheckInputFile(argv[2], args);
}

void AutomataInformation(const Automaton& automaton) {
  std::cout << R"(Desea ver la información de su autómata? (S/N): )";
  char respuesta;
  std::cin >> respuesta;
  if (respuesta == 'S' || respuesta == 's' || respuesta == 'Y' ||
      respuesta == 'y') {
    std::cout << "Información del autómata:\n";
    std::cout << automaton << std::endl;
  }
}

/**
 * @brief Lee un autómata desde un archivo .fa
 * @param filename - ruta del archivo
 * @return Automaton construido
 */
Automaton ReadAutomaton(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw FileError("No se pudo abrir el archivo de autómata: " + filename);
  }

  std::string line;
  int line_number = 1;

  Alphabet alphabet = ReadAlphabet(line, file, line_number);
  int num_states = ReadNumberOfStates(line, file, line_number);
  State initial_state = ReadInitialState(line, file, line_number);

  std::set<State> all_states;
  Transition all_transitions;
  std::set<State> final_states;

  ReadStatesAndTransitions(num_states, alphabet, file, all_states,
                           all_transitions, final_states, line_number);

  file.close();

  auto automaton = CreateAutomaton(alphabet, all_states, initial_state,
                                   final_states, all_transitions);

  ValidateStateCount(num_states, automaton);
  return automaton;
}

/**
 * @brief Lee el alfabeto desde el archivo de autómata
 * @param line - línea temporal
 * @param file_in - archivo de autómata
 * @param line_number - línea actual del fichero
 * @return Alfabeto leído
 */
Alphabet ReadAlphabet(std::string& line, std::ifstream& file_in,
                      int line_number) {
  if (!std::getline(file_in, line)) {
    throw FileError("Error en línea " + std::to_string(line_number) +
                    ": no se pudo leer el alfabeto.");
  }

  std::istringstream iss(line);
  std::string symbol_str;
  std::string alphabet_string;
  while (iss >> symbol_str) alphabet_string += symbol_str;
  line_number++;
  return Alphabet(alphabet_string);
}

/**
 * @brief Lee el número de estados desde el archivo de autómata
 * @param line - línea temporal
 * @param file_in - archivo de autómata
 * @param line_number - línea actual del fichero
 * @return Número de estados leído
 * @throw FileError si el número de estados no es un entero positivo
 */
int ReadNumberOfStates(std::string& line, std::ifstream& file_in,
                       int line_number) {
  if (!std::getline(file_in, line)) {
    throw FileError("Error en línea " + std::to_string(line_number) +
                    ": no se pudo leer el número de estados.");
  }

  std::istringstream iss(line);
  if (std::count(line.begin(), line.end(), ' ') > 0) {
    throw FileError(
        "Error en línea " + std::to_string(line_number) +
        ": el número de estados debe ser un único entero positivo.");
  }

  int num_states;
  iss >> num_states;
  if (num_states <= 0) {
    throw FileError("Error en línea " + std::to_string(line_number) +
                    ": el número de estados debe ser positivo.");
  }
  line_number++;
  return num_states;
}

/**
 * @brief Lee el estado inicial desde el archivo de autómata
 * @param line - línea temporal
 * @param file_in - archivo de autómata
 * @param line_number - línea actual del fichero
 * @return Estado inicial leído
 * @throw FileError si el estado inicial está vacío o hay más de un estado
 */
State ReadInitialState(std::string& line, std::ifstream& file_in,
                       int line_number) {
  if (!std::getline(file_in, line)) {
    throw FileError("Error en línea " + std::to_string(line_number) +
                    ": no se pudo leer el estado inicial.");
  }

  std::istringstream iss(line);
  if (std::count(line.begin(), line.end(), ' ') > 0) {
    throw MoreThanOneInitialState("Error en línea " +
                                  std::to_string(line_number) +
                                  ": el estado inicial debe ser único.");
  }

  std::string initial_state;
  iss >> initial_state;
  if (initial_state.empty()) {
    throw NoInitialState("Error en línea " + std::to_string(line_number) +
                         ": el estado inicial no puede estar vacío.");
  }
  line_number++;
  return State(initial_state);
}

/**
 * @brief Lee la información básica de un estado: nombre, si es final y número
 * de transiciones
 * @param file_in - archivo de autómata
 * @param line_number - línea actual del fichero
 * @return tuple con nombre del estado, flag de final y número de transiciones
 * @throw FileError si faltan datos
 */
std::tuple<std::string, int, int> ReadStateInfo(std::ifstream& file_in,
                                                int line_number,
                                                int state_index) {
  std::string state_name;
  int is_final;
  int num_transitions;

  if (!(file_in >> state_name >> is_final >> num_transitions)) {
    throw FileError("Error en línea " + std::to_string(line_number) +
                    ": faltan datos para el estado " +
                    std::to_string(state_index + 1));
  }
  return {state_name, is_final, num_transitions};
}

/**
 * @brief Lee las transiciones de un estado y las valida.
 * @param file_in - flujo del archivo de autómata.
 * @param alphabet - alfabeto declarado del autómata.
 * @param state - estado origen desde el que salen las transiciones.
 * @param num_transitions - número de transiciones que debe leer.
 * @param transition - función de transición donde se insertarán.
 * @param line_number - número de línea actual (se actualiza dentro).
 * @throw FileError si hay errores en el formato, símbolos inválidos o estados
 * destino vacíos.
 */
void ReadTransitions(std::ifstream& file_in, const Alphabet& alphabet,
                     const State& state, int num_transitions,
                     Transition& transition, int& line_number) {
  for (int i = 0; i < num_transitions; ++i) {
    char symbol_char;
    std::string dest_name;

    if (!(file_in >> symbol_char >> dest_name)) {
      throw FileError("Error en línea " + std::to_string(line_number) +
                      ": se esperaban " + std::to_string(num_transitions) +
                      " transiciones para el estado '" + state.GetName() +
                      "'.");
    }

    Symbol symbol(symbol_char);
    if (!alphabet.Contains(symbol) && !symbol.IsVoidChainSymbol()) {
      throw FileError("Error en línea " + std::to_string(line_number) +
                      ": símbolo '" + std::string(1, symbol_char) +
                      "' en transición desde '" + state.GetName() +
                      "' no pertenece al alfabeto.");
    }
    if (dest_name.empty()) {
      throw FileError("Error en línea " + std::to_string(line_number) +
                      ": estado destino vacío en transición desde '" +
                      state.GetName() + "'.");
    }
    auto key = std::make_pair(state, symbol);
    transition.Insert(key, State(dest_name));
  }
}

/**
 * @brief Valida que los estados destino existan
 * @param all_transitions - vector con todas las transiciones
 * @param state_names - nombres de los estados válidos
 */
void ValidateDestinationStates(const Transition& all_transitions,
                               const std::set<State>& state_names) {
  for (const auto& trans : all_transitions.GetTransitions()) {
    const State& from_state = trans.first.first;
    const Symbol& symbol = trans.first.second;
    const std::set<State>& to_states = trans.second;

    for (const auto& to_state : to_states) {
      if (state_names.find(to_state) == state_names.end()) {
        std::cerr << "⚠️  Warning: La transición desde '" << from_state.GetName()
                  << "' con símbolo '" << symbol.GetSymbol()
                  << "' apunta a un estado inexistente: '" << to_state.GetName()
                  << "'.\n";
      }
    }
  }
}

/**
 * @brief Lee todos los estados y sus transiciones desde el archivo
 * @param num_states - número total de estados declarados
 * @param alphabet - alfabeto declarado
 * @param file_in - archivo de autómata
 * @param all_states - vector donde se almacenan todos los estados
 * @param all_transitions - vector donde se almacenan todas las transiciones
 * @param final_states - conjunto donde se almacenan los estados finales
 * @param line_number - línea actual del fichero (se actualizará dentro)
 */
void ReadStatesAndTransitions(const int& num_states, const Alphabet& alphabet,
                              std::ifstream& file_in,
                              std::set<State>& all_states,
                              Transition& all_transitions,
                              std::set<State>& final_states, int& line_number) {
  std::set<State> state_names;

  for (int i = 0; i < num_states; ++i) {
    auto [state_name, is_final, num_transitions] =
        ReadStateInfo(file_in, line_number, i);
    line_number++;  // Estado leído, incrementar línea

    State state(state_name);
    all_states.insert(state);
    state_names.insert(state);
    if (is_final) final_states.insert(state);

    ReadTransitions(file_in, alphabet, state, num_transitions, all_transitions,
                    line_number);
  }

  ValidateDestinationStates(all_transitions, state_names);
}

/**
 * @brief Comprueba si las transiciones definen un DFA
 * @param automaton - autómata a comprobar
 * @return true si es DFA, false si es NFA
 */
bool IsDFA(const Automaton& automaton) {
  const auto& transitions = automaton.GetTransition().GetTransitions();
  const auto& states = automaton.GetStates();
  const auto& alphabet = automaton.GetAlphabet().GetAlphabet();

  std::map<std::pair<std::string, char>, int> transition_count;

  for (const auto& entry : transitions) {
    const State& from = entry.first.first;
    const Symbol& symbol = entry.first.second;
    const std::set<State>& dest_states = entry.second;
    if (symbol.IsVoidChainSymbol()) return false;
    if (dest_states.size() != 1) return false;
    transition_count[{from.GetName(), symbol.GetSymbol()}]++;
    if (transition_count[{from.GetName(), symbol.GetSymbol()}] > 1)
      return false;  // duplicada
  }
  for (const auto& state : states) {
    for (const auto& sym : alphabet) {
      if (transition_count.find({state.GetName(), sym.GetSymbol()}) ==
          transition_count.end())
        return false;  // falta transición
    }
  }

  return true;
}

/**
 * @brief Crea un autómata (DFA o NFA) a partir de los componentes dados
 * @param is_dfa - true si es DFA, false si es NFA
 * @param alphabet - alfabeto del autómata
 * @param all_states - vector de todos los estados
 * @param initial_state - estado inicial
 * @param final_states - conjunto de estados finales
 * @param all_transitions - vector de todas las transiciones
 * @return Puntero único al autómata creado
 */
Automaton CreateAutomaton(const Alphabet& alphabet,
                          const std::set<State>& all_states,
                          const State& initial_state,
                          const std::set<State>& final_states,
                          const Transition& all_transitions) {
  Automaton automaton(alphabet);

  automaton.InsertState(all_states);
  automaton.SetInitialState(initial_state);
  automaton.AddFinalState(final_states);
  automaton.InsertTransition(all_transitions);

  return automaton;
}

/**
 * @brief Valida que el número de estados coincide con los definidos
 * @param num_states - número de estados declarado
 * @param automaton - autómata a validar
 */
void ValidateStateCount(int num_states, const Automaton& automaton) {
  if (num_states != static_cast<int>(automaton.GetStates().size())) {
    throw LessStatesThanDefined(
        "El número de estados no coincide con los estados definidos."
        " Número esperado: " +
        std::to_string(num_states) +
        ", Número encontrado: " + std::to_string(automaton.GetStates().size()));
  }
}

/**
 * @brief Valida que el autómata es correcto
 * @param automaton - autómata a validar
 * @throw InvalidState si el estado inicial o final no están en el conjunto de
 * estados
 * @throw NoFinalStates si no hay estados finales
 */
void ValidateAutomaton(const Automaton& automaton) {
  CheckInitialStateExists(automaton);
  CheckFinalStatesExist(automaton);

  auto reachable = IsReachableStates(automaton);

  CheckFinalStatesReachable(automaton, reachable);
  WarnUnreachableStates(automaton, reachable);
}

/**
 * @brief Valida que el estado inicial está en el conjunto de estados
 * @param automaton - autómata a validar
 */
void CheckInitialStateExists(const Automaton& automaton) {
  const auto& initial = automaton.GetInitialState();
  const auto& states = automaton.GetStates();
  if (states.find(initial) == states.end()) {
    throw InvalidState("El estado inicial '" + initial.GetName() +
                       "' no está definido en el conjunto de estados.");
  }
}

/**
 * @brief Valida que los estados finales están en el conjunto de estados
 * @param automaton - autómata a validar
 * @return conjunto de estados alcanzables desde el inicial
 * con epsilon-cierres
 */
std::set<State> IsReachableStates(const Automaton& automaton) {
  std::set<State> reachable;
  std::set<State> to_process;
  to_process.insert(automaton.GetInitialState());

  while (!to_process.empty()) {
    State current = *to_process.begin();
    to_process.erase(to_process.begin());
    if (reachable.find(current) == reachable.end()) {
      reachable.insert(current);
      const auto& alphabet = automaton.GetAlphabet().GetAlphabet();
      for (const auto& symbol : alphabet) {
        std::set<State> next_states =
            automaton.GetTransition().GetNextStates(current, symbol);
        for (const auto& ns : next_states) {
          if (reachable.find(ns) == reachable.end()) {
            to_process.insert(ns);
          }
        }
      }
      std::set<State> epsilon_closure = automaton.EpsilonClosure({current});
      for (const auto& es : epsilon_closure) {
        if (reachable.find(es) == reachable.end()) {
          to_process.insert(es);
        }
      }
    }
  }
  return reachable;
}

/**
 * @brief Valida que los estados finales están en el conjunto de estados
 * @param automaton - autómata a validar
 */
void CheckFinalStatesExist(const Automaton& automaton) {
  const auto& states = automaton.GetStates();
  const auto& final_states = automaton.GetFinalStates();

  if (final_states.empty()) {
    throw NoFinalStates("El autómata debe tener al menos un estado final.");
  }

  for (const auto& fs : final_states) {
    if (states.find(fs) == states.end()) {
      throw InvalidState("El estado final '" + fs.GetName() +
                         "' no está definido en el conjunto de estados.");
    }
  }
}

/**
 * @brief Valida que al menos un estado final es alcanzable desde el inicial
 * @param automaton - autómata a validar
 * @param reachable - conjunto de estados alcanzables
 */
void CheckFinalStatesReachable(const Automaton& automaton,
                               const std::set<State>& reachable) {
  const auto& final_states = automaton.GetFinalStates();
  for (const auto& fs : final_states) {
    if (reachable.find(fs) != reachable.end()) return;
  }
  throw NonReachableState(
      "Error: Ningún estado final es alcanzable desde el estado inicial.");
}

/**
 * @brief Muestra una advertencia por cada estado inalcanzable desde el inicial
 * @param automaton - autómata a validar
 * @param reachable - conjunto de estados alcanzables
 */
void WarnUnreachableStates(const Automaton& automaton,
                           const std::set<State>& reachable) {
  for (const auto& state : automaton.GetStates()) {
    if (reachable.find(state) == reachable.end()) {
      std::cerr << "⚠️  Warning: Estado '" << state.GetName()
                << "' es inalcanzable desde el inicial.\n";
    }
  }
}

/**
 * @brief Detecta estados con transiciones equivalentes (misma salida para todos
 * los símbolos)
 * @param automaton - autómata a validar
 * @param equivalent_states - función de transición del autómata
 */
void WarnEquivalentStates(const Automaton& automaton,
                          const Transition& equivalent_states) {
  const auto& alphabet = automaton.GetAlphabet().GetAlphabet();
  const auto& states = automaton.GetStates();

  std::map<std::string, std::map<Symbol, std::set<State>>> transitions_map;

  // Construir la tabla de transiciones para cada estado
  for (const auto& st : states) {
    for (const auto& sym : alphabet) {
      transitions_map[st.GetName()][sym] =
          equivalent_states.GetNextStates(st, sym);
    }
  }

  // Comparar comportamientos de cada par de estados
  for (auto it1 = transitions_map.begin(); it1 != transitions_map.end();
       ++it1) {
    for (auto it2 = std::next(it1); it2 != transitions_map.end(); ++it2) {
      bool equivalent = true;
      for (const auto& sym : alphabet) {
        if (it1->second.at(sym) != it2->second.at(sym)) {
          equivalent = false;
          break;
        }
      }
      if (equivalent) {
        std::cerr
            << "⚠️  Warning: Estados '" << it1->first << "' y '" << it2->first
            << "' tienen transiciones equivalentes para todo el alfabeto.\n";
      }
    }
  }
}

/**
 * @brief Procesa un archivo .txt con cadenas y las evalúa en el autómata
 * @param filename - archivo con cadenas
 * @param automaton - autómata cargado
 */
void ProcessInputFile(const std::string& filename, const Automaton& automaton) {
  std::ifstream input_file(filename);
  if (!input_file.is_open()) {
    throw FileError("No se pudo abrir el archivo de cadenas: " + filename);
  }

  std::string chain_str;
  while (input_file >> chain_str) {
    Chain chain(chain_str, automaton.GetAlphabet());
    bool accepted = automaton.Accepts(chain);

    std::cout << chain << " --> " << (accepted ? "ACEPTADA ✅" : "RECHAZADA ❌")
              << std::endl;
  }

  input_file.close();
}