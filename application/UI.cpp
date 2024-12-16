#include "../include/UI.h"
#include "../include/ChessPiece.h"
#include "../include/ChessMatch.h"
#include "../include/ChessPosition.h"
#include "../include/Color.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

#define SQUARE_SIZE 100.0f
#define MARGIN_SIZE 100.0f


void UI::addPiece(std::vector<std::pair<int, PieceImage*>>& pieceList, Pieces type, PieceColor color, float x, float y, int format) {
    Vector2 pos = {x, y};
    PieceImage* piece = new PieceImage(type, color, pos, format);
    pieceList.push_back(std::make_pair(type, piece));
}

UI::UI() {
    float yPosBlack = MARGIN_SIZE + SQUARE_SIZE;
    float yPosWhite = MARGIN_SIZE + 6 * SQUARE_SIZE;

    // ADDING PAWNS
    for (int i = 0; i < 8; i++) {
        float xPos = MARGIN_SIZE + i * SQUARE_SIZE;
        addPiece(_blackPieceList, PAWN, PieceColor::B, xPos, yPosBlack);
        addPiece(_whitePieceList, PAWN, PieceColor::W, xPos, yPosWhite);
    }

    // ADDING OTHER PIECES
    PieceColor colors[] = {PieceColor::B, PieceColor::W}; // 0 -> Preto, 1 -> Branco
    for (PieceColor color : colors) {
        float yPos = MARGIN_SIZE + 7 * static_cast<int>(color) * SQUARE_SIZE;
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), ROOK, color, MARGIN_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), ROOK, color, MARGIN_SIZE + 7 * SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), KNIGHT, color, MARGIN_SIZE + SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), KNIGHT, color, MARGIN_SIZE + 6 * SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), BISHOP, color, MARGIN_SIZE + 2 * SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), BISHOP, color, MARGIN_SIZE + 5 * SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), QUEEN, color, MARGIN_SIZE + 3  * SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), KING, color, MARGIN_SIZE + 4 * SQUARE_SIZE, yPos);
    }
}


UI::~UI(){
    for(auto pieces : _blackPieceList)
        delete pieces.second;
    for(auto pieces : _whitePieceList)
        delete pieces.second;
    for(auto pieces : _whiteRemovedList)
        delete pieces.second;
    for(auto pieces : _blackRemovedList)
        delete pieces.second;
    for(auto pieces : _promotionList)
        delete pieces.second;
}

void UI::Draw(std::vector<std::vector<bool>> possibleMoves){
    DrawChessBoard(possibleMoves);
    for(auto it = _blackPieceList.begin(); it != _blackPieceList.end(); it++)
        it->second->Draw();
    for(auto it = _whitePieceList.begin(); it != _whitePieceList.end(); it++)
        it->second->Draw(); 
    for(auto it = _blackRemovedList.begin(); it != _blackRemovedList.end(); it++)
        it->second->Draw();
    for(auto it = _whiteRemovedList.begin(); it != _whiteRemovedList.end(); it++)
        it->second->Draw(); 
    if(not _promotionList.empty()){
        int width = GetScreenWidth();
        int height = GetScreenHeight();
        DrawRectangle(width/2 - 270, height/2 - 110, 540, 220, GRAY);
        DrawText("Choose your promotion piece:", width/2 - 270, height/2 - 110, 20, BLACK);
        for(auto it = _promotionList.begin(); it != _promotionList.end(); it++)
            it->second->Draw(); 
    }
}

void UI::Update(){
}

ChessPosition UI::SelectPosition(std::vector<std::vector<bool>> possibleMoves){
    // Color grey = {29, 29, 30, 255};
    while (!WindowShouldClose()){ // Loop bloqueante, mas não fecha a janela
        BeginDrawing();
        ClearBackground(GRAY);

        this->Draw(possibleMoves);

        EndDrawing();

        if (IsMouseButtonPressed(0)){ // Verifica se houve clique
            Vector2 mousePos = GetMousePosition();

            int column = static_cast<int>((mousePos.x - MARGIN_SIZE) / SQUARE_SIZE);
            int row = static_cast<int>((mousePos.y - MARGIN_SIZE) / SQUARE_SIZE);

            if (column >= 0 && column < 8 && row >= 0 && row < 8) {
                return ChessPosition('a' + column, 8 - row); // Retorna posição válida
            } else {
                std::cerr << "Click outside the board boundaries. Try again." << std::endl;
            }
        }
    }

    // Se a janela for fechada, lançamos uma exceção ou retornamos uma posição especial
    throw std::runtime_error("Window closed while waiting for user input.");
}



PieceImage* UI::findPiece(const ChessPosition& position, std::vector<std::pair<int, PieceImage*>>& pieceList){
    for (auto& piece : pieceList) {
        int pieceColumn = static_cast<int>((piece.second->getPosition().x - MARGIN_SIZE)  / SQUARE_SIZE);
        int pieceRow = static_cast<int>((piece.second->getPosition().y - MARGIN_SIZE) / SQUARE_SIZE);

        ChessPosition piecePos(pieceColumn + 'a', 8 - pieceRow);
        if (piecePos.getColumn() == position.getColumn() && piecePos.getRow() == position.getRow()){
            return piece.second;
        }
    }
    return nullptr;
}

void UI::MovePiece(const ChessPosition& source, const ChessPosition& target, int isCastling) {
    PieceColor color;
    PieceImage* piece = findPiece(source, _blackPieceList);
    PieceImage* tower;
    if (!piece) {
        piece = findPiece(source, _whitePieceList);
        color = PieceColor::W;
    } else 
        color = PieceColor::B;

    if (piece) {
        piece->MoveTo({static_cast<float>(target.getColumn() - 'a'), static_cast<float>(8 - target.getRow())});
        if(isCastling == 1){
            if(color == PieceColor::W){
                tower = findPiece(ChessPosition('h', 1), _whitePieceList);
                tower->MoveTo({static_cast<float>(target.getColumn() - 'a' - 1), static_cast<float>(8 - target.getRow())});
            } else {
                tower = findPiece(ChessPosition('h', 8), _blackPieceList);
                tower->MoveTo({static_cast<float>(target.getColumn() - 'a' - 1), static_cast<float>(8 - target.getRow())});
            }
        }
        else if(isCastling == 2){
            if(color == PieceColor::W){
                tower = findPiece(ChessPosition('a', 1), _whitePieceList);
                tower->MoveTo({static_cast<float>(target.getColumn() - 'a' + 1), static_cast<float>(8 - target.getRow())});
            } else {
                tower = findPiece(ChessPosition('a', 8), _blackPieceList);
                tower->MoveTo({static_cast<float>(target.getColumn() - 'a' + 1), static_cast<float>(8 - target.getRow())});
            }
        }
    }
}


void UI::removePieceFromList(const ChessPosition& position, std::vector<std::pair<int, PieceImage*>>& pieceList, PieceColor color){
    for (auto it = pieceList.begin(); it != pieceList.end(); it++) {
        int pieceColumn = static_cast<int>((it->second->getPosition().x - MARGIN_SIZE)  / SQUARE_SIZE);
        int pieceRow = static_cast<int>((it->second->getPosition().y - MARGIN_SIZE) / SQUARE_SIZE);

        ChessPosition piecePos(pieceColumn + 'a', 8 - pieceRow);
        if (piecePos.getColumn() == position.getColumn() && piecePos.getRow() == position.getRow()) {
            addToRemovedList(it->first, color);
            delete it->second;
            pieceList.erase(it);
            break;
        }
    }
}

void UI::removePiece(ChessPiece* capturedPiece) {
    PieceColor color = capturedPiece->getColor();
    bool isWhite = color == PieceColor::W ? true : false;
    ChessPosition capturedPos = ChessPosition(capturedPiece->getPosition().getColumn() + 'a', 8 - capturedPiece->getPosition().getRow());

    std::cout << "removePiece(): " << static_cast<int>(color) <<" (" << capturedPos.getColumn() << ", " << capturedPos.getRow() << ")" << std::endl;

    if (isWhite) {
        removePieceFromList(capturedPos, _whitePieceList, color);
    } else {
        removePieceFromList(capturedPos, _blackPieceList, color);
    }
}


void UI::addToRemovedList(int p, PieceColor color){
    PieceImage* piece;
    Vector2 pos;
    if(color == PieceColor::W){
        pos.x = MARGIN_SIZE + (_whiteRemovedList.size() + 1) * 30;// 30 is image width
        pos.y = MARGIN_SIZE/2;
        piece = new PieceImage(p, color, pos, true);
        _whiteRemovedList.push_back(std::make_pair(p, piece));
    } else {
        pos.x = MARGIN_SIZE + (_blackRemovedList.size() + 1) * 30; // 30 is image width
        pos.y = MARGIN_SIZE + 8*SQUARE_SIZE;
        piece = new PieceImage(p, color, pos, true);
        _blackRemovedList.push_back(std::make_pair(p, piece));
    }
}

void UI::DrawChessBoard(std::vector<std::vector<bool>> possibleMoves) {
    const int boardSize = 8;
    const float squareSize = SQUARE_SIZE; // Size of each square in pixels
    const float marginSize = MARGIN_SIZE;  // Margin around the board in pixels

    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            Color color = ((x + y) % 2 == 0) ? DARKGRAY : RAYWHITE;
            DrawRectangle(x * squareSize + marginSize, y * squareSize + marginSize, squareSize, squareSize, color);
        }
    }
    if(not possibleMoves.empty()){
        for (int x = 0; x < boardSize; x++) {
            for (int y = 0; y < boardSize; y++) {
                if (possibleMoves[x][y]) {
                Rectangle square = { y * squareSize + marginSize, x * squareSize + marginSize, squareSize, squareSize };
                DrawRectangleLinesEx(square, 10.0f, RED);
                }
            }
        }
    }
}

void UI::fillPromotionList(PieceColor color){
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    addPiece(_promotionList, ROOK, color, width/2 - 270, height/2 - 90, 2);
    addPiece(_promotionList, KNIGHT, color, width/2 - 130, height/2 - 90, 2);
    addPiece(_promotionList, BISHOP, color, width/2 + 10, height/2 - 90, 2);
    addPiece(_promotionList, QUEEN, color, width/2 + 150, height/2 - 90, 2);
}

std::string UI::selectPromotionPiece(std::vector<std::vector<bool>> possibleMoves){
    while (!WindowShouldClose()){ // Loop bloqueante, mas não fecha a janela
        BeginDrawing();
        ClearBackground(GRAY);

        this->Draw(possibleMoves);

        EndDrawing();

        if (IsMouseButtonPressed(0)){ // Verifica se houve clique
            int width = GetScreenWidth();
            int height = GetScreenHeight();
            Vector2 mousePos = GetMousePosition();
            std::cout << "mousePos: (" << mousePos.x << ", " << mousePos.y << ")" <<std::endl;
            int column = static_cast<int>((mousePos.x - (width/2 - 270))/135);
            int row = static_cast<int>((mousePos.y -(height/2 - 90))/200);

            if(column >= 0 && column < 4 && row == 0){
                if(column == 0)
                    return std::string("R");
                if(column == 1)
                    return std::string("N");
                if(column == 2)
                    return std::string("B");
                if(column == 3)
                    return std::string("Q");
            } else {
                std::cerr << "Click outside the board boundaries. Try again." << std::endl;
            }
        }
    }

    // Se a janela for fechada, lançamos uma exceção ou retornamos uma posição especial
    throw std::runtime_error("Window closed while waiting for user input.");
}

void UI::replacePromotedPiece(ChessPosition pos, PieceColor color, Pieces type){
    if (color == PieceColor::W) {
        removePieceFromList(pos, _whitePieceList, color);
        Vector2 boardPos = {MARGIN_SIZE + (pos.getColumn()- 'a') * SQUARE_SIZE, MARGIN_SIZE + (8 - pos.getRow())*SQUARE_SIZE};
        addPiece(_whitePieceList, type, color, boardPos.x, boardPos.y);

    } else {
        removePieceFromList(pos, _blackPieceList, color);
        Vector2 boardPos = {MARGIN_SIZE + (pos.getColumn()- 'a') * SQUARE_SIZE, MARGIN_SIZE + (8 - pos.getRow())*SQUARE_SIZE};
        addPiece(_whitePieceList, type, color, boardPos.x, boardPos.y);
    }
    for(auto it = _promotionList.begin(); it != _promotionList.end(); it++)
        delete it->second;
    _promotionList.clear();
}

Pieces UI::toPieces(char piece){
    if(piece == 'p' || piece == 'P')
        return PAWN;
    if(piece == 'r' || piece == 'R')
        return ROOK;
    if(piece == 'n' || piece == 'N')
        return KNIGHT;
    if(piece == 'b' || piece == 'B')
        return BISHOP;
    if(piece == 'q' || piece == 'Q')
        return QUEEN;
    if(piece == 'k' || piece == 'K')
        return KING;
    return PAWN;
}


PieceImage::PieceImage(int piece, PieceColor color, Vector2 pos, int format){
    bool white = color == PieceColor::W;
    Image img;
    switch (piece){
        case PAWN:{
            if(white)
                img = LoadImage("../Images/WhitePawn.png");
            else
                img = LoadImage("../Images/BlackPawn.png");            
        } break;
        case ROOK:{
            if(white)
                img = LoadImage("../Images/WhiteRook.png");
            else
                img = LoadImage("../Images/BlackRook.png");            
        } break;
        case KNIGHT:{
            if(white)
                img = LoadImage("../Images/WhiteKnight.png");
            else
                img = LoadImage("../Images/BlackKnight.png");            
        } break;
        case BISHOP:{
            if(white)
                img = LoadImage("../Images/WhiteBishop.png");
            else
                img = LoadImage("../Images/BlackBishop.png");            
        } break;
        case QUEEN:{
            if(white)
                img = LoadImage("../Images/WhiteQueen.png");
            else
                img = LoadImage("../Images/BlackQueen.png");            
        } break;
        case KING:{
            if(white)
                img = LoadImage("../Images/WhiteKing.png");
            else
                img = LoadImage("../Images/BlackKing.png");            
        } break;
        default:{
            img = LoadImage("../Images/Pawn.png"); 
        }break;
    }
    if(format == 0){
        ImageResize(&img, 60, 100);
        image = LoadTextureFromImage(img);
        position.x = pos.x + (SQUARE_SIZE - image.width)/2;
        position.y = pos.y;
    }
    else if(format == 1){
        ImageResize(&img, 30, 50);
        image = LoadTextureFromImage(img);
        position.x = pos.x;
        position.y = pos.y;
    } else {
        ImageResize(&img, 120, 200);
        image = LoadTextureFromImage(img);
        position.x = pos.x + (SQUARE_SIZE - image.width)/2;
        position.y = pos.y;
    }
    
}

PieceImage::~PieceImage(){
    // UnloadTexture(image);
}

void PieceImage::Draw(){
    DrawTextureV(image, position, WHITE);
    // DrawTextureEx(image, position, 0, 0.4, WHITE);
}


Vector2 PieceImage::getPosition(){
    Vector2 pos;
    pos.x = position.x;
    pos.y = position.y;
    return pos;
}

void PieceImage::MoveTo(Vector2 targetPos){
    position.x = MARGIN_SIZE + targetPos.x*SQUARE_SIZE + (SQUARE_SIZE - image.width)/2;
    position.y = MARGIN_SIZE+ targetPos.y*SQUARE_SIZE;
}

