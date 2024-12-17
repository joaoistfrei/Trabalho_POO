#include "../include/Piece.h"
#include "../include/Board.h"
#include <iostream>

Piece::Piece(Board* board) : position(Position()), board(board) {}

Piece::~Piece() = default;

// getters
Board* Piece::getBoard() const {
    return board;
}

Position Piece::getPosition() const {
    return position;
}

// setter
void Piece::setPosition(Position position) {
    this->position = position;
}

// retorna se um movimento é possivel
bool Piece::possibleMove(const Position& position) const {
    return possibleMoves()[position.getRow()][position.getColumn()];
}

// retorna se ha algum movimento possivel
bool Piece::isThereAnyPossibleMove() const {
    auto mat = possibleMoves();
            std::cout << "TESTELOOP" << std::endl;
    for (const auto& row : mat) {
        for (bool move : row) {
            if (move) {
                return true;
            }
        }
    }
    return false;
}