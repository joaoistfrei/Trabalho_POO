#ifndef PIECE_H
#define PIECE_H

#include "Position.h"
#include "Color.h"

#include <vector>

class Board;

class Piece {
protected:
    Position position;
    Board* board;

public:
    Piece();
    Piece(Board* board);
    virtual ~Piece();

    Board* getBoard() const;
    virtual std::vector<std::vector<bool>> possibleMoves() const = 0;
    bool possibleMove(const Position& position) const;
    bool isThereAnyPossibleMove() const;
    friend class Board;
    friend class Position;
    friend class BoardException;

};

#endif // PIECE_H