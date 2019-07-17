#ifndef __ENGINE__CORE__FONT__FONT__
#define __ENGINE__CORE__FONT__FONT__

#include <allegro5/allegro_font.h>
#include <exception>
#include <string>

class FontException : public std::exception
{
    std::string message;
public:
    FontException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};

class Font
{
protected:
    ALLEGRO_FONT *_font;

    Font(int);
public:
    const int size;

    Font();
    Font(std::string filename, int size);
    ~Font();

    operator ALLEGRO_FONT *() const;
    operator const ALLEGRO_FONT *() const;
};

#endif