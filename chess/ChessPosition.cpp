#include "../include/ChessPosition.h"
#include "../include/ChessException.h"
#include "../include/Position.h"
#include <sstream>

ChessPosition::ChessPosition(char column, int row) {
    if (column < 'a' || column > 'h' || row < 1 || row > 8) {
        throw ChessException("Error instantiating ChessPosition. Valid values are from a1 to h8.");
    }
    this->column = column;
    this->row = row;
}

char ChessPosition::getColumn() const {
    return column;
}

int ChessPosition::getRow() const {
    return row;
}

Position ChessPosition::toPosition() const {
    return Position(8 - row, column - 'a');
}

ChessPosition ChessPosition::fromPosition(const Position& position) {
    return ChessPosition((char)('a' + position.getColumn()), 8 - position.getRow());
}

std::string ChessPosition::toString() const {
    std::ostringstream oss;
    oss << column << row;
    return oss.str();
}