#ifndef CHESSPOSITION_H
#define CHESSPOSITION_H

#include "Position.h"
#include "ChessException.h"
#include <string>

class ChessPosition {
private:
    char column;
    int row;

public:
    ChessPosition(char column, int row);

    char getColumn() const;
    int getRow() const;

    Position toPosition() const;
    static ChessPosition fromPosition(const Position& position);

    std::string toString() const;
};

#endif // CHESSPOSITION_H