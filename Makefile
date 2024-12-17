# compilador e as flags relevantes
CXX = g++
CXXFLAGS = -Iinclude -c -std=c++14 -Wall -Wextra -g 
LDFLAGS = -lraylib -fPIE

# diretorios com os arquivos-fonte
SRC_DIRS = boardgame chess chess/pieces application

# encontrar todos os arquivos .cpp nos diretórios de origem
SOURCES = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))

# gerar arquivos de objeto (.o) a partir dos arquivos fonte (.cpp)
OBJECTS = $(patsubst %.cpp, bin/%.o, $(SOURCES))

# executavel final
TARGET = bin/my_program

# regra padrao de compilar o programa
all: $(TARGET)

# regra para o linking (gerar o exec a partir dos arquivos de objeto)
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# regra para compilar os arquivos .cpp em arquivos .o
bin/%.o: %.cpp
	@mkdir -p $(dir $@) # Cria os diretórios, se necessário
	$(CXX) $(CXXFLAGS) $< -o $@

# regra para compilar e executar o programa
run: all
	@echo "Executando $(TARGET)..."
	./$(TARGET)

# regra para limpar os arquivos gerados durante a compilação
clean:
	rm -f $(OBJECTS) $(TARGET) # Remove os arquivos-objeto e o executável
	rm -rf bin                # Remove o diretório 'bin'

# declaracao de regras "PHONY" para evitar conflitos com arquivos que possam ter o mesmo nome
.PHONY: all clean run
