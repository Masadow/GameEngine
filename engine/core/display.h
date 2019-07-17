#ifndef __ENGINE__CORE__DISPLAY__
#define __ENGINE__CORE__DISPLAY__

#include <string>
#include <allegro5/allegro.h>
#include "font/font.h"
#include "state.h"

class DisplayException : public std::exception
{
    std::string message;
public:
    DisplayException(std::string message) : message(message) {};
    virtual const char* what() const throw() { return message.c_str(); }
};


class Display
{
    ALLEGRO_DISPLAY *_display;
    Font *_default_font;
    ALLEGRO_BITMAP *_cur_bmp;

public:
    Display(int width, int height);
    ~Display();

    ALLEGRO_DISPLAY *getAllegroDisplay() const;
    
    const int width;
    const int height;

    void setDefaultFont(Font &);
    Font *getDefaultFont() const;

    void cleanStates();
    ALLEGRO_EVENT_SOURCE *eventSource() const;

    void startDrawing();
    void endDrawing() const;

    void startBitmapDrawing(ALLEGRO_BITMAP *bmp, int flags = ALLEGRO_LOCK_READWRITE);
    void endBitmapDrawing();
};

#endif