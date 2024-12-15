#include "../include/game.hpp"
#include <cmath>
#include <iostream>

Game::Game(){
    turn = true;

    Vector2 piecePos;
    for(int i = 0; i < 8; i++){
        PieceImage* piece = nullptr;
        // BLACK PAWNS
        piecePos.x = MARGIN_SIZE + i*SQUARE_SIZE; // Rooks x position
        piecePos.y = MARGIN_SIZE + SQUARE_SIZE; // Rooks y position 
        piece = new PieceImage(PAWN, 0, piecePos);
        _blackPieceList.push_back(std::make_pair(PAWN, piece));
        // WHITE PAWNS
        piecePos.y = MARGIN_SIZE + 6 * SQUARE_SIZE; // y pos for white pieces(down on board)
        piece = new PieceImage(PAWN, 1, piecePos);
        _whitePieceList.push_back(std::make_pair(PAWN, piece));
    }

    // OTHER PIECES (i = 0 -> BLACK // i = 1 -> WHITE)
    for(int i = 0; i < 2; i++){
        PieceImage* piece = nullptr;
        piecePos.y = MARGIN_SIZE + 7 * i * SQUARE_SIZE;
        // ROOKS
        piecePos.x = MARGIN_SIZE;
        piece = new PieceImage(ROOK, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(ROOK, piece));
        else
            _whitePieceList.push_back(std::make_pair(ROOK, piece));

        piecePos.x = MARGIN_SIZE + 7 * SQUARE_SIZE;
        piece = new PieceImage(ROOK, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(ROOK, piece));
        else
            _whitePieceList.push_back(std::make_pair(ROOK, piece));

        // KNIGHTS
        piecePos.x = MARGIN_SIZE + SQUARE_SIZE;
        piece = new PieceImage(KNIGHT, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(KNIGHT, piece));
        else
            _whitePieceList.push_back(std::make_pair(KNIGHT, piece));

        piecePos.x = MARGIN_SIZE + 6*SQUARE_SIZE;
        piece = new PieceImage(KNIGHT, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(KNIGHT, piece));
        else
            _whitePieceList.push_back(std::make_pair(KNIGHT, piece));
        
        // BISHOPS
        piecePos.x = MARGIN_SIZE + 2 * SQUARE_SIZE;
        piece = new PieceImage(BISHOP, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(BISHOP, piece));
        else
            _whitePieceList.push_back(std::make_pair(BISHOP, piece));

        piecePos.x = MARGIN_SIZE + 5 * SQUARE_SIZE;
        piece = new PieceImage(BISHOP, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(BISHOP, piece));
        else
            _whitePieceList.push_back(std::make_pair(BISHOP, piece));

        // QUEEN
        piecePos.x = MARGIN_SIZE + (3 + i) * SQUARE_SIZE;
        piece = new PieceImage(QUEEN, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(QUEEN, piece));
        else
            _whitePieceList.push_back(std::make_pair(QUEEN, piece));

        // KING
        piecePos.x = MARGIN_SIZE + (4 - i) * SQUARE_SIZE;
        piece = new PieceImage(KING, i, piecePos);
        if(i == 0)
            _blackPieceList.push_back(std::make_pair(KING, piece));
        else
            _whitePieceList.push_back(std::make_pair(KING, piece));
    }
}

Game::~Game(){
}

void Game::Draw(){
    DrawChessBoard();
    for(auto it = _blackPieceList.begin(); it != _blackPieceList.end(); it++)
        it->second->Draw();
    for(auto it = _whitePieceList.begin(); it != _whitePieceList.end(); it++)
        it->second->Draw(); 
}

void Game::Update(){
}

void Game::HandleInput(bool& isPieceSelected){
    Vector2 mousePos = GetMousePosition();

    if(not isPieceSelected){
        if(IsMouseButtonPressed(0)){
            std::cout << "entrou aq" << std::endl;
            int piecePos = GetSelectedPiece(mousePos);
            isPieceSelected = piecePos != -1 ? true : false;
            if(isPieceSelected){
                selectedPiece =  piecePos;
            }
        }
    } else {
        if(IsMouseButtonPressed(0)){
            std::cout << "entrou aq 3" << std::endl;
            mousePos.x = static_cast<int>(mousePos.x/SQUARE_SIZE);
            mousePos.y = static_cast<int>(mousePos.y/SQUARE_SIZE);
            
            if(turn)
                _whitePieceList[selectedPiece].second->MoveTo(mousePos); // MUDAR
            else
                _blackPieceList[selectedPiece].second->MoveTo(mousePos);

            isPieceSelected = false;
            turn = !turn;
        }
    }
}

void Game::DrawChessBoard() {
    const int boardSize = 8;
    const float squareSize = SQUARE_SIZE; // Size of each square in pixels
    const float marginSize = MARGIN_SIZE;  // Margin around the board in pixels

    for (int x = 0; x < boardSize; x++) {
        for (int y = 0; y < boardSize; y++) {
            Color color = ((x + y) % 2 == 0) ? DARKGRAY : RAYWHITE;
            DrawRectangle(x * squareSize + marginSize, y * squareSize + marginSize, squareSize, squareSize, color);
        }
    }
}



int Game::GetSelectedPiece(Vector2 mousePos){
    mousePos.x = static_cast<int>(mousePos.x/SQUARE_SIZE)*100;
    mousePos.y = static_cast<int>(mousePos.y/SQUARE_SIZE)*100;
    std::cout << "mousePos (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
    std::cout << turn << std::endl; 
    if(turn){
        for(auto it = _whitePieceList.begin(); it != _whitePieceList.end(); it++){
            Vector2 piecePos = it->second->getPosition();
            int width = it->second->getImageWidth();
                std::cout << "piecePos (" << piecePos.x << ", " << piecePos.y << ")" << std::endl;
            if(mousePos.x + (SQUARE_SIZE - width)/2 ==  piecePos.x && mousePos.y ==  piecePos.y){
                std::cout << "achou uma peca" << std::endl;
                return it - _whitePieceList.begin();
            }
        }
        return -1;
    }
    else {
        for(auto it = _blackPieceList.begin(); it != _blackPieceList.end(); it++){
            Vector2 piecePos = it->second->getPosition();
            int width = it->second->getImageWidth();
            if(mousePos.x + (SQUARE_SIZE - width)/2 ==  piecePos.x && mousePos.y ==  piecePos.y){
                return it - _blackPieceList.begin();
            }
        }
        return -1;
    }
}

