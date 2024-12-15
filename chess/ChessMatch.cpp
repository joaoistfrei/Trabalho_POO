#include "../include/ChessMatch.h"
#include "../include/ChessPiece.h"
#include "../include/Board.h"
#include "../include/Position.h"
#include "../include/ChessPosition.h"
#include "../include/Rook.h"
#include "../include/Knight.h"
#include "../include/Bishop.h"
#include "../include/King.h"
#include "../include/Queen.h"
#include "../include/Pawn.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

ChessMatch::ChessMatch() : board(8, 8), turn(1), currentPlayer(PieceColor::W), check(false), checkMate(false), enPassantVulnerable(nullptr), promoted(nullptr) {
    initialSetup();
}

void ChessMatch::remove(Piece* piece) {
    std::cout << "Removendo peça" << piece << std::endl;
    piecesOnTheBoard.remove_if([piece](const std::unique_ptr<Piece>& p) { return p.get() == piece; });
    std::cout << "Removida peça" << piece << std::endl;
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

PieceColor ChessMatch::getCurrentPlayer() const {
    return currentPlayer;
}

ChessPiece* ChessMatch::getEnPassantVulnerable() const {
    return enPassantVulnerable;
}

ChessPiece* ChessMatch::getPromoted() const {
    return promoted;
}

std::vector<std::vector<ChessPiece*>> ChessMatch::getPieces() const {
    std::vector<std::vector<ChessPiece*>> mat(board.getRows(), std::vector<ChessPiece*>(board.getColumns(), nullptr));
    for (int i = 0; i < board.getRows(); ++i) {
        for (int j = 0; j < board.getColumns(); ++j) {
            mat[i][j] = dynamic_cast<ChessPiece*>(board.piece(i, j));
        }
    }
    return mat;
}

std::vector<std::vector<bool>> ChessMatch::possibleMoves(const ChessPosition& sourcePosition) const {
    Position position = sourcePosition.toPosition();
    validateSourcePosition(position);
    //std::cout << "TESTE" << std::endl;
    return board.piece(position)->possibleMoves();
}






ChessPiece* ChessMatch::performChessMove(const ChessPosition& fromPosition, const ChessPosition& toPosition) {
    Position from = fromPosition.toPosition();
    Position to = toPosition.toPosition();
    validateSourcePosition(from);
    validateTargetPosition(from, to);
    ChessPiece* captured = dynamic_cast<ChessPiece*>(board.piece(to));;
    Piece* capturedPiece = makeMove(from, to);
    std::cout << "1Captured(Piece) piece type: " << capturedPiece << std::endl;
    std::cout << "1Captured(ChessPiece) piece type: " << captured << std::endl;


    if (testCheck(currentPlayer)) {
        undoMove(from, to, capturedPiece);
        throw ChessException("You can't place yourself in check!");
    }

    ChessPiece* movedPiece = dynamic_cast<ChessPiece*>(board.piece(to));

    // Promotion
    promoted = nullptr;
    if (dynamic_cast<Pawn*>(movedPiece)) {
        if ((movedPiece->getColor() == PieceColor::W && to.getRow() == 0) || (movedPiece->getColor() == PieceColor::W && to.getRow() == 7)) {
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
    /*
    if (capturedPiece != nullptr) {
    std::cout << "AQUI???????" << std::endl;
        if (captured == nullptr) {
            throw std::logic_error("Captured piece is not a ChessPiece");
        }
        std::cout << "De fato entrou" << std::endl;
        board.removePiece(capturedPiece->getPosition());
        std::cout << "Captured piece type: " << typeid(*capturedPiece).name() << std::endl;
        // Remover a peça capturada da lista de peças no tabuleiro
        // Adicionar a peça capturada à lista de peças capturadas
        remove(captured);
        capturedPieces.push_back(std::unique_ptr<ChessPiece>(captured));
    }
    */

    return captured;
}

ChessPiece* ChessMatch::replacePromotedPiece(const std::string& type) {
    if (promoted == nullptr) {
        throw std::logic_error("There is no piece to be promoted!");
    }
    if (type != "B" && type != "G" && type != "R" && type != "Q") {
        return promoted;
    }

    Position pos = promoted->getChessPosition().toPosition();
    Piece* p = board.removePiece(pos);
    remove(p);

    std::unique_ptr<ChessPiece> newPiece = createNewPiece(type, promoted->getColor());
    board.placePiece(newPiece.get(), pos);
    piecesOnTheBoard.push_back(std::move(newPiece));

    return promoted;
}

std::unique_ptr<ChessPiece> ChessMatch::createNewPiece(const std::string& type, PieceColor color) {
    if (type == "B") return std::make_unique<Bishop>(&board, color);
    if (type == "N") return std::make_unique<Knight>(&board, color);
    if (type == "R") return std::make_unique<Rook>(&board, color);
    return std::make_unique<Queen>(&board, color);
}

std::unique_ptr<ChessPiece> ChessMatch::newPiece(const std::string& type, PieceColor color) {
    if (type == "B") return std::make_unique<Bishop>(&board, color);
    if (type == "G") return std::make_unique<Knight>(&board, color);
    if (type == "R") return std::make_unique<Rook>(&board, color);
    return std::make_unique<Queen>(&board, color);
}









Piece* ChessMatch::makeMove(const Position& source, const Position& target) {
    ChessPiece* p = dynamic_cast<ChessPiece*>(board.removePiece(source));
    p->increaseMoveCount();
    Piece* capturedPiece = board.removePiece(target);
    std::cout << "Captured piece type: " << capturedPiece << std::endl;
    board.placePiece(p, target);
    /*
    if (capturedPiece != nullptr) {
        //std::cout << "Moved piece type: " << capturedPiece << std::endl;
        //remove(capturedPiece);
        //std::cout << "Captured piece type: " << capturedPiece << std::endl;
        //capturedPieces.push_back(std::unique_ptr<Piece>(capturedPiece));
    }
    */

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
        if (source.getColumn() != target.getColumn() && capturedPiece == nullptr) {
            Position pawnPos;
            if (p->getColor() == PieceColor::W) {
                pawnPos = Position(target.getRow() + 1, target.getColumn());
            } else {
                pawnPos = Position(target.getRow() - 1, target.getColumn());
            }
            capturedPiece = board.removePiece(pawnPos);
            ChessPiece* captured = dynamic_cast<ChessPiece*>(capturedPiece);
            if (captured != nullptr) {
                capturedPieces.push_back(std::unique_ptr<ChessPiece>(captured));
                remove(captured);
            }
        }
    }

    return capturedPiece;
}





void ChessMatch::undoMove(const Position& source, const Position& target, Piece* captured) {
    ChessPiece* p = dynamic_cast<ChessPiece*>(board.removePiece(target));
    p->decreaseMoveCount();
    board.placePiece(p, source);

    if (captured != nullptr) {
        board.placePiece(captured, target);
        capturedPieces.erase(std::remove_if(capturedPieces.begin(), capturedPieces.end(),
                                            [captured](const std::unique_ptr<Piece>& piece) { return piece.get() == captured; }),
                             capturedPieces.end());
        piecesOnTheBoard.push_back(std::unique_ptr<Piece>(captured));
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
            Position pawnPosition;
            if (p->getColor() == PieceColor::W) {
                pawnPosition = Position(3, target.getColumn());
            } else {
                pawnPosition = Position(4, target.getColumn());
            }
            board.placePiece(pawn, pawnPosition);
        }
    }
}

void ChessMatch::validateSourcePosition(const Position& position) const {
    std::cout << "TESTE1" << std::endl;
    if (!board.thereIsAPiece(position)) {
        throw ChessException("Error moving piece! Source position empty!");
    }
        std::cout << "TESTE2" << std::endl;
    if (currentPlayer != dynamic_cast<ChessPiece*>(board.piece(position))->getColor()) {
        throw ChessException("Chosen piece is not yours!");
    }
        std::cout << "TESTE3" << std::endl;

    if (!board.piece(position)->isThereAnyPossibleMove()) {
        throw ChessException("There is no possible move for chosen piece!");
    }
        std::cout << "TESTE4" << std::endl;

}

void ChessMatch::validateTargetPosition(const Position& from, const Position& to) const {
    if (!board.piece(from)->possibleMove(to)) {
        throw ChessException("The chosen piece can't move to target position!");
    }
}

void ChessMatch::nextTurn() {
    turn++;
    currentPlayer = (currentPlayer == PieceColor::W ? PieceColor::W : PieceColor::W);
}

PieceColor ChessMatch::opponent(PieceColor color) const {
    return (color == PieceColor::W ? PieceColor::W : PieceColor::W);
}

ChessPiece* ChessMatch::king(PieceColor color) const {
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* chessPiece = dynamic_cast<ChessPiece*>(piece.get());
        if (chessPiece->getColor() == color && dynamic_cast<King*>(chessPiece)) {
            std::cout << "TESTE REY" << std::endl;
            return chessPiece;
        }
    }
    throw std::logic_error("There is no " + std::to_string(static_cast<int>(color)) + " king on the board!");
}

bool ChessMatch::testCheck(PieceColor color) const {
    Position kingPosition = king(color)->getChessPosition().toPosition();
    std::cout << "TESTE CHECKKKKK" << std::endl;
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* opponentPiece = dynamic_cast<ChessPiece*>(piece.get());
        if (opponentPiece->getColor() == opponent(color)) {
            std::vector<std::vector<bool>> mat = opponentPiece->possibleMoves();
            if (mat[kingPosition.getRow()][kingPosition.getColumn()]) {
                return true;
            }
        }
    }
    return false;
}

bool ChessMatch::testCheckMate(PieceColor color) const {
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
                         Piece* capturedPiece = const_cast<ChessMatch*>(this)->makeMove(source, target);
                        bool testCheck = this->testCheck(color);
                        const_cast<ChessMatch*>(this)->undoMove(source, target, capturedPiece);
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
    placeNewPiece('a', 1, std::make_unique<Rook>(&board, PieceColor::W));
    placeNewPiece('h', 1, std::make_unique<Rook>(&board, PieceColor::W));
    placeNewPiece('a', 8, std::make_unique<Rook>(&board, PieceColor::B));
    placeNewPiece('h', 8, std::make_unique<Rook>(&board, PieceColor::B));

    // Knight
    placeNewPiece('b', 1, std::make_unique<Knight>(&board, PieceColor::W));
    placeNewPiece('g', 1, std::make_unique<Knight>(&board, PieceColor::W));
    placeNewPiece('b', 8, std::make_unique<Knight>(&board, PieceColor::B));
    placeNewPiece('g', 8, std::make_unique<Knight>(&board, PieceColor::B));

    // Bishop
    placeNewPiece('c', 1, std::make_unique<Bishop>(&board, PieceColor::W));
    placeNewPiece('f', 1, std::make_unique<Bishop>(&board, PieceColor::W));
    placeNewPiece('c', 8, std::make_unique<Bishop>(&board, PieceColor::B));
    placeNewPiece('f', 8, std::make_unique<Bishop>(&board, PieceColor::B));

    // King
    placeNewPiece('e', 1, std::make_unique<King>(&board, PieceColor::W, this));
    placeNewPiece('e', 8, std::make_unique<King>(&board, PieceColor::B, this));

    // Queen
    placeNewPiece('d', 1, std::make_unique<Queen>(&board, PieceColor::W));
    placeNewPiece('d', 8, std::make_unique<Queen>(&board, PieceColor::B));

    // Pawn
    for (int i = 0; i < board.getColumns(); ++i) {
        placeNewPiece('a' + i, 2, std::make_unique<Pawn>(&board, PieceColor::W, this));
        placeNewPiece('a' + i, 7, std::make_unique<Pawn>(&board, PieceColor::B, this));
    }
}