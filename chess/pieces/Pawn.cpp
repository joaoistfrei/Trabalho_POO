#include "../../include/Pawn.h"
#include "../../include/ChessMatch.h"

Pawn::Pawn(Board* board, PieceColor color, ChessMatch* chessMatch) : ChessPiece(board, color), chessMatch(chessMatch) {}

std::string Pawn::toString() const {
    return "P"; // Representação da peça Rook
}

std::vector<std::vector<bool>> Pawn::possibleMoves() const {
    std::vector<std::vector<bool>> mat(getBoard()->getRows(), std::vector<bool>(getBoard()->getColumns(), false));
    Position p(0, 0);

    if (getColor() == PieceColor::W) {
        p.setValues(position.getRow() - 1, position.getColumn());
        if (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
            mat[p.getRow()][p.getColumn()] = true;
        }
        Position p2(position.getRow() - 2, position.getColumn());
        if (getBoard()->positionExists(p2) && !getBoard()->thereIsAPiece(p2) && getMoveCount() == 0) {
            mat[p2.getRow()][p2.getColumn()] = true;
        }
        p.setValues(position.getRow() - 1, position.getColumn() - 1);
        if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
            mat[p.getRow()][p.getColumn()] = true;
        }
        p.setValues(position.getRow() - 1, position.getColumn() + 1);
        if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
            mat[p.getRow()][p.getColumn()] = true;
        }

        // En passant
        if (position.getRow() == 3) {
            Position left(position.getRow(), position.getColumn() - 1);
            if (getBoard()->positionExists(left) && isThereOpponentPiece(left) && getBoard()->piece(left) == chessMatch->getEnPassantVulnerable()) {
                mat[left.getRow() - 1][left.getColumn()] = true;
            }
            Position right(position.getRow(), position.getColumn() + 1);
            if (getBoard()->positionExists(right) && isThereOpponentPiece(right) && getBoard()->piece(right) == chessMatch->getEnPassantVulnerable()) {
                mat[right.getRow() - 1][right.getColumn()] = true;
            }
        }
    } else {
        p.setValues(position.getRow() + 1, position.getColumn());
        if (getBoard()->positionExists(p) && !getBoard()->thereIsAPiece(p)) {
            mat[p.getRow()][p.getColumn()] = true;
        }
        Position p2(position.getRow() + 2, position.getColumn());
        if (getBoard()->positionExists(p2) && !getBoard()->thereIsAPiece(p2) && getMoveCount() == 0) {
            mat[p2.getRow()][p2.getColumn()] = true;
        }
        p.setValues(position.getRow() + 1, position.getColumn() - 1);
        if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
            mat[p.getRow()][p.getColumn()] = true;
        }
        p.setValues(position.getRow() + 1, position.getColumn() + 1);
        if (getBoard()->positionExists(p) && isThereOpponentPiece(p)) {
            mat[p.getRow()][p.getColumn()] = true;
        }

        // En passant
        if (position.getRow() == 4) {
            Position left(position.getRow(), position.getColumn() - 1);
            if (getBoard()->positionExists(left) && isThereOpponentPiece(left) && getBoard()->piece(left) == chessMatch->getEnPassantVulnerable()) {
                mat[left.getRow() + 1][left.getColumn()] = true;
            }
            Position right(position.getRow(), position.getColumn() + 1);
            if (getBoard()->positionExists(right) && isThereOpponentPiece(right) && getBoard()->piece(right) == chessMatch->getEnPassantVulnerable()) {
                mat[right.getRow() + 1][right.getColumn()] = true;
            }
        }
    }

    return mat;
}