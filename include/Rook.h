#ifndef ROOK_H
#define ROOK_H

#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include <vector>

class Rook : public ChessPiece {
public:
    Rook(Board* board, Color color);
    std::string toString() const override; // Adicionado o método toString
    std::vector<std::vector<bool>> possibleMoves() const override;
};

#endif // ROOK_H