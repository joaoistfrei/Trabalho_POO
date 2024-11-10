#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position {
private:
    int row;
    int column;

public:
    Position();
    Position(int row, int column);

    int getRow() const;
    int getColumn() const;

    void setRow(int row);
    void setColumn(int column);

    void setValues(int row, int column);

    std::string toString() const;
};

#endif // POSITION_H