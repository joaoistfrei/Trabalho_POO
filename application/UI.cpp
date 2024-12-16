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

// const string ANSI_CYAN = "\033[36m";
// const string ANSI_RESET = "\033[0m";
// const string ANSI_BLUE_BACKGROUND = "\033[44m";
// const string ANSI_WHITE = "\033[37m";
// const string ANSI_YELLOW = "\033[33m";

// void UI::clearScreen() {
//     cout << "\033[2J\033[H";
// }

// void UI::printMatch(const ChessMatch* chessMatch, const vector<ChessPiece*>& captured) {
//     printBoard(chessMatch->getPieces(), std::vector<std::vector<bool>>());
//     cout << endl;
//     printCapturedPieces(captured);
//     cout << endl;
//     cout << "Turn: " << chessMatch->getTurn() << endl;
//     if (!chessMatch->getCheckMate()) {
//         cout << "Waiting player: " << (chessMatch->getCurrentPlayer() == Color::WHITE ? "WHITE" : "BLACK") << endl;
//         if (chessMatch->getCheck()) {
//             cout << "CHECK!" << endl;
//         }
//     } else {
//         cout << "CHECKMATE!" << endl;
//         cout << "Winner: " << (chessMatch->getCurrentPlayer() == Color::WHITE ? "WHITE" : "BLACK") << endl;
//     }
// }

// void UI::printBoard(const vector<vector<ChessPiece*>>& pieces, const std::vector<std::vector<bool>>& possibleMoves) {
//     for (vector<ChessPiece*>::size_type i = 0; i < pieces.size(); i++) {
//         cout << 8 - i << " ";
//         for (vector<ChessPiece*>::size_type j = 0; j < pieces[i].size(); j++) {
//             if (possibleMoves.size() > 0 && possibleMoves[i][j]) {
//                 cout << ANSI_BLUE_BACKGROUND;
//             }
//             printPiece(pieces[i][j], possibleMoves.size() > 0 && possibleMoves[i][j]);
//             cout << ANSI_RESET << " ";
//         }
//         cout << endl;
//     }
//     cout << "  a b c d e f g h" << endl;
// }

// void printBoard(const vector<vector<ChessPiece*>>& pieces) {
//     for (vector<ChessPiece*>::size_type i = 0; i < pieces.size(); i++) {
//         cout << 8 - i << " ";
//         for (vector<ChessPiece*>::size_type j = 0; j < pieces[i].size(); j++) {
//             printPiece(pieces[i][j], false);
//             cout << " ";
//         }
//         cout << endl;
//     }
//     cout << "  a b c d e f g h" << endl;
// }

// ChessPosition UI::readChessPosition() {
//     string s;
//     cin >> s;
//     if (s.length() != 2) {
//         throw invalid_argument("Error reading ChessPosition. Valid values are from a1 to h8.");
//     }
//     char column = s[0];
//     int row = s[1] - '0';
//     if (column < 'a' || column > 'h' || row < 1 || row > 8) {
//         throw invalid_argument("Error reading ChessPosition. Valid values are from a1 to h8.");
//     }
//     //cout << ChessPosition(column, row).toString() << endl;
//     return ChessPosition(column, row);
// }

// void printPiece(const ChessPiece* piece, bool background) {
//     if (background) {
//         cout << ANSI_BLUE_BACKGROUND;
//     }
//     if (piece == nullptr) {
//         cout << "-" << ANSI_RESET;
//     } else {
//         if (piece->getColor() == Color::WHITE) {
//             cout << ANSI_WHITE << piece->toString() << ANSI_RESET;
//         } else {
//             cout << ANSI_YELLOW << piece->toString() << ANSI_RESET;
//         }
//     }
// }

// void printCapturedPieces(const vector<ChessPiece*>& captured) {
//     vector<ChessPiece*> white;
//     vector<ChessPiece*> black;
//     for (ChessPiece* piece : captured) {
//         if (piece->getColor() == Color::WHITE) {
//             white.push_back(piece);
//         } else {
//             black.push_back(piece);
//         }
//     }
//     cout << "Captured pieces:" << endl;
//     cout << "White: ";
//     for (ChessPiece* piece : white) {
//         cout << piece->toString() << " ";
//     }
//     cout << endl;
//     cout << "Black: ";
//     for (ChessPiece* piece : black) {
//         cout << piece->toString() << " ";
//     }
//     cout << endl;
// }

void UI::addPiece(std::vector<std::pair<int, PieceImage*>>& pieceList, int type, PieceColor color, float x, float y) {
    Vector2 pos = {x, y};
    PieceImage* piece = new PieceImage(type, color, pos);
    pieceList.push_back(std::make_pair(type, piece));
}

UI::UI() {
    turn = true;
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
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), QUEEN, color, MARGIN_SIZE + (3 + static_cast<int>(color)) * SQUARE_SIZE, yPos);
        addPiece((color == PieceColor::B ? _blackPieceList : _whitePieceList), KING, color, MARGIN_SIZE + (4 - static_cast<int>(color)) * SQUARE_SIZE, yPos);
    }
}


UI::~UI(){
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
}

void UI::Update(){
}

ChessPosition UI::SelectPosition(std::vector<std::vector<bool>> possibleMoves) {
    Color grey = {29, 29, 30, 255};
    while (!WindowShouldClose()) { // Loop bloqueante, mas não fecha a janela
        BeginDrawing();
        ClearBackground(grey);

        this->Draw(possibleMoves);
        // DrawText("Click on a square to select a position", 10, 10, 20, RAYWHITE);

        EndDrawing();

        if (IsMouseButtonPressed(0)) { // Verifica se houve clique
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

void UI::MovePiece(const ChessPosition& source, const ChessPosition& target) {
    PieceImage* piece = findPiece(source, _blackPieceList);
    if (!piece) {
        piece = findPiece(source, _whitePieceList);
    }

    if (piece) {
        piece->MoveTo({static_cast<float>(target.getColumn() - 'a'), static_cast<float>(8 - target.getRow())});
    }
}


void UI::removePieceFromList(const ChessPosition& position, std::vector<std::pair<int, PieceImage*>>& pieceList, PieceColor color){
    for (auto it = pieceList.begin(); it != pieceList.end(); ++it) {
        int pieceColumn = static_cast<int>((it->second->getPosition().x - MARGIN_SIZE)  / SQUARE_SIZE);
        int pieceRow = static_cast<int>((it->second->getPosition().y - MARGIN_SIZE) / SQUARE_SIZE);

        ChessPosition piecePos(pieceColumn + 'a', 8 - pieceRow);
        if (piecePos.getColumn() == position.getColumn() && piecePos.getRow() == piecePos.getRow()) {
            addToRemovedList(it->first, color);
            delete it->second;
            pieceList.erase(it);
            break;
        }
    }
}

void UI::removePiece(ChessPiece* capturedPiece) {
    // std::cout << "removePiece(): ENTROU AQUI " << std::endl;
    // std::cout << "(" << capturedPiece->getPosition().getColumn() << ", " << capturedPiece->getPosition().getRow() << ")" << std::endl;
    ChessPosition capturedPos = ChessPosition(capturedPiece->getPosition().getColumn() + 'a', 8 - capturedPiece->getPosition().getRow());
    PieceColor color = capturedPiece->getColor();

    std::cout << "removePiece(): " << static_cast<int>(color) <<" (" << capturedPos.getColumn() << ", " << capturedPos.getRow() << ")" << std::endl;

    if (color == PieceColor::W) {
        removePieceFromList(capturedPos, _whitePieceList, color);
    } else {
        removePieceFromList(capturedPos, _blackPieceList, color);
    }
}


void UI::addToRemovedList(int p, PieceColor color){
    PieceImage* piece;
    Vector2 pos;
    if(color == PieceColor::W){
        pos.x = MARGIN_SIZE + _whitePieceList.size();
        pos.y = MARGIN_SIZE/2;
        piece = new PieceImage(p, color, pos, true);
        _whiteRemovedList.push_back(std::make_pair(p, piece));
    } else {
        pos.x = MARGIN_SIZE + _whitePieceList.size() - 1;
        pos.y = MARGIN_SIZE + 8*SQUARE_SIZE;
        piece = new PieceImage(p, color, pos, true);
        _whiteRemovedList.push_back(std::make_pair(p, piece));
    }
}


// int UI::PieceMouseSelect(Vector2 mousePos){
//     mousePos.x = static_cast<int>(mousePos.x/SQUARE_SIZE);
//     mousePos.y = static_cast<int>(mousePos.y/SQUARE_SIZE);
//     // std::cout << "mousePos (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
//     // std::cout << turn << std::endl; 
//     // if(turn){
//     //     for(auto it = _whitePieceList.begin(); it != _whitePieceList.end(); it++){
//     //         Vector2 piecePos = it->second->getPosition();
//     //         int width = it->second->getImageWidth();
//     //             std::cout << "piecePos (" << piecePos.x << ", " << piecePos.y << ")" << std::endl;
//     //         if(mousePos.x + (SQUARE_SIZE - width)/2 ==  piecePos.x && mousePos.y ==  piecePos.y){
//     //             std::cout << "achou uma peca" << std::endl;
//     //             return it - _whitePieceList.begin();
//     //         }
//     //     }
//     //     return -1;
//     // }
//     // else {
//     //     for(auto it = _blackPieceList.begin(); it != _blackPieceList.end(); it++){
//     //         Vector2 piecePos = it->second->getPosition();
//     //         int width = it->sec        void setAsRemoved();ond->getImageWidth();
//     //         if(mousePos.x + (SQUARE_SIZE - width)/2 ==  piecePos.x && mousePos.y ==  piecePos.y){
//     //             return it - _blackPieceList.begin();
//     //         }
//     //     }
//     //     return -1;
//     // }
// }

std::string UI::getSelectedPosString(){
    Vector2 piecePos;
    std::string res;
    if(turn){
        piecePos = _whitePieceList[selectedPiece].second->getPosition();
    } else {
        piecePos = _blackPieceList[selectedPiece].second->getPosition();
    }
    piecePos.x = static_cast<int>(piecePos.x/SQUARE_SIZE);
    piecePos.y = static_cast<int>(piecePos.y/SQUARE_SIZE);
    
    res.push_back('a'+ piecePos.x - 1);
    res.push_back('1'+ piecePos.y - 1);
    return res;
}

void UI::DrawChessBoard(std::vector<std::vector<bool>> possibleMoves) {
    const int boardSize = 8;
    const float squareSize = SQUARE_SIZE; // Size of each square in pixels
    const float marginSize = MARGIN_SIZE;  // Margin around the board in pixels

    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            Color color = ((x + y) % 2 == 0) ? DARKGRAY : RAYWHITE;
            DrawRectangle(x * squareSize + marginSize, y * squareSize + marginSize, squareSize, squareSize, color);
             if ((not possibleMoves.empty()) && possibleMoves[x][y]) {
                Rectangle square = { y * squareSize + marginSize, x * squareSize + marginSize, squareSize, squareSize };
                DrawRectangleLinesEx(square, 10.0f, RED);
            }
        }
    }
}


PieceImage::PieceImage(int piece, PieceColor color, Vector2 pos, bool removed){
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
    if(removed)
        ImageResize(&img, 30, 50);
    else    
        ImageResize(&img, 60, 100);
    image = LoadTextureFromImage(img);
    position.x = pos.x + (SQUARE_SIZE - image.width)/2;
    position.y = pos.y;
}

PieceImage::~PieceImage(){
    UnloadTexture(image);
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

