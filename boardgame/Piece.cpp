#include "../include/Piece.h"

Piece::Piece(Board* board) : position(Position()), board(board) {}

Piece::~Piece() = default;

Board* Piece::getBoard() const {
    return board;
}

bool Piece::possibleMove(const Position& position) const {
    return possibleMoves()[position.getRow()][position.getColumn()];
}

bool Piece::isThereAnyPossibleMove() const {
    auto mat = possibleMoves();
    for (const auto& row : mat) {
        for (bool move : row) {
            if (move) {
                return true;
            }
        }
    }
    return false;
}