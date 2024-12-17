#include "../include/ChessPiece.h"

ChessPiece::ChessPiece(Board* board, PieceColor color) : Piece(board), color(color), moveCount(0) {}

// getters
PieceColor ChessPiece::getColor() const {
    return color;
}

int ChessPiece::getMoveCount() const {
    return moveCount;
}

ChessPosition ChessPiece::getChessPosition() const {
    return ChessPosition::fromPosition(position);
}

// aumenta o numero de movimentos da peca
void ChessPiece::increaseMoveCount() {
    moveCount++;
}

// diminui o numero de movimentos da peca
void ChessPiece::decreaseMoveCount() {
    moveCount--;
}

// ve se ha uma peca oponente em uma posicao em analise
bool ChessPiece::isThereOpponentPiece(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p != nullptr && p->getColor() != color);
}

// converte a posicao matricial em posicao de xadrez
ChessPiece* ChessPiece::toChessPiece(Piece* piece) {
    return dynamic_cast<ChessPiece*>(piece);
}