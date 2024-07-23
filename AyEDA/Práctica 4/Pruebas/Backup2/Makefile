# Directorios
SRC_DIR := src
LIB_DIR := lib
BUILD_DIR := build
BIN_DIR := Exec

# Compilador y opciones
CXX := g++
CXXFLAGS := -std=c++23 -Wall -I$(LIB_DIR)
LDFLAGS :=
RM := rm -rf

# Obtener todos los archivos .cc en el directorio src/
SRCS := $(wildcard $(SRC_DIR)/*.cc)

# Obtener todos los archivos .o en el directorio build/
OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(SRCS))

# Nombre del ejecutable
TARGET := $(BIN_DIR)/hash

# Reglas de construcción
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Asegurarse de que los directorios necesarios existan
$(shell mkdir -p $(BUILD_DIR) $(BIN_DIR))

# Limpiar archivos generados
clean:
	$(RM) $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean