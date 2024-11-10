#ifndef CHESSEXCEPTION_H
#define CHESSEXCEPTION_H

#include "BoardException.h"
#include <string>

class ChessException : public BoardException {
public:
    explicit ChessException(const std::string& msg);
};

#endif // CHESSEXCEPTION_H