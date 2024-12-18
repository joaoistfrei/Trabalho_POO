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

// Função para iniciar a partida com as propriedades desejadas (tabuleiro, turno, jogador atual, etc)
ChessMatch::ChessMatch() : board(8, 8), turn(1), currentPlayer(PieceColor::W), check(false), checkMate(false), enPassantVulnerable(nullptr), promoted(nullptr), castling(0), enPassantCompleted(false) {
    initialSetup();
}

// Função para removar uma peça do tabuleiro
void ChessMatch::remove(Piece* piece) {
    std::cout << "Removendo peça" << piece << std::endl;
    piecesOnTheBoard.remove_if([piece](const std::unique_ptr<Piece>& p) { return p.get() == piece; });
    std::cout << "Removida peça" << piece << std::endl;
}

// getters
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

// funcao para ver quais sao os possiveis movimentos de uma peca escolhida
std::vector<std::vector<bool>> ChessMatch::possibleMoves(const ChessPosition& sourcePosition) const {
    Position position = sourcePosition.toPosition();
    validateSourcePosition(position);
    return board.piece(position)->possibleMoves();
}

// funcao para mover uma peca no tabuleiro
ChessPiece* ChessMatch::performChessMove(const ChessPosition& fromPosition, const ChessPosition& toPosition) {
    //transforma as posicoes de entrada de xadrez em posicoes matriciais
    Position from = fromPosition.toPosition();
    Position to = toPosition.toPosition();

    // verifica se as posicoes sao validas
    validateSourcePosition(from);
    validateTargetPosition(from, to);

    // realiza o movimento da peca
    ChessPiece* captured = dynamic_cast<ChessPiece*>(board.piece(to));
    Piece* capturedPiece = makeMove(from, to);

    // verifica se o rei esta em estado de cheque
    if (testCheck(currentPlayer)) {
        undoMove(from, to, capturedPiece);
        throw ChessException("You can't place yourself in check!");
    }

    ChessPiece* movedPiece = dynamic_cast<ChessPiece*>(board.piece(to));

    // analisa promocao de peao
    promoted = nullptr;
    if (dynamic_cast<Pawn*>(movedPiece)) {
        if ((movedPiece->getColor() == PieceColor::W && to.getRow() == 0) || (movedPiece->getColor() == PieceColor::B && to.getRow() == 7)) {
            promoted = dynamic_cast<ChessPiece*>(board.piece(to));
        }
    }

    // analisa en passant
    if(enPassantCompleted){
        Position enPassantPos(to.getRow(), to.getColumn());
        enPassantPos.setRow(enPassantPos.getRow() + (currentPlayer == PieceColor::W ? 1 : -1));
        capturedPiece = board.removePiece(enPassantPos);
        captured = dynamic_cast<ChessPiece*>(capturedPiece);
        if (captured != nullptr) {
            capturedPieces.push_back(std::unique_ptr<Piece>(std::move(capturedPiece)));
            remove(captured);
        }
    }

    // verifica cheque de novo
    check = testCheck(opponent(currentPlayer));

    // verifica cheque mate
    if (testCheckMate(opponent(currentPlayer))) {
        checkMate = true;
    } 
    // se for o caso, coloca o peao como vulneravel ao en passant
    if (dynamic_cast<Pawn*>(movedPiece) && (to.getRow() == from.getRow() + 2 || to.getRow() == from.getRow() - 2)) {
            enPassantVulnerable = movedPiece;
    } else {
        enPassantVulnerable = nullptr;
    }

    return captured;
}

// quando um peao eh promovido, escolhe qual peca vai ser posta no lugar
ChessPiece* ChessMatch::replacePromotedPiece(const std::string& type) {
    if (promoted == nullptr) {
        throw std::logic_error("There is no piece to be promoted!");
    }
    if (type != "B" && type != "N" && type != "R" && type != "Q") {
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

// cria uma nova peca do xadrez, de acordo com o tipo e cor passados como argumento
std::unique_ptr<ChessPiece> ChessMatch::createNewPiece(const std::string& type, PieceColor color) {
    if (type == "B") return std::make_unique<Bishop>(&board, color);
    if (type == "N") return std::make_unique<Knight>(&board, color);
    if (type == "R") return std::make_unique<Rook>(&board, color);
    return std::make_unique<Queen>(&board, color);
}

// funcao principal de movimentar pecas
Piece* ChessMatch::makeMove(const Position& source, const Position& target) {

    // tira a peca da origem
    ChessPiece* p = dynamic_cast<ChessPiece*>(board.removePiece(source));
    p->increaseMoveCount(); // aumenta o numero de movimentos 

    // remove a peca que estava no destino e salva em capturedPiece
    Piece* capturedPiece = board.removePiece(target);
    
    std::cout << "Captured piece type: " << capturedPiece << std::endl;

    // coloca a peca movida no destino
    board.placePiece(p, target);

    // castling kingside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() + 2) {
        Position sourceT(source.getRow(), source.getColumn() + 3);
        Position targetT(source.getRow(), source.getColumn() + 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(sourceT));
        board.placePiece(rook, targetT);
        rook->increaseMoveCount();
        castling = 1;
    }

    // castling queenside
    else if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() - 2) {
        Position sourceT(source.getRow(), source.getColumn() - 4);
        Position targetT(source.getRow(), source.getColumn() - 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(sourceT));
        board.placePiece(rook, targetT);
        rook->increaseMoveCount();
        castling = 2;
    } else 
        castling = 0;

    // en passant
    if (dynamic_cast<Pawn*>(p)) {
        if (source.getColumn() != target.getColumn() && capturedPiece == nullptr) {
            Position pawnPos;
            enPassantCompleted = true;
            
        } else
            enPassantCompleted = false;
    }

    if (capturedPiece != nullptr) {
        // Encontrando o unique_ptr correspondente em piecesOnTheBoard
        auto it = std::find_if(piecesOnTheBoard.begin(), piecesOnTheBoard.end(),
                            [capturedPiece](const std::unique_ptr<Piece>& p) { return p.get() == capturedPiece; });

        if (it != piecesOnTheBoard.end()) {
            // Movendo o unique_ptr encontrado para capturar a ownership
            std::unique_ptr<Piece> newPiece = std::move(*it);  // Move o unique_ptr para 'newPiece'
            
            // Remove o unique_ptr da lista
            piecesOnTheBoard.erase(it);
            
            // Adiciona o objeto à lista capturedPieces (movendo a ownership)
            capturedPieces.push_back(std::move(newPiece));  // Move ownership para capturedPieces
        }
    }



    return capturedPiece;
}

// desfaz o movimento no caso de movimento ilegal
void ChessMatch::undoMove(const Position& source, const Position& target, Piece* captured) {
    // remove peca do destino
    ChessPiece* p = dynamic_cast<ChessPiece*>(board.removePiece(target));
    // diminui numero de movimentos
    p->decreaseMoveCount();

    // volta com a peca para o lugar
    board.placePiece(p, source);

    // se houver capturado uma peca, voltar com ela p lugar
    if (captured != nullptr) {
        auto it = std::find_if(capturedPieces.begin(), capturedPieces.end(),
                       [captured](const std::unique_ptr<Piece>& piece) {
                           return piece.get() == captured;
                        });
        if(it != capturedPieces.end()){
            std::unique_ptr<Piece> newPtr = std::move(*it);
            board.placePiece(newPtr.get(), target);
            piecesOnTheBoard.push_back(std::move(newPtr));
            capturedPieces.erase(it);
        }
    }

    // castling kingside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() + 2) {
        Position sourceT(source.getRow(), source.getColumn() + 3);
        Position targetT(source.getRow(), source.getColumn() + 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(targetT));
        board.placePiece(rook, sourceT);
        rook->decreaseMoveCount();
    }
    // castling queenside
    if (dynamic_cast<King*>(p) && target.getColumn() == source.getColumn() - 2) {
        Position sourceT(source.getRow(), source.getColumn() - 4);
        Position targetT(source.getRow(), source.getColumn() - 1);
        ChessPiece* rook = dynamic_cast<ChessPiece*>(board.removePiece(targetT));
        board.placePiece(rook, sourceT);
        rook->decreaseMoveCount();
    }

    // en passant
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
            enPassantCompleted = false;
        }
    }
}

// validar posicao de origem
void ChessMatch::validateSourcePosition(const Position& position) const {
    if (!board.thereIsAPiece(position)) {
        throw ChessException("Error moving piece! Source position empty!");
    }
    if (currentPlayer != dynamic_cast<ChessPiece*>(board.piece(position))->getColor()) {
        throw ChessException("Chosen piece is not yours!");
    }
    if (!board.piece(position)->isThereAnyPossibleMove()) {
        throw ChessException("There is no possible move for chosen piece!");
    }
}

// validar posicao de destino
void ChessMatch::validateTargetPosition(const Position& from, const Position& to) const {
    if (!board.piece(from)->possibleMove(to)) {
        throw ChessException("The chosen piece can't move to target position!");
    }
}

// muda o turno
void ChessMatch::nextTurn() {
    turn++;
    currentPlayer = (currentPlayer == PieceColor::W ? PieceColor::B : PieceColor::W);
}

// retorna a cor do oponente
PieceColor ChessMatch::opponent(PieceColor color) const {
    return (color == PieceColor::W ? PieceColor::B : PieceColor::W);
}

// retorna um ponteiro pro rei de uma cor
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

// verifica se o rei de uma cor esta em cheque
bool ChessMatch::testCheck(PieceColor color) const {
    Position kingPosition = king(color)->getChessPosition().toPosition();
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* opponentPiece = dynamic_cast<ChessPiece*>(piece.get());
        if (opponentPiece->getColor() == opponent(color)) {
            if(!board.thereIsAPiece(opponentPiece->getPosition()))
            continue;
            std::vector<std::vector<bool>> mat = opponentPiece->possibleMoves();
            if (mat[kingPosition.getRow()][kingPosition.getColumn()]) {
                return true;
            }
        }
    }
    return false;
}

// testa se esta em cheque e se tem algum movimento que possa tirar do cheque (caso contrario, cheque mate)
bool ChessMatch::testCheckMate(PieceColor color) const {
    if (!testCheck(color)) {
        return false;
    }
    for (const auto& piece : piecesOnTheBoard) {
        ChessPiece* chessPiece = dynamic_cast<ChessPiece*>(piece.get());
        if(!board.thereIsAPiece(chessPiece->getPosition()))
            continue;
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

// coloca uma peca no tabuleiro
void ChessMatch::placeNewPiece(char column, int row, std::unique_ptr<ChessPiece> piece) {
    board.placePiece(piece.get(), ChessPosition(column, row).toPosition());
    piecesOnTheBoard.push_back(std::move(piece));
}

// coloca as pecas como comumente se colocam no xadrez (aqui poderiamos colocar quaisquer pecas onde quisermos)
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