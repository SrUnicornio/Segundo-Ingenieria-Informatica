/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramáticas en FNC
 *
 * @author 
 * @date 29/11/2025
 * @file grammar.cc
 * @brief Implementación de la clase Grammar.
 *        Incluye funciones de lectura, validación y almacenamiento
 *        de gramáticas independientes del contexto (CFG).
 *        Así como la conversión a Forma Normal de Chomsky (CNF).
 */
#include "../lib/grammar.h"

/**
 * @brief Carga una gramática desde un fichero y valida su estructura.
 * @param input_file_name Nombre del fichero de entrada.
 * @throw FileError Si el fichero no puede abrirse.
 * @throw GrammarFormatError Si la estructura del fichero es inválida.
 */
void Grammar::LoadGrammar(const std::string& input_file_name) {
  std::ifstream input(input_file_name);
  if (!input.is_open()) {
    throw FileError("No se pudo abrir el fichero de entrada: " +
                    input_file_name);
  }

  int line_number = 0;
  ReadSymbols(input, line_number, terminal_symbols_, true);
  ReadSymbols(input, line_number, non_terminal_symbols_, false);
  ReadProductions(input, line_number);
}

/**
 * @brief Comprueba si el símbolo inicial pertenece al conjunto de no
 * terminales.
 * @return true si el símbolo inicial está definido, false en caso contrario.
 */
bool Grammar::IsStartSymbolDefined() const {
  return non_terminal_symbols_.Contains(start_symbol_);
}

/**
 * @brief Guarda la gramática en un fichero de salida.
 * @param output_file_name Nombre del fichero de salida.
 * @throw FileError Si el fichero no puede abrirse.
 */
void Grammar::SaveGrammar(const std::string& output_file_name) const {
  std::ofstream out(output_file_name);
  if (!out.is_open()) throw FileError("No se pudo abrir el fichero de salida.");

  out << terminal_symbols_.Size() << "\n";
  for (const auto& s : terminal_symbols_.GetAlphabet()) out << s << "\n";

  out << non_terminal_symbols_.Size() << "\n";
  for (const auto& s : non_terminal_symbols_.GetAlphabet()) out << s << "\n";

  size_t count = 0;
  for (const auto& [left_side, right_side_set] : production_rules_)
    count += right_side_set.size();
  out << count << "\n";

  for (const auto& [left_side, right_side_list] : production_rules_) {
    for (const auto& right_side : right_side_list) {
      out << left_side << " ";
      for (const auto& s : right_side) out << s;
      out << "\n";
    }
  }
}

/**
 * @brief Lee un conjunto de símbolos (terminales o no terminales) desde un
 * fichero.
 * @param input Flujo de entrada del fichero.
 * @param line_number Contador de líneas (se incrementa durante la lectura).
 * @param alphabet Objeto Alphabet donde se almacenan los símbolos.
 * @param is_terminal true si se trata de símbolos terminales, false para no
 * terminales.
 * @throw GrammarFormatError Si los símbolos no cumplen el formato esperado.
 * @throw MissingStartSymbol Si el símbolo inicial no pertenece a los no
 * terminales.
 */
void Grammar::ReadSymbols(std::ifstream& input, int& line_number,
                          Alphabet& alphabet, bool is_terminal) {
  int count;
  input >> count;
  line_number++;

  const std::string symbol_type = is_terminal ? "terminal" : "no terminal";

  if (count <= 0) {
    throw GrammarFormatError("Número de símbolos " + symbol_type +
                             " inválido. (Línea " +
                             std::to_string(line_number) + ")");
  }

  for (int i = 0; i < count; ++i) {
    Symbol symbol;
    input >> symbol;
    line_number++;

    ValidateSymbol(symbol, line_number, is_terminal, alphabet);
  }

  // Validación extra solo para no terminales
  if (!is_terminal && !IsStartSymbolDefined()) {
    throw MissingStartSymbol("No se encuentra el símbolo inicial " +
                             std::string(1, start_symbol_.GetSymbol()) +
                             " en el conjunto de no terminales.");
  }

  if (alphabet.Size() != count) {
    throw GrammarFormatError("El número de símbolos " + symbol_type +
                             " no coincide con el declarado.");
  }
}

/**
 * @brief Valida un símbolo leído, según su tipo y si ya está en el alfabeto.
 * @param symbol Símbolo a validar.
 * @param line_number Línea actual del fichero.
 * @param is_terminal Indica si se trata de un símbolo terminal.
 * @param alphabet Alfabeto en el que se inserta el símbolo si es válido.
 * @throw GrammarFormatError Si el símbolo no cumple el formato
 * (mayúscula/minúscula).
 */
void Grammar::ValidateSymbol(const Symbol& symbol, int line_number,
                             bool is_terminal, Alphabet& alphabet) {
  const std::string symbol_type = is_terminal ? "terminal" : "no terminal";
  char c = symbol.GetSymbol();

  if (is_terminal && std::isupper(c)) {
    throw GrammarFormatError(
        "El símbolo terminal debe ser minúscula: " + std::string(1, c) +
        " (Línea " + std::to_string(line_number) + ")");
  }
  if (!is_terminal && !std::isupper(c)) {
    throw GrammarFormatError(
        "El símbolo no terminal debe ser mayúscula: " + std::string(1, c) +
        " (Línea " + std::to_string(line_number) + ")");
  }

  if (alphabet.Contains(symbol)) {
    std::cerr << "Warning: Símbolo " << symbol_type << " duplicado: " << c
              << " (Línea " << line_number << ")" << std::endl;
  } else {
    alphabet.InsertSymbol(symbol);
  }
}

/**
 * @brief Lee y valida las producciones de la gramática desde un fichero.
 * @param input Flujo de entrada del fichero.
 * @param line_number Contador de líneas (se incrementa durante la lectura).
 * @throw GrammarFormatError Si el formato de producción es inválido.
 * @throw EmptySymbol Si se detecta una producción vacía no permitida.
 * @throw UnitaryProductionError Si se detecta una producción unitaria.
 */
void Grammar::ReadProductions(std::ifstream& input, int& line_number) {
  int production_count;
  input >> production_count;
  line_number++;

  if (production_count <= 0) {
    throw GrammarFormatError("Número de producciones inválido. (Línea " +
                             std::to_string(line_number) + ")");
  }

  std::string left_side_str, right_side_str;
  for (int i = 0; i < production_count; ++i) {
    line_number++;
    if (!(input >> left_side_str >> right_side_str)) {
      throw GrammarFormatError("Formato incorrecto en la producción. (Línea " +
                               std::to_string(line_number) + ")");
    }

    Symbol left_side(left_side_str[0]);
    ValidateProduction(left_side, left_side_str, right_side_str, line_number);

    std::vector<Symbol> right_side;
    for (char c : right_side_str) {
      Symbol current(c);
      if (!terminal_symbols_.Contains(current) &&
          !non_terminal_symbols_.Contains(current) &&
          !current.IsVoidChainSymbol()) {
        throw GrammarFormatError("Símbolo no definido: " + std::string(1, c) +
                                 " (Línea " + std::to_string(line_number) +
                                 ")");
      }
      right_side.push_back(current);
    }

    production_rules_[left_side].push_back(right_side);
  }
  // Comprobar EOF después de leer todas las producciones y son distintas de empty
  std::string extra_line;
  while (input.peek() != EOF) {
    std::getline(input, extra_line);
    if (!extra_line.empty()) {
      throw GrammarFormatError("Líneas extra detectadas después de las "
                               "producciones: " + extra_line + " (Línea " +
                               std::to_string(line_number) + ")");
    }
    line_number++;
  }
  RemoveUnitProductions();
  VerifyNoUselessProductions();
}

/**
 * @brief Valida una producción completa (lado izquierdo y derecho).
 * @param left_side - Símbolo del lado izquierdo.
 * @param left_side_str - Cadena del lado izquierdo.
 * @param right_side_str - Cadena del lado derecho.
 * @param line_number - Línea actual del fichero.
 */
void Grammar::ValidateProduction(const Symbol& left_side,
                                  const std::string& left_side_str,
                                  const std::string& right_side_str,
                                  int line_number) {
  ValidateLeftSide(left_side, line_number);
  ValidateRightSide(left_side_str, right_side_str, line_number);
}

/**
 * @brief Valida que el lado izquierdo de una producción sea un no terminal
 * válido.
 * @param left_side Símbolo del lado izquierdo.
 * @param line_number Línea actual del fichero.
 * @throw GrammarFormatError Si el lado izquierdo no es un no terminal.
 */
void Grammar::ValidateLeftSide(const Symbol& left_side, int line_number) {
  if (!non_terminal_symbols_.Contains(left_side)) {
    throw GrammarFormatError(
        "Lado izquierdo no válido: " + std::string(1, left_side.GetSymbol()) +
        " (Línea " + std::to_string(line_number) + ")");
  }
}

/**
 * @brief Valida que el lado derecho de una producción cumpla las restricciones.
 * @param lhs Lado izquierdo (nombre del no terminal).
 * @param rhs Cadena del lado derecho.
 * @param line_number Línea actual del fichero.
 * @throw EmptySymbol Si la producción es vacía y no parte del símbolo inicial.
 * @throw UnitaryProductionError Si la producción es unitaria.
 */
void Grammar::ValidateRightSide(const std::string& lhs, const std::string& rhs,
                                int line_number) {
  const std::string kVoidChainSymbol = "&";
  const std::string kStartSymbolStr(1, start_symbol_.GetSymbol());
  if (rhs == kVoidChainSymbol && lhs != kStartSymbolStr) {
    throw EmptySymbol("Producción vacía no permitida: " + lhs +
                      kProductionArrow + kVoidChainSymbol + " (Línea " +
                      std::to_string(line_number) + ")");
  }

  // if (rhs.size() == 1 && std::isupper(rhs[0])) {
  //   throw UnitaryProductionError("Producción unitaria no permitida: " + lhs +
  //                                kProductionArrow + rhs + " (Línea " +
  //                                std::to_string(line_number) + ")");
  // }
}

/**
 * @brief Verifica si existen producciones o símbolos inútiles en la gramática.
 * Si existen, lanza una excepción y detiene el programa.
 * @throw GrammarSimplificationError Si hay símbolos no alcanzables o no
 * productivos.
 */
void Grammar::VerifyNoUselessProductions() const {
  auto productive = FindProductiveNonTerminals();
  auto reachable = FindReachableNonTerminals();

  std::vector<Symbol> non_productive;
  std::vector<Symbol> unreachable;

  for (const auto& nt : non_terminal_symbols_.GetAlphabet()) {
    if (!productive.count(nt)) non_productive.push_back(nt);
    if (!reachable.count(nt)) unreachable.push_back(nt);
  }

  if (!non_productive.empty() || !unreachable.empty()) {
    std::ostringstream oss;
    oss << "❌ La gramática contiene producciones inútiles:\n";
    if (!non_productive.empty()) {
      oss << " - No terminales no productivos: ";
      for (const auto& s : non_productive) oss << s << " ";
      oss << "\n";
    }
    if (!unreachable.empty()) {
      oss << " - No terminales no alcanzables desde "
          << start_symbol_.GetSymbol() << ": ";
      for (const auto& s : unreachable) oss << s << " ";
      oss << "\n";
    }
    throw UselessSymbolError(oss.str());
  }
}

/** Encuentra los no terminales productivos. */
std::set<Symbol> Grammar::FindProductiveNonTerminals() const {
  std::set<Symbol> productive;
  bool changed = true;

  for (const auto& [lhs, rhs_list] : production_rules_) {
    for (const auto& rhs : rhs_list) {
      bool all_terminals = true;
      for (const auto& sym : rhs) {
        if (non_terminal_symbols_.Contains(sym)) {
          all_terminals = false;
          break;
        }
      }
      if (all_terminals) {
        productive.insert(lhs);
        break;
      }
    }
  }

  while (changed) {
    changed = false;
    for (const auto& [lhs, rhs_list] : production_rules_) {
      if (productive.count(lhs)) continue;
      for (const auto& rhs : rhs_list) {
        bool all_good = true;
        for (const auto& sym : rhs) {
          if (non_terminal_symbols_.Contains(sym) && !productive.count(sym)) {
            all_good = false;
            break;
          }
        }
        if (all_good) {
          productive.insert(lhs);
          changed = true;
          break;
        }
      }
    }
  }

  return productive;
}

/**
 * @brief Calcula los no terminales alcanzables desde el símbolo inicial.
 */
std::set<Symbol> Grammar::FindReachableNonTerminals() const {
  std::set<Symbol> reachable{start_symbol_};
  std::set<Symbol> frontier{start_symbol_};

  while (!frontier.empty()) {
    Symbol current = *frontier.begin();
    frontier.erase(frontier.begin());

    auto it = production_rules_.find(current);
    if (it == production_rules_.end()) continue;

    for (const auto& rhs : it->second) {
      for (const auto& sym : rhs) {
        if (non_terminal_symbols_.Contains(sym) && !reachable.count(sym)) {
          reachable.insert(sym);
          frontier.insert(sym);
        }
      }
    }
  }

  return reachable;
}

/**
 * @brief Convierte la gramática a su Forma Normal de Chomsky (CNF).
 * @throw UselessSymbolError Si se detectan símbolos inútiles durante la
 * conversión.
 * @throw GrammarFormatError Si la gramática no puede convertirse a CNF.
 */
void Grammar::ConvertToChomskyNormalForm() {
  ReplaceTerminalsInLongProductions();
  BinarizeLongProductions();
}

/**
 * @brief Convierte un número entero a su representación en subíndices
 * @param numero - Número entero a convertir
 * @return Representación en subíndices del número
 */
static std::string convertirASubindice(int numero) {
  static const std::string subindices[] = {"₀", "₁", "₂", "₃", "₄",
                                           "₅", "₆", "₇", "₈", "₉"};
  if (numero == 0) {
    return subindices[0];
  }
  std::string resultado;
  for (char digito : std::to_string(numero)) {
    if (digito >= '0' && digito <= '9') {
      resultado += subindices[digito - '0'];
    } else {
      resultado += '?';
    }
  }
  return resultado;
}

/**
 * @brief Genera un nuevo no terminal único (A-Z) que no esté en uso.
 * @return Nuevo símbolo no terminal.
 * @throw GrammarFormatError Si se agotan los nombres disponibles.
 */
Symbol Grammar::GenerateNewNonTerminal() {
  static char current = 'A';
  while (current <= 'Z') {
    Symbol candidate(current);
    if (!non_terminal_symbols_.Contains(candidate) &&
        current != start_symbol_.GetSymbol()) {
      ++current;
      return candidate;
    }
    ++current;
  }
  throw GrammarFormatError(
      "Se agotaron los nombres de no terminales disponibles (A-Z).");
}

/**
 * @brief Reemplaza terminales en producciones largas (≥2) por nuevos no
 * terminales A(a), B(b), etc. Cada nuevo símbolo cumple A(a) → a.
 */
void Grammar::ReplaceTerminalsInLongProductions() {
  ProductionSet updated_rules;
  std::map<char, Symbol> terminal_map;

  Symbol base = GenerateNewNonTerminal();

  for (const auto& [lhs, rhs_list] : production_rules_) {
    for (const auto& rhs : rhs_list) {
      std::vector<Symbol> new_rhs = rhs;

      if (rhs.size() >= 2) {
        for (auto& sym : new_rhs) {
          if (terminal_symbols_.Contains(sym)) {
            char c = sym.GetSymbol();

            // Si no existe, crear A(c)
            if (!terminal_map.count(c)) {
              std::string new_name =
                  std::string(1, base.GetSymbol()) + "(" + std::string(1, c) + ")";
              Symbol new_nt(new_name);

              terminal_map[c] = new_nt;
              non_terminal_symbols_.InsertSymbol(new_nt);
              production_rules_[new_nt].push_back({sym});
            }

            sym = terminal_map[c];
          }
        }
      }

      updated_rules[lhs].push_back(new_rhs);
    }
  }

  production_rules_ = updated_rules;
}

/**
 * @brief Descompone producciones largas (>2) en binarias usando nuevos no
 * terminales.
 */
void Grammar::BinarizeLongProductions() {
  ProductionSet new_rules;
  Symbol next_prefix = GenerateNewNonTerminal();

  for (const auto& [lhs, rhs_list] : production_rules_) {
    for (const auto& rhs : rhs_list) {
      if (rhs.size() <= 2) {
        new_rules[lhs].push_back(rhs);
      } else {
        // Prefijo local único por producción compleja
        Symbol prefix = next_prefix;
        next_prefix = GenerateNewNonTerminal();
        int local_idx = 1;

        Symbol current_lhs = lhs;

        for (size_t i = 0; i < rhs.size() - 2; ++i) {
          std::string new_name =
              std::string(1, prefix.GetSymbol()) + convertirASubindice(local_idx++);
          Symbol new_nt(new_name);
          non_terminal_symbols_.InsertSymbol(new_nt);

          std::vector<Symbol> new_rhs = {rhs[i], new_nt};
          new_rules[current_lhs].push_back(new_rhs);

          current_lhs = new_nt;
        }

        // Última producción binaria
        std::vector<Symbol> final_rhs = {rhs[rhs.size() - 2], rhs.back()};
        new_rules[current_lhs].push_back(final_rhs);
      }
    }
  }

  production_rules_ = new_rules;
}

/**
 * @brief Sobrecarga del operador de salida para imprimir la gramática.
 * @param os Flujo de salida.
 * @param grammar Gramática a imprimir.
 * @return Flujo de salida modificado.
 */
std::ostream& operator<<(std::ostream& os, const Grammar& grammar) {
  os << "Símbolos Terminales:\n" + kSigma;
  os << grammar.terminal_symbols_;
  os << "\nSímbolos No Terminales:\n";
  os << grammar.non_terminal_symbols_;
  os << "\nSímbolo Inicial (S): " << grammar.start_symbol_ << "\n";
  os << "Producciones (P):\n";
  for (const auto& [lhs, rhs_list] : grammar.production_rules_) {
    os << "  " << lhs << " "
       << kProductionArrow;
    for (const auto& rhs : rhs_list) {
      for (const auto& sym : rhs) {
        os << sym;
      }
      if (&rhs != &rhs_list.back()) {
        os << " | ";
      }
    }
    os << "\n";
  }
  return os;
}

/**
 * @brief Elimina las producciones unitarias (A → B) de la gramática.
 * 
 * Construye el cierre transitivo H ⊆ (V × V) tal que (A, B) ∈ H ⇔ A ⇒* B
 * y reemplaza cada producción unitaria por las producciones de su destino.
 * 
 * @return Una gramática equivalente sin producciones unitarias.
 */
void Grammar::RemoveUnitProductions() {
  using Pair = std::pair<Symbol, Symbol>;

  // Paso 1: Inicializar H con todas las producciones unitarias
  std::set<Pair> H;
  for (const auto& [lhs, rhs_list] : production_rules_) {
    for (const auto& rhs : rhs_list) {
      if (rhs.size() == 1 && non_terminal_symbols_.Contains(rhs[0])) {
        H.insert({lhs, rhs[0]});
      }
    }
  }

  // Paso 2: Calcular el cierre transitivo de H
  bool changed = true;
  while (changed) {
    changed = false;
    std::set<Pair> new_pairs;

    for (const auto& [A, B] : H) {
      for (const auto& [C, D] : H) {
        if (B == C) {
          Pair AC = {A, D};
          if (!H.count(AC)) {
            new_pairs.insert(AC);
            changed = true;
          }
        }
      }
    }
    H.insert(new_pairs.begin(), new_pairs.end());
  }

  // Paso 3: Eliminar producciones unitarias de la gramática
  ProductionSet new_rules;
  for (const auto& [lhs, rhs_list] : production_rules_) {
    for (const auto& rhs : rhs_list) {
      // Mantener solo producciones NO unitarias
      if (!(rhs.size() == 1 && non_terminal_symbols_.Contains(rhs[0]))) {
        new_rules[lhs].push_back(rhs);
      }
    }
  }

  // Paso 4: Para cada (A,B) ∈ H, añadir producciones B → α como A → α (si no unitaria)
  for (const auto& [A, B] : H) {
    auto it = production_rules_.find(B);
    if (it != production_rules_.end()) {
      for (const auto& rhs : it->second) {
        // Evitar copiar producciones unitarias de nuevo
        if (!(rhs.size() == 1 && non_terminal_symbols_.Contains(rhs[0]))) {
          // Evitar duplicados exactos
          if (std::find(new_rules[A].begin(), new_rules[A].end(), rhs) ==
              new_rules[A].end()) {
            new_rules[A].push_back(rhs);
          }
        }
      }
    }
  }

  // Paso 5: Sustituir el conjunto original por el limpio
  production_rules_ = new_rules;
}
