#include "../../include/Queen.h"

Queen::Queen(Board* board, PieceColor color) : ChessPiece(board, color) {}

std::string Queen::toString() const {
    return "Q"; // Representação da peça Queen
}

// método canMove, verifica se a peça pode se mover para a posição passada como argumento
bool Queen::canMove(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p == nullptr || p->getColor() != getColor());
}

// método possibleMoves, retorna uma matriz de booleanos que indica as posições possíveis para a peça
std::vector<std::vector<bool>> Queen::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // diagonal superior esquerda
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn() - 1);
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow() - 1, p.getColumn() - 1);
    }

    // diagonal superior direita
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn() + 1);
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow() - 1, p.getColumn() + 1);
    }

    // diagonal inferior esquerda
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn() - 1);
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow() + 1, p.getColumn() - 1);
    }

    // diagonal inferior direita
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn() + 1);
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow() + 1, p.getColumn() + 1);
    }

    // vertical acima
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn());
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow() - 1, p.getColumn());
    }

    // vertical abaixo
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn());
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow() + 1, p.getColumn());
    }

    // horizontal esquerda
    p.setValues(getPosition().getRow(), getPosition().getColumn() - 1);
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow(), p.getColumn() - 1);
    }

    // horizontal direita
    p.setValues(getPosition().getRow(), getPosition().getColumn() + 1);
    while (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
        if (getBoard()->thereIsAPiece(p)) {
            break;
        }
        p.setValues(p.getRow(), p.getColumn() + 1);
    }

    return mat;
}