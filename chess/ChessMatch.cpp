#include "ChessMatch.h"
#include "ChessPiece.h"
#include "Board.h"
#include "Position.h"
#include "ChessPosition.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include <algorithm>
#include <stdexcept>

ChessMatch::ChessMatch() : board(8, 8), turn(1), currentPlayer(Color::WHITE), check(false), checkMate(false), enPassantVulnerable(nullptr), promoted(nullptr) {
    initialSetup();
}

int ChessMatch::getTurn() const {
    return turn;
}

bool ChessMatch::getCheck() const {
    return check;
}

bool ChessMatch::getCheckMate() const {
    return checkMate;
}

Color ChessMatch::getCurrentPlayer() const {
    return currentPlayer;
}

ChessPiece* ChessMatch::getEnPassantVulnerable() const {
    return enPassantVulnerable;
}

ChessPiece* ChessMatch::getPromoted() const {
    return promoted;
}

ChessPiece* ChessMatch::replacePromotedPiece(const std::string& type) {
    if (promoted == nullptr) {
        throw std::logic_error("There is no piece to be promoted!");
    }
    if (type != "B" && type != "N" && type != "R" && type != "Q") {
        return promoted;
    }

    Position pos = promoted->getChessPosition().toPosition();
    Piece* p = board.removePiece(pos);
    piecesOnTheBoard.erase(std::remove_if(piecesOnTheBoard.begin(), piecesOnTheBoard.end(),
                                          [p](const std::unique_ptr<Piece>& piece) { return piece.get() == p; }),
                           piecesOnTheBoard.end());

    std::unique_ptr<ChessPiece> newPiece = createNewPiece(type, promoted->getColor());
    board.placePiece(newPiece.get(), pos);
    piecesOnTheBoard.push_back(std::move(newPiece));

    return promoted;
}

std::unique_ptr<ChessPiece> ChessMatch::createNewPiece(const std::string& type, Color color) {
    if (type == "B") return std::make_unique<Bishop>(&board, color);
    if (type == "N") return std::make_unique<Knight>(&board, color);
    if (type == "R") return std::make_unique<Rook>(&board, color);
    return std::make_unique<Queen>(&board, color);
}

ChessPiece* ChessMatch::performChessMove(const ChessPosition& fromPosition, const ChessPosition& toPosition) {
    Position from = fromPosition.toPosition();
    Position to = toPosition.toPosition();
    validateSourcePosition(from);
    validateTargetPosition(from, to);
    Piece* capturedPiece = makeMove(from, to);

    if (testCheck(currentPlayer)) {
        undoMove(from, to, capturedPiece);
        throw std::logic_error("You can't place yourself in check!");
    }

    ChessPiece* movedPiece = dynamic_cast<ChessPiece*>(board.piece(to));

    // Promotion
    promoted = nullptr;
    if (dynamic_cast<Pawn*>(movedPiece)) {
        if ((movedPiece->getColor() == Color::WHITE && to.getRow() == 0) || (movedPiece->getColor() == Color::BLACK && to.getRow() == 7)) {
            promoted = dynamic_cast<ChessPiece*>(board.piece(to));
            promoted = replacePromotedPiece("Q");
        }
    }

    check = testCheck(opponent(currentPlayer));

    if (testCheckMate(opponent(currentPlayer))) {
        checkMate = true;
    } else {
        nextTurn();
    }

    // En passant
    if (dynamic_cast<Pawn*>(movedPiece) && (to.getRow() == from.getRow() + 2 || to.getRow() == from.getRow() - 2)) {
        enPassantVulnerable = movedPiece;
    } else {
        enPassantVulnerable = nullptr;
    }

    return capturedPiece;
}

Piece* ChessMatch::makeMove(const Position& source, const Position& target) {
    ChessPiece* p = dynamic_cast<ChessPiece*>(board.removePiece(source));
    p->increaseMoveCount();
    Piece* captured = board.removePiece(target);
    board.placePiece(p, target);

    if (captured != nullptr) {
        piecesOnTheBoard.erase(std::remove_if(piecesOnTheBoard.begin(), piecesOnTheBoard.end(),
                                              [captured](const std::unique_ptr<Piece>& piece) { return piece.get() == captured; }),
                               piecesOnTheBoard.end());
        capturedPieces.push_back(std::unique_ptr<ChessPiece>(dynamic_cast<ChessPiece*>(captured)));
    }

    // Castling kingside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() + 2) {
        Position sourceT(source.getRow(), source.getColumn() + 3);
        Position targetT(source.getRow(), source.getColumn() + 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(sourceT));
        board.placePiece(rook, targetT);
        rook->increaseMoveCount();
    }

    // Castling queenside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() - 2) {
        Position sourceT(source.getRow(), source.getColumn() - 4);
        Position targetT(source.getRow(), source.getColumn() - 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(sourceT));
        board.placePiece(rook, targetT);
        rook->increaseMoveCount();
    }

    // En passant
    if (dynamic_cast<Pawn*>(p)) {
        if (source.getColumn() != target.getColumn() && captured == nullptr) {
            Position pawnPos;
            if (p->getColor() == Color::WHITE) {
                pawnPos = Position(target.getRow() + 1, target.getColumn());
            } else {
                pawnPos = Position(target.getRow() - 1, target.getColumn());
            }
            captured = board.removePiece(pawnPos);
            capturedPieces.push_back(std::unique_ptr<ChessPiece>(dynamic_cast<ChessPiece*>(captured)));
            piecesOnTheBoard.erase(std::remove_if(piecesOnTheBoard.begin(), piecesOnTheBoard.end(),
                                                  [captured](const std::unique_ptr<Piece>& piece) { return piece.get() == captured; }),
                                   piecesOnTheBoard.end());
        }
    }

    return captured;
}

void ChessMatch::undoMove(const Position& source, const Position& target, Piece* captured) {
    ChessPiece* p = dynamic_cast<ChessPiece*>(board.removePiece(target));
    p->decreaseMoveCount();
    board.placePiece(p, source);

    if (captured != nullptr) {
        board.placePiece(captured, target);
        capturedPieces.pop_back();
        piecesOnTheBoard.push_back(std::unique_ptr<ChessPiece>(dynamic_cast<ChessPiece*>(captured)));
    }

    // Castling kingside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() + 2) {
        Position sourceT(source.getRow(), source.getColumn() + 3);
        Position targetT(source.getRow(), source.getColumn() + 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(targetT));
        board.placePiece(rook, sourceT);
        rook->decreaseMoveCount();
    }

    // Castling queenside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() - 2) {
        Position sourceT(source.getRow(), source.getColumn() - 4);
        Position targetT(source.getRow(), source.getColumn() - 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(targetT));
        board.placePiece(rook, sourceT);
        rook->decreaseMoveCount();
    }

    // En passant
    if (dynamic_cast<Pawn*>(p)) {
        if (source.getColumn() != target.getColumn() && captured == enPassantVulnerable) {
            ChessPiece* pawn = dynamic_cast<ChessPiece*>(board.removePiece(target));
            Position pawnPos(p->getColor() == Color::WHITE ? 3 : 4, target.getColumn());
            board.placePiece(pawn, pawnPos);
        }
    }
}

void ChessMatch::validateSourcePosition(const Position& position) const {
    if (!board.thereIsAPiece(position)) {
        throw std::logic_error("Error moving piece! Source position empty!");
    }
    if (currentPlayer != dynamic_cast<ChessPiece*>(board.piece(position))->getColor()) {
        throw std::logic_error("Chosen piece is not yours!");
    }
    if (!dynamic_cast<ChessPiece*>(board.piece(position))->isThereAnyPossibleMove()) {
        throw std::logic_error("There is no possible move for chosen piece!");
    }
}

void ChessMatch::validateTargetPosition(const Position& from, const Position& to) const {
    if (!dynamic_cast<ChessPiece*>(board.piece(from))->possibleMove(to)) {
        throw std::logic_error("The chosen piece can't move to target position!");
    }
}

void ChessMatch::nextTurn() {
    turn++;
    currentPlayer = (currentPlayer == Color::WHITE ? Color.BLACK : Color.WHITE);
}

Color ChessMatch::opponent(Color color) const {
    return (color == Color.WHITE ? Color.BLACK : Color.WHITE);
}

ChessPiece* ChessMatch::king(Color color) const {
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* chessPiece = dynamic_cast<ChessPiece*>(piece.get());
        if (chessPiece->getColor() == color && dynamic_cast<King*>(chessPiece)) {
            return chessPiece;
        }
    }
    throw std::logic_error("There is no " + std::string(color == Color::WHITE ? "WHITE" : "BLACK") + " king on the board!");
}

bool ChessMatch::testCheck(Color color) const {
    Position kingPosition = king(color)->getChessPosition().toPosition();
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* chessPiece = dynamic_cast<ChessPiece*>(piece.get());
        if (chessPiece->getColor() != color) {
            std::vector<std::vector<bool>> mat = chessPiece->possibleMoves();
            if (mat[kingPosition.getRow()][kingPosition.getColumn()]) {
                return true;
            }
        }
    }
    return false;
}

bool ChessMatch::testCheckMate(Color color) const {
    if (!testCheck(color)) {
        return false;
    }
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* chessPiece = dynamic_cast<ChessPiece*>(piece.get());
        if (chessPiece->getColor() == color) {
            std::vector<std::vector<bool>> mat = chessPiece->possibleMoves();
            for (int i = 0; i < board.getRows(); ++i) {
                for (int j = 0; j < board.getColumns(); ++j) {
                    if (mat[i][j]) {
                        Position source = chessPiece->getChessPosition().toPosition();
                        Position target(i, j);
                        Piece* capturedPiece = makeMove(source, target);
                        bool testCheck = this->testCheck(color);
                        undoMove(source, target, capturedPiece);
                        if (!testCheck) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void ChessMatch::placeNewPiece(char column, int row, std::unique_ptr<ChessPiece> piece) {
    board.placePiece(piece.get(), ChessPosition(column, row).toPosition());
    piecesOnTheBoard.push_back(std::move(piece));
}

void ChessMatch::initialSetup() {
    // Rook
    placeNewPiece('a', 1, std::make_unique<Rook>(&board, Color::WHITE));
    placeNewPiece('h', 1, std::make_unique<Rook>(&board, Color::WHITE));
    placeNewPiece('a', 8, std::make_unique<Rook>(&board, Color::BLACK));
    placeNewPiece('h', 8, std::make_unique<Rook>(&board, Color::BLACK));

    // Knight
    placeNewPiece('b', 1, std::make_unique<Knight>(&board, Color::WHITE));
    placeNewPiece('g', 1, std::make_unique<Knight>(&board, Color::WHITE));
    placeNewPiece('b', 8, std::make_unique<Knight>(&board, Color::BLACK));
    placeNewPiece('g', 8, std::make_unique<Knight>(&board, Color::BLACK));

    // Bishop
    placeNewPiece('c', 1, std::make_unique<Bishop>(&board, Color::WHITE));
    placeNewPiece('f', 1, std::make_unique<Bishop>(&board, Color::WHITE));
    placeNewPiece('c', 8, std::make_unique<Bishop>(&board, Color::BLACK));
    placeNewPiece('f', 8, std::make_unique<Bishop>(&board, Color::BLACK));

    // King
    placeNewPiece('e', 1, std::make_unique<King>(&board, Color::WHITE, this));
    placeNewPiece('e', 8, std::make_unique<King>(&board, Color::BLACK, this));

    // Queen
    placeNewPiece('d', 1, std::make_unique<Queen>(&board, Color::WHITE));
    placeNewPiece('d', 8, std::make_unique<Queen>(&board, Color::BLACK));

    // Pawn
    for (int i = 0; i < 8; i++) {
        placeNewPiece('a' + i, 2, std::make_unique<Pawn>(&board, Color::WHITE, this));
        placeNewPiece('a' + i, 7, std::make_unique<Pawn>(&board, Color::BLACK, this));
    }
}