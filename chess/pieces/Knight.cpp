#include "../../include/Knight.h"

Knight::Knight(Board* board, Color color) : ChessPiece(board, color) {}

std::string Knight::toString() const {
    return "G"; // Representação da peça Rook
}

bool Knight::canMove(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p == nullptr || p->getColor() != getColor());
}

std::vector<std::vector<bool>> Knight::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // cima esquerda
    p.setValues(position->getRow() - 2, position->getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // cima direita
    p.setValues(position->getRow() - 2, position->getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // direita cima
    p.setValues(position->getRow() - 1, position->getColumn() + 2);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // direita baixo
    p.setValues(position->getRow() + 1, position->getColumn() + 2);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // esquerda cima
    p.setValues(position->getRow() - 1, position->getColumn() - 2);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // esquerda baixo
    p.setValues(position->getRow() + 1, position->getColumn() - 2);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // baixo direita
    p.setValues(position->getRow() + 2, position->getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // baixo esquerda
    p.setValues(position->getRow() + 2, position->getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    return mat;
}