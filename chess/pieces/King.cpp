#include "../../include/King.h"
#include "../../include/ChessMatch.h"

std::string King::toString() const {
    return "K"; // Representação da peça Rook
}

King::King(Board* board, Color color, ChessMatch* chessMatch) : ChessPiece(board, color), chessMatch(chessMatch) {}

bool King::canMove(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p == nullptr || p->getColor() != getColor());
}

bool King::testRookCastling(Position position) const {
    ChessPiece* p = dynamic_cast<ChessPiece*>(getBoard()->piece(position));
    return (p != nullptr && p->getMoveCount() == 0 && dynamic_cast<Rook*>(p) != nullptr && p->getColor() == getColor());
}

std::vector<std::vector<bool>> King::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    // above
    p.setValues(position->getRow() - 1, position->getColumn());
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // below
    p.setValues(position->getRow() + 1, position->getColumn());
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // left
    p.setValues(position->getRow(), position->getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // right
    p.setValues(position->getRow(), position->getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // above right
    p.setValues(position->getRow() - 1, position->getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // above left
    p.setValues(position->getRow() - 1, position->getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // below right
    p.setValues(position->getRow() + 1, position->getColumn() + 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // below left
    p.setValues(position->getRow() + 1, position->getColumn() - 1);
    if (getBoard()->positionExists(p) && canMove(p)) {
        mat[p.getRow()][p.getColumn()] = true;
    }

    // Castling
    if (getMoveCount() == 0 && !chessMatch->getCheck()) {
        // King side castling
        Position t1(position->getRow(), position->getColumn() + 3);
        if (testRookCastling(t1)) {
            Position p1(position->getRow(), position->getColumn() + 1);
            Position p2(position->getRow(), position->getColumn() + 2);
            if (getBoard()->piece(p1) == nullptr && getBoard()->piece(p2) == nullptr) {
                mat[position->getRow()][position->getColumn() + 2] = true;
            }
        }
        // Queen side castling
        Position t2(position->getRow(), position->getColumn() - 4);
        if (testRookCastling(t2)) {
            Position p1(position->getRow(), position->getColumn() - 1);
            Position p2(position->getRow(), position->getColumn() - 2);
            Position p3(position->getRow(), position->getColumn() - 3);
            if (getBoard()->piece(p1) == nullptr && getBoard()->piece(p2) == nullptr && getBoard()->piece(p3) == nullptr) {
                mat[position->getRow()][position->getColumn() - 2] = true;
            }
        }
    }

    return mat;
}