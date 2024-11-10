#ifndef BOARDEXCEPTION_H
#define BOARDEXCEPTION_H

#include <stdexcept>
#include <string>

class BoardException : public std::runtime_error {
public:
    explicit BoardException(const std::string& msg);
};

#endif // BOARDEXCEPTION_H