#include "../include/BoardException.h"

BoardException::BoardException(const std::string& msg) : std::runtime_error(msg) {}
