#include "../../include/King.h"
#include "../../include/ChessMatch.h"
#include "../../include/Rook.h"

// classe King, herda de ChessPiece

std::string King::toString() const {
    return "K"; // Representação da peça King
}

// a peca do rei precisa ter acesso ao jogo, para poder julgar a possibilidade de castling

King::King(Board* board, PieceColor color, ChessMatch* chessMatch) : ChessPiece(board, color), chessMatch(chessMatch) {}

// funcao para checar movimentacao do rei
bool King::canMove(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p == nullptr || p->getColor() != getColor());
}

// testa se eh possivel fazer o castling (caso as pecas estejam nas posicoes corretas e nao tenham se movido)
bool King::testRookCastling(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p != nullptr && p->getMoveCount() == 0 && dynamic_cast<Rook*>(p) != nullptr && p->getColor() == getColor());
}

// retorna as posicoes possiveis para o rei
std::vector<std::vector<bool>> King::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // above
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn());
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // below
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn());
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // left
    p.setValues(getPosition().getRow(), getPosition().getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // right
    p.setValues(getPosition().getRow(), getPosition().getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // above right
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // above left
    p.setValues(getPosition().getRow() - 1, getPosition().getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // below right
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // below left
    p.setValues(getPosition().getRow() + 1, getPosition().getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // Castling kingside
    if (getMoveCount() == 0 && !chessMatch->getCheck()) {
        // Kingside rook
        Position posT1(getPosition().getRow(), getPosition().getColumn() + 3);
        if (testRookCastling(posT1)) {
            Position p1(getPosition().getRow(), getPosition().getColumn() + 1);
            Position p2(getPosition().getRow(), getPosition().getColumn() + 2);
            if (getBoard()->piece(p1) == nullptr && getBoard()->piece(p2) == nullptr) {
                mat[getPosition().getRow()][getPosition().getColumn() + 2] = true;
            }
        }
        // Queenside rook
        Position posT2(getPosition().getRow(), getPosition().getColumn() - 4);
        if (testRookCastling(posT2)) {
            Position p1(getPosition().getRow(), getPosition().getColumn() - 1);
            Position p2(getPosition().getRow(), getPosition().getColumn() - 2);
            Position p3(getPosition().getRow(), getPosition().getColumn() - 3);
            if (getBoard()->piece(p1) == nullptr && getBoard()->piece(p2) == nullptr && getBoard()->piece(p3) == nullptr) {
                mat[getPosition().getRow()][getPosition().getColumn() - 2] = true;
            }
        }
    }

    return mat;
}