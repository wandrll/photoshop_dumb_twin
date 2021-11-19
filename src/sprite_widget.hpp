#ifndef SPRITE_WIDGET_HEADER
#define SPRITE_WIDGET_HEADER

#include "widget.hpp"
#include "resource_handler.hpp"

class Widget_sprite : public Widget{
    public:
    
    Widget_sprite (const int x, const int y, const int size_x, const int size_y, const Resource_type type) :
        Widget(x, y, size_x, size_y),
        sprite_id(type)
    {
        // printf("x = %d y = %d\n", this->x, this->y);
    }

    virtual void draw (const int x, const int y, Window& window);


   

    private:

    Resource_type sprite_id;

};





#endif
