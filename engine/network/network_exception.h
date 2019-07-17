#ifndef __ENGINE__NETWORK__NETWORK_EXCEPTION__
#define __ENGINE__NETWORK__NETWORK_EXCEPTION__

#include <exception>
#include <string>

class NetworkException : public std::exception
{
    std::string message;
public:
    NetworkException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};

class SteamException : public std::exception
{
    std::string message;
public:
    SteamException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};

#endif
