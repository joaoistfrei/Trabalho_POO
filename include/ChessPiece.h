#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include "Piece.h"
#include "Board.h"
#include "Position.h"
#include "ChessPosition.h"
#include "Color.h"

class ChessPiece : public Piece {
private:
    PieceColor color;
    int moveCount;

public:
    ChessPiece(Board* board, PieceColor color);
    virtual ~ChessPiece() = default; // Destrutor virtual default


    PieceColor getColor() const;
    int getMoveCount() const;
    void increaseMoveCount();
    void decreaseMoveCount();
    ChessPosition getChessPosition() const;
    
    virtual std::string toString() const = 0; // Adicionado o método toString

    static ChessPiece* toChessPiece(Piece* piece);

protected:
    bool isThereOpponentPiece(Position position) const;
};

#endif // CHESSPIECE_H