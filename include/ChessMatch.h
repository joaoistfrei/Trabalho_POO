#ifndef CHESSMATCH_H
#define CHESSMATCH_H

#include "Board.h"
#include "Color.h"
#include "ChessPiece.h"
#include <vector>
#include <memory>

class ChessMatch {
private:
    Board board;
    int turn;
    Color currentPlayer;
    bool check;
    bool checkMate;
    ChessPiece* enPassantVulnerable;
    ChessPiece* promoted;
    std::vector<std::unique_ptr<ChessPiece>> piecesOnTheBoard;
    std::vector<std::unique_ptr<ChessPiece>> capturedPieces;

    void initialSetup();
    void placeNewPiece(char column, int row, std::unique_ptr<ChessPiece> piece);

public:
    ChessMatch();

    int getTurn() const;
    bool getCheck() const;
    bool getCheckMate() const;
    Color getCurrentPlayer() const;
    ChessPiece* getEnPassantVulnerable() const;
    ChessPiece* getPromoted() const;

    ChessPiece* replacePromotedPiece(const std::string& type);
    std::unique_ptr<ChessPiece> createNewPiece(const std::string& type, Color color);
    ChessPiece* performChessMove(const ChessPosition& fromPosition, const ChessPosition& toPosition);
    std::vector<std::vector<bool>> possibleMoves(const ChessPosition& source) const;

private:
    Piece makeMove(const Position& source, const Position& target);
    void undoMove(const Position& source, const Position& target, Piece captured);
    void validateSourcePosition(const Position& position) const;
    void validateTargetPosition(const Position& from, const Position& to) const;
    void nextTurn();
    Color opponent(Color color) const;
    ChessPiece king(Color color) const;
    bool testCheck(Color color) const;
    bool testCheckMate(Color color) const;
};

#endif // CHESSMATCH_H