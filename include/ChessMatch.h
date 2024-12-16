#ifndef CHESSMATCH_H
#define CHESSMATCH_H

#include "Board.h"
#include "Color.h"
#include "ChessPiece.h"
#include "ChessPosition.h"
#include "ChessException.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"
#include <vector>
#include <memory>
#include <list>

class ChessMatch {
private:
    Board board;
    int turn;
    PieceColor currentPlayer;
    bool check;
    bool checkMate;
    ChessPiece* enPassantVulnerable;
    ChessPiece* promoted;
    int castling;
    bool enPassantCompleted;
    std::list<std::unique_ptr<Piece>> piecesOnTheBoard;
    std::list<std::unique_ptr<Piece>> capturedPieces;

    void initialSetup();
    void placeNewPiece(char column, int row, std::unique_ptr<ChessPiece> piece);

public:
    void validateSourcePosition(const Position& position) const;
    void validateTargetPosition(const Position& from, const Position& to) const;
    void nextTurn();
    PieceColor opponent(PieceColor color) const;
    ChessPiece* king(PieceColor color) const;
    bool testCheck(PieceColor color) const;
    bool testCheckMate(PieceColor color) const;
    Piece* makeMove(const Position& source, const Position& target);
    void undoMove(const Position& source, const Position& target, Piece* captured);
    void remove(Piece* piece); // Declaração da função remove

    ChessMatch();

    int getTurn() const;
    bool getCheck() const;
    bool getCheckMate() const;
    int getCastling() const { return castling;}
    bool getEnPassantCompleted() const { return enPassantCompleted;}
    PieceColor getCurrentPlayer() const;
    ChessPiece* getEnPassantVulnerable() const;
    ChessPiece* getPromoted() const;

    std::vector<std::vector<ChessPiece*>> getPieces() const;
    std::unique_ptr<ChessPiece> createNewPiece(const std::string& type, PieceColor color); // Renomeado para evitar conflito
    std::vector<std::vector<bool>> possibleMoves(const ChessPosition& sourcePosition) const;
    std::unique_ptr<ChessPiece> newPiece(const std::string& type, PieceColor color);
    ChessPiece* replacePromotedPiece(const std::string& type);
    ChessPiece* performChessMove(const ChessPosition& fromPosition, const ChessPosition& toPosition);
};

#endif // CHESSMATCH_H