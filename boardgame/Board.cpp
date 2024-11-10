#include "Board.h"

Board::Board(int rows, int columns) : rows(rows), columns(columns), pieces(rows, std::vector<Piece*>(columns, nullptr)) {
    if (rows <= 0 || columns <= 0) {
        throw BoardException("Error creating board: there must be at least 1 row and 1 column!");
    }
}

int Board::getRows() const {
    return rows;
}

int Board::getColumns() const {
    return columns;
}

Piece* Board::piece(int row, int column) const {
    if (!positionExists(row, column)) {
        throw BoardException("Position (" + std::to_string(row) + ", " + std::to_string(column) + ") not on the board!");
    }
    return pieces[row][column];
}

Piece* Board::piece(Position position) const {
    if (!positionExists(position)) {
        throw BoardException("Position " + position.toString() + " not on the board!");
    }
    return pieces[position.getRow()][position.getColumn()];
}

void Board::placePiece(Piece* piece, Position position) {
    if (thereIsAPiece(position)) {
        throw BoardException("Error placing piece! Position " + position.toString() + " already taken!");
    }
    pieces[position.getRow()][position.getColumn()] = piece;
    piece->position = position;
}

bool Board::positionExists(int row, int column) const {
    return (row >= 0 && row < rows && column >= 0 && column < columns);
}

bool Board::positionExists(Position position) const {
    return positionExists(position.getRow(), position.getColumn());
}

bool Board::thereIsAPiece(Position position) const {
    if (!positionExists(position)) {
        throw BoardException("Position " + position.toString() + " not on the board!");
    }
    return piece(position) != nullptr;
}

Piece* Board::removePiece(Position position) {
    if (!positionExists(position)) {
        throw BoardException("Error removing piece: position " + position.toString() + " not on the board!");
    }
    if (piece(position) == nullptr) {
        return nullptr;
    }
    Piece* aux = piece(position);
    pieces[position.getRow()][position.getColumn()] = nullptr;
    aux->position = Position(-1, -1); // Set to an invalid position
    return aux;
}