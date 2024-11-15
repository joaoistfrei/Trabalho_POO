#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include "BoardException.h"
#include "Piece.h"
#include "Position.h"

class Board {
private:
    int rows;
    int columns;
    std::vector<std::vector<Piece*>> pieces;

public:
    Board(int rows, int columns);

    int getRows() const; // Adicionado
    int getColumns() const; // Adicionado

    Piece* piece(int row, int column) const;
    Piece* piece(Position position) const;

    //Piece *piece(Position position);

    void placePiece(Piece *piece, Position position);

protected:
    bool positionExists(int row, int column) const;

public:
    bool positionExists(Position position) const;
    bool thereIsAPiece(Position position) const;
    Piece* removePiece(Position position);
};

#endif // BOARD_H