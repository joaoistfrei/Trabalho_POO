#include "../include/Position.h"
#include <sstream>

Position::Position() : row(0), column(0) {}

Position::Position(int row, int column) : row(row), column(column) {}

int Position::getRow() const {
    return row;
}

int Position::getColumn() const {
    return column;
}

void Position::setRow(int row) {
    this->row = row;
}

void Position::setColumn(int column) {
    this->column = column;
}

void Position::setValues(int row, int column) {
    this->row = row;
    this->column = column;
}

std::string Position::toString() const {
    std::ostringstream oss;
    oss << "(" << row << ", " << column << ")";
    return oss.str();
}