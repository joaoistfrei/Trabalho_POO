#include "../../include/Queen.h"

Queen::Queen(Board* board, Color color) : ChessPiece(board, color) {}

std::string Queen::toString() const {
    return "Q"; // Representação da peça Rook
}

std::vector<std::vector<bool>> Queen::possibleMoves() const {
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

    // cima
    p.setValues(position->getRow() - 1, position->getColumn());
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setRow(p.getRow() - 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // baixo
    p.setValues(position->getRow() + 1, position->getColumn());
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setRow(p.getRow() + 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // direita 
    p.setValues(position->getRow(), position->getColumn() + 1);
    while (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        p.setColumn(p.getColumn() + 1);
    }
    if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // esquerda
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