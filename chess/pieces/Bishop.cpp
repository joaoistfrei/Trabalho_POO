#include "../../include/Bishop.h"

Bishop::Bishop(Board* board, Color color) : ChessPiece(board, color) {}

std::string Bishop::toString() const {
    return "B"; // Representação da peça Rook
}

std::vector<std::vector<bool>> Bishop::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // diagonal superior esquerda
    p.setValues(position->getRow() - 1, position->getColumn() - 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setValues(p.getRow() - 1, p.getColumn() - 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // diagonal superior direita
    p.setValues(position->getRow() - 1, position->getColumn() + 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setValues(p.getRow() - 1, p.getColumn() + 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // diagonal inferior esquerda
    p.setValues(position->getRow() + 1, position->getColumn() - 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setValues(p.getRow() + 1, p.getColumn() - 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // diagonal inferior direita
    p.setValues(position->getRow() + 1, position->getColumn() + 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setValues(p.getRow() + 1, p.getColumn() + 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    return mat;
}