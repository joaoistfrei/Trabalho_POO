#include "../include/UI.h"
#include "../include/ChessPiece.h"
#include "../include/ChessMatch.h"
#include "../include/ChessPosition.h"
#include "../include/Color.h"
#include "../include/ChessException.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>

using namespace std;

#define SQUARE_SIZE 100.0f
#define MARGIN_SIZE 100.0f
#define OUTER_BOX_SIZE 100.0f
#define NEWGAME_FONTSIZE 50
#define PAUSEBUTTON_SIZE 60

// Method to add piece to a list
void UI::addPiece(std::vector<std::pair<int, PieceImage*>>& pieceList, Pieces type, PieceColor color, float x, float y, int format) {
    Vector2 pos = {x, y};
    PieceImage* piece = new PieceImage(type, color, pos, format);
    pieceList.push_back(std::make_pair(type, piece));
}
// Constructor
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

// Destructor
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
// Draw method
void UI::Draw(std::vector<std::vector<bool>> possibleMoves, PieceColor player){
    Rectangle pauseButton = {MARGIN_SIZE + 8*SQUARE_SIZE + (MARGIN_SIZE - PAUSEBUTTON_SIZE)/2,
                             (MARGIN_SIZE - PAUSEBUTTON_SIZE)/2, PAUSEBUTTON_SIZE, PAUSEBUTTON_SIZE};
    // Draw chessboard and pieces on list
    DrawChessBoard(possibleMoves);
    for(auto it = _blackPieceList.begin(); it != _blackPieceList.end(); it++)
        it->second->Draw();
    for(auto it = _whitePieceList.begin(); it != _whitePieceList.end(); it++)
        it->second->Draw(); 
    for(auto it = _blackRemovedList.begin(); it != _blackRemovedList.end(); it++)
        it->second->Draw();
    for(auto it = _whiteRemovedList.begin(); it != _whiteRemovedList.end(); it++)
        it->second->Draw(); 
    // If there are elements on promotion list, draw selection screen
    if(not _promotionList.empty()){
        int width = GetScreenWidth();
        int height = GetScreenHeight();
        DrawRectangle(width/2 - 270, height/2 - 110, 540, 220, GRAY);
        DrawText("Choose your promotion piece:", width/2 - 270, height/2 - 110, 20, BLACK);
        for(auto it = _promotionList.begin(); it != _promotionList.end(); it++)
            it->second->Draw(); 
    }
    // Draw turn text
    if(player == PieceColor::W){
        int fontSize = 25;
        Color cian = {104,168,222,255};
        int textWidth = MeasureText("WHITE TURN", fontSize);
        DrawText("WHITE TURN", GetScreenWidth() - textWidth - MARGIN_SIZE, (MARGIN_SIZE - fontSize)/2, fontSize, cian);
    } else {
        int fontSize = 25;
        int textWidth = MeasureText("BLACK TURN", fontSize);
        DrawText("BLACK TURN", GetScreenWidth() - textWidth - MARGIN_SIZE, (MARGIN_SIZE - fontSize)/2, fontSize, BLACK);
    }
    // Draw pause button
    DrawRectangleRounded(pauseButton, 0.1, 16, DARKGRAY);
    for(int i = 0; i < 2; i ++){
        Vector2 startPos = {pauseButton.x + 3 * PAUSEBUTTON_SIZE/10 + 2*i*PAUSEBUTTON_SIZE/5, pauseButton.y + 5};
        Vector2 endPos = {pauseButton.x + 3 * PAUSEBUTTON_SIZE/10 + 2*i*PAUSEBUTTON_SIZE/5, pauseButton.y + PAUSEBUTTON_SIZE - 5};

        DrawLineEx(startPos, endPos, pauseButton.width/5, WHITE);
    }
}

// Method to draw checkmate message
void UI::DrawCheckMate(PieceColor player, bool& drawMenu){
    int fontSize = 50;
    int textWidth = MeasureText("WHITE WINS", fontSize);
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();
    int continueWidth = MeasureText("Click anywhere to continue!", 25);
    Color cian = {104,168,222,255};
    vector<vector<bool>> possibleMoves;
    // Checkmate draw loop
    while(!WindowShouldClose()){
        ClearBackground(GRAY);
        BeginDrawing();
        // Draw chessboard
        Draw(possibleMoves, player);
        if(player == PieceColor::W){
            DrawText("WHITE WINS", (windowWidth - textWidth)/2 , (windowHeight- fontSize)/2, fontSize, cian);
        } else
            DrawText("BLACK WINS", (windowWidth - textWidth)/2 , (windowHeight- fontSize)/2, fontSize, BLACK);

        DrawText("Click anywhere to continue!", (windowWidth - continueWidth)/2, (windowWidth + fontSize + 35)/2, 25, RED);
        if(IsMouseButtonPressed(0)){
            drawMenu = true;
            break;
        }
        EndDrawing();
    }
}
// Draw menu method
bool UI::DrawMenu(bool& isGameBeingPlayed, bool& drawMenu, PieceColor player){
    std::vector<std::vector<bool>> moves;
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Rectangle outerBox = {screenWidth/2 - 2*OUTER_BOX_SIZE , (screenHeight - 3*OUTER_BOX_SIZE)/2, 4*OUTER_BOX_SIZE, 3*OUTER_BOX_SIZE};
    int textWidth = MeasureText("CHESS", OUTER_BOX_SIZE);
    int newGameWidth = MeasureText("New Game", NEWGAME_FONTSIZE);
    Rectangle newgameBox = {static_cast<float>(screenWidth - newGameWidth - 20)/2, outerBox.y + outerBox.height - 110, 
                            static_cast<float>(20 + newGameWidth), static_cast<float>(20 + NEWGAME_FONTSIZE)};
    bool isInsideOuterBox;
    bool isInsideNewgame;
    // Menu Draw Loop
    while(!WindowShouldClose()){
        // Draw chessboard
        Draw(moves, player);
        // Draw Menu
        DrawRectangleRounded(outerBox, 0.1, 16, GRAY);
        DrawRectangleRoundedLines(outerBox, 0.1, 16, BLACK);
        DrawRectangleRounded(newgameBox, 0.1, 16, BEIGE);
        
        DrawText("CHESS", (screenWidth - textWidth)/2, (screenHeight - outerBox.y + 100)/2, 100, BLACK);
        DrawText("New Game", (screenWidth - newGameWidth)/2, outerBox.y + outerBox.height - 100, 50, BLACK);
        
        if(IsMouseButtonPressed(0)){
            Vector2 mousePos = GetMousePosition();
            isInsideOuterBox = CheckCollisionPointRec(mousePos, outerBox);
            if(isInsideOuterBox){
                isInsideNewgame = CheckCollisionPointRec(mousePos, newgameBox);
                if(isInsideNewgame)
                    DrawRectangleRoundedLines(newgameBox, 0.1, 16, BLACK);
            }
            break;
        }
        EndDrawing();
    }
    // If window was closed, throw exception
    if(WindowShouldClose()){
        EndDrawing();
        throw std::runtime_error("Window closed while waiting for user input.");
    }

    EndDrawing();
    if(isInsideNewgame){
        drawMenu = false;
        return true;
    }
    if(isInsideOuterBox){
        return false;
    } 
    if(isGameBeingPlayed)
        drawMenu = false;
    return false;
}
// Method to get position on chessboard clicked by user
ChessPosition UI::SelectPosition(std::vector<std::vector<bool>> possibleMoves, PieceColor player, bool& drawMenu){
    Rectangle pauseButton = {MARGIN_SIZE + 8*SQUARE_SIZE + (MARGIN_SIZE - PAUSEBUTTON_SIZE)/2,
                            (MARGIN_SIZE - PAUSEBUTTON_SIZE)/2, PAUSEBUTTON_SIZE, PAUSEBUTTON_SIZE};
    // Loop to draw while waiting for user entry
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);
        // Draw chessboard and pieces
        Draw(possibleMoves, player);

        // If mouse button pressed, verifies if pos is valid
        if (IsMouseButtonPressed(0)){
            Vector2 mousePos = GetMousePosition();

            if(CheckCollisionPointRec(mousePos, pauseButton)){
                drawMenu = true;
                EndDrawing();
                throw ChessException("Menu Opened!");
            }

            int column = static_cast<int>((mousePos.x - MARGIN_SIZE) / SQUARE_SIZE);
            int row = static_cast<int>((mousePos.y - MARGIN_SIZE) / SQUARE_SIZE);

            if (column >= 0 && column < 8 && row >= 0 && row < 8) {
                EndDrawing();
                return ChessPosition('a' + column, 8 - row); // Retorna posição válida
            } else {
                std::cerr << "Click outside the board boundaries. Try again." << std::endl;
            }
        }
        EndDrawing();
    }

    // If window was closed, throw exception
    throw std::runtime_error("Window closed while waiting for user input.");
}


// Method to find piece on board
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
// Method to move pieces on board
void UI::MovePiece(const ChessPosition& source, const ChessPosition& target, int isCastling) {
    PieceColor color;
    PieceImage* piece = findPiece(source, _blackPieceList);
    PieceImage* tower;
    if (!piece) {
        piece = findPiece(source, _whitePieceList);
        color = PieceColor::W;
    } else 
        color = PieceColor::B;
    // If a piece was found in source
    if (piece) {
        piece->MoveTo({static_cast<float>(target.getColumn() - 'a'), static_cast<float>(8 - target.getRow())});
        // if Is castling, we should move tower as well
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

// Method to remove Piece from a list
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
// Method to remove a piece from board
void UI::removePiece(ChessPiece* capturedPiece, bool enPassantCompleted, ChessPosition target) {
    PieceColor color = capturedPiece->getColor();
    bool isWhite = (color == PieceColor::W ? true : false);
    ChessPosition capturedPos = ChessPosition('a', 1);

    std::cout << "removePiece(): " << static_cast<int>(color) <<" (" << capturedPos.getColumn() << ", " << capturedPos.getRow() << ")" << std::endl;
    if(enPassantCompleted && not capturedPiece->getBoard()->positionExists(capturedPiece->getPosition())){
        capturedPos =  ChessPosition(target.getColumn(), target.getRow() + (capturedPiece->getColor() == PieceColor::W ? 1 : -1));
    } else 
        capturedPos = ChessPosition(capturedPiece->getPosition().getColumn() + 'a', 8 - capturedPiece->getPosition().getRow());
    // Chooses list from correct color
    if (isWhite) {
        removePieceFromList(capturedPos, _whitePieceList, color);
    } else {
        removePieceFromList(capturedPos, _blackPieceList, color);
    }
}

// Method to add piece to removed list
void UI::addToRemovedList(int p, PieceColor color){
    PieceImage* piece;
    Vector2 pos;
    // Verifies piece color
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
// Method to Draw Chessboard
void UI::DrawChessBoard(std::vector<std::vector<bool>> possibleMoves) {
    const int boardSize = 8;
    const float squareSize = SQUARE_SIZE; // Size of each square in pixels
    const float marginSize = MARGIN_SIZE;  // Margin around the board in pixels
    // Loop to draw board squares
    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            Color color = ((x + y) % 2 == 0) ? DARKGRAY : RAYWHITE;
            DrawRectangle(x * squareSize + marginSize, y * squareSize + marginSize, squareSize, squareSize, color);
        }
    }
    // Loop to draw possibleMoves highlight
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
// Method to fill promotion list in case we have to draw it
void UI::fillPromotionList(PieceColor color){
    int width = GetScreenWidth();
    int height = GetScreenHeight();
    addPiece(_promotionList, ROOK, color, width/2 - 270, height/2 - 90, 2);
    addPiece(_promotionList, KNIGHT, color, width/2 - 130, height/2 - 90, 2);
    addPiece(_promotionList, BISHOP, color, width/2 + 10, height/2 - 90, 2);
    addPiece(_promotionList, QUEEN, color, width/2 + 150, height/2 - 90, 2);
}
// Method to get promotion piece users choice
std::string UI::selectPromotionPiece(std::vector<std::vector<bool>> possibleMoves, PieceColor player){
    // Method loop
    while (!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(GRAY);
        // Draw chessboard and pieces
        this->Draw(possibleMoves, player);

        EndDrawing();
        // Verifies Users mouse button click
        if (IsMouseButtonPressed(0)){ 
            int width = GetScreenWidth();
            int height = GetScreenHeight();
            Vector2 mousePos = GetMousePosition();
            std::cout << "mousePos: (" << mousePos.x << ", " << mousePos.y << ")" <<std::endl;
            int column = static_cast<int>((mousePos.x - (width/2 - 270))/135);
            int row = static_cast<int>((mousePos.y -(height/2 - 90))/200);
            // Return chosen piece
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

    // If window was closed, throw exception
    throw std::runtime_error("Window closed while waiting for user input.");
}
// Method to replace promoted piece
void UI::replacePromotedPiece(ChessPosition pos, PieceColor color, Pieces type){
    // Verifies piece color, remove from piece list, then add new piece to list
    if (color == PieceColor::W) {
        removePieceFromList(pos, _whitePieceList, color);
        Vector2 boardPos = {MARGIN_SIZE + (pos.getColumn()- 'a') * SQUARE_SIZE, MARGIN_SIZE + (8 - pos.getRow())*SQUARE_SIZE};
        addPiece(_whitePieceList, type, color, boardPos.x, boardPos.y);
    } else {
        removePieceFromList(pos, _blackPieceList, color);
        Vector2 boardPos = {MARGIN_SIZE + (pos.getColumn()- 'a') * SQUARE_SIZE, MARGIN_SIZE + (8 - pos.getRow())*SQUARE_SIZE};
        addPiece(_blackPieceList, type, color, boardPos.x, boardPos.y);
    }
    // Remove promotion list elements and textures
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
    // Switch case to select correct image based on color and piece
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
    // Choose format based on if it is in play, if it is a promotion piece or if it is a removed list
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
// Destructor
PieceImage::~PieceImage(){
    // UnloadTexture(image);
}
// Draw method
void PieceImage::Draw(){
    DrawTextureV(image, position, WHITE);
    // DrawTextureEx(image, position, 0, 0.4, WHITE);
}

// Method to get piece position on board
Vector2 PieceImage::getPosition(){
    Vector2 pos;
    pos.x = position.x;
    pos.y = position.y;
    return pos;
}
// Method to move PieceImage on board
void PieceImage::MoveTo(Vector2 targetPos){
    position.x = MARGIN_SIZE + targetPos.x*SQUARE_SIZE + (SQUARE_SIZE - image.width)/2;
    position.y = MARGIN_SIZE+ targetPos.y*SQUARE_SIZE;
}

