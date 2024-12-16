#ifndef UI_H
#define UI_H

#include <iostream>
#include <vector>
#include <string>
#include "ChessMatch.h"
#include "ChessPiece.h"
#include "ChessPosition.h"

#include <raylib.h>

// class UI {
// public:
//     static void clearScreen();
//     static void printMatch(const ChessMatch* chessMatch, const std::vector<ChessPiece*>& captured);
//     static void printBoard(const std::vector<std::vector<ChessPiece*>>& pieces, const std::vector<std::vector<bool>>& possibleMoves);
//     static ChessPosition readChessPosition();
// };

// extern const std::string ANSI_CYAN;
// extern const std::string ANSI_RESET;
// extern const std::string ANSI_BLUE_BACKGROUND;
// extern const std::string ANSI_WHITE;
// extern const std::string ANSI_YELLOW;

// void printPiece(const ChessPiece* piece, bool background);
// void printCapturedPieces(const std::vector<ChessPiece*>& captured);

enum Pieces{
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    QUEEN,
    KING
};


class PieceImage{
    public:
        PieceImage(int piece = 0, PieceColor color = PieceColor::B, Vector2 pos = {0,0}, int format = 0);
        ~PieceImage();
        void Draw();
        void MoveTo(Vector2 targetPos);
        Vector2 getPosition();
        int getImageWidth(){ return image.width; }
    
    protected:
        Texture2D image;
        Vector2 position;
    
};

class UI{
    public:
        UI();
        ~UI();
        void addPiece(std::vector<std::pair<int, PieceImage*>>& pieceList, Pieces type, PieceColor color, float x, float y, int format = 0);
        void Draw(std::vector<std::vector<bool>> possibleMoves);
        void Update();
        ChessPosition SelectPosition(std::vector<std::vector<bool>> possibleMoves);
        void MovePiece(const ChessPosition& source, const ChessPosition& target, int isCastling);
        PieceImage* findPiece(const ChessPosition& position, std::vector<std::pair<int, PieceImage*>>& pieceList);
        int PieceMouseSelect(Vector2 mousePos);
        std::string getSelectedPosString();
        void removePiece(ChessPiece* capturedPiece);
        void removePieceFromList(const ChessPosition& position, std::vector<std::pair<int, PieceImage*>>& pieceList, PieceColor color);
        void addToRemovedList(int p, PieceColor color);

        void fillPromotionList(PieceColor color);
        std::string selectPromotionPiece(std::vector<std::vector<bool>> possibleMoves);
        void replacePromotedPiece(ChessPosition pos, PieceColor color, Pieces type);
        Pieces toPieces(char piece);

        //void StartScreen();
    private:
        void DrawChessBoard(std::vector<std::vector<bool>> possibleMoves);
        std::vector<std::pair<int, PieceImage*>> _whitePieceList;
        std::vector<std::pair<int, PieceImage*>> _blackPieceList;
        std::vector<std::pair<int, PieceImage*>> _whiteRemovedList;
        std::vector<std::pair<int, PieceImage*>> _blackRemovedList;
        std::vector<std::pair<int, PieceImage*>> _promotionList;
        int selectedPiece;
        PieceImage pawn;
};


#endif // UI_H