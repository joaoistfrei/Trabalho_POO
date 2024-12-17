#include "../../include/Rook.h"

Rook::Rook(Board* board, PieceColor color) : ChessPiece(board, color) {}

std::string Rook::toString() const {
    return "R"; // Representação da peça Rook
}

// método canMove, verifica se a peça pode se mover para a posição passada como argumento
bool Rook::canMove(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p == nullptr || p->getColor() != getColor());
}

// método possibleMoves, retorna uma matriz de booleanos que indica as posições possíveis para a peça
std::vector<std::vector<bool>> Rook::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // checando em cima da torre
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn());
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setRow(p.getRow() - 1);
    }
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // checando em baixo da torre
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn());
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setRow(p.getRow() + 1);
    }
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // checando na direita da torre
    p.setValues(getPosition().getRow(), getPosition().getColumn() + 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setColumn(p.getColumn() + 1);
    }
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // checando na esquerda da torre
    p.setValues(getPosition().getRow(), getPosition().getColumn() - 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setColumn(p.getColumn() - 1);
    }
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    return mat;
}