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
        PieceImage(int piece = 0, int color = 0, Vector2 pos = {0,0}, bool removed = false);
        ~PieceImage();
        void Draw();
        void MoveTo(Vector2 mousePos);
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
        void addPiece(std::vector<std::pair<int, PieceImage*>>& pieceList, int type, int color, float x, float y);
        void Draw();
        void Update();
        ChessPosition SelectPosition();
        void MovePiece(const ChessPosition& source, const ChessPosition& target);
        PieceImage* findPiece(const ChessPosition& position, std::vector<std::pair<int, PieceImage*>>& pieceList);
        int PieceMouseSelect(Vector2 mousePos);
        // int GetSelectedPiece(){ return selectedPiece; }
        std::string getSelectedPosString();
        void setTurn(bool t){ turn = t; }
        void removePiece(ChessPiece* capturedPiece);
        void addToRemovedList(int p, bool color);

        //void StartScreen();

        const Vector2 invalidPos = {-1, -1};
    private:
        void DrawChessBoard();
        bool turn; // 0 = black, 1 = white
        std::vector<std::pair<int, PieceImage*>> _whitePieceList;
        std::vector<std::pair<int, PieceImage*>> _blackPieceList;
        std::vector<std::pair<int, PieceImage*>> _whiteRemovedList;
        std::vector<std::pair<int, PieceImage*>> _blackRemovedList;
        int selectedPiece;
        PieceImage pawn;
};


#endif // UI_H