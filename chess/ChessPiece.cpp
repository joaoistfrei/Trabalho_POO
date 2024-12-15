#include "../include/ChessPiece.h"

ChessPiece::ChessPiece(Board* board, PieceColor color) : Piece(board), color(color), moveCount(0) {}

PieceColor ChessPiece::getColor() const {
    return color;
}

int ChessPiece::getMoveCount() const {
    return moveCount;
}

void ChessPiece::increaseMoveCount() {
    moveCount++;
}

void ChessPiece::decreaseMoveCount() {
    moveCount--;
}

ChessPosition ChessPiece::getChessPosition() const {
    return ChessPosition::fromPosition(position);
}

bool ChessPiece::isThereOpponentPiece(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p != nullptr && p->getColor() != color);
}

ChessPiece* ChessPiece::toChessPiece(Piece* piece) {
    return dynamic_cast<ChessPiece*>(piece);
}