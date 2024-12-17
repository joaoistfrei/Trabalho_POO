#include "../include/BoardException.h"

// cria uma excessao persanalizada para o tabuleiro de xadrez, para melhorar a legibilidade do código e facilitar compreensao de erros
BoardException::BoardException(const std::string& msg) : std::runtime_error(msg) {}
