#include "../../include/Rook.h"

Rook::Rook(Board* board, Color color) : ChessPiece(board, color) {}

std::string Rook::toString() const {
    return "R"; // Representação da peça Rook
}

std::vector<std::vector<bool>> Rook::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // checando em cima da torre
    p.setValues(position->getRow() - 1, position->getColumn());
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setRow(p.getRow() - 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // checando em baixo da torre
    p.setValues(position->getRow() + 1, position->getColumn());
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setRow(p.getRow() + 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // checando na direita da torre
    p.setValues(position->getRow(), position->getColumn() + 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setColumn(p.getColumn() + 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // checando na esquerda da torre
    p.setValues(position->getRow(), position->getColumn() - 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setColumn(p.getColumn() - 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    return mat;
}