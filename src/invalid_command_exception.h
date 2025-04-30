#ifndef INVALID_COMMAND_EXCEPTION_H
#define INVALID_COMMAND_EXCEPTION_H

#include <exception>
#include <string>

class invalid_command_exception : public std::exception
{
private:
    std::string message;

public:
    explicit invalid_command_exception(const std::string &msg) : message(msg) {}

    const char *what() const noexcept override
    {
        return message.c_str();
    }
};

#endif
