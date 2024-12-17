#include "../include/ChessException.h"

// cria uma excessao persanalizada para o xadrez, para melhorar a legibilidade do código e facilitar compreensao de erros
ChessException::ChessException(const std::string& msg) : BoardException(msg) {}