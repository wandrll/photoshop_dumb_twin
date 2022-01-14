#ifndef WIDGET_GRAPHICS_HEADER
#define WIDGET_GRAPHICS_HEADER


#include "widget.hpp"
#include "graphics_provider.hpp"





class Rectangle_widget : virtual public Widget {
    public:

    Rectangle_widget (const int x, const int y, const int size_x, const int size_y, const Color& col = {255, 0, 0}, const int thickness = 0, const Color& outline_color = {0, 0, 0, 255}) :
        Widget(x, y, size_x, size_y),
        rect(0, 0, size_x, size_y, col, thickness, outline_color)
    {}

    virtual void draw (const int x, const int y, Texture& window){
        this->rect.draw(window, this->x + x, this->y + y);
    }
    
 

    private:

    Rectangle rect;

};



class Text_widget : virtual public Widget {
    public:

    Text_widget (const int x, const int y, const char* text, const int font_size, const Color& col = {0, 0, 0}) :
        Widget(x, y, font_size * strlen(text), font_size),
        text(0, 0, text, font_size, col)
    {}

    virtual void draw (const int x, const int y, Texture& window){
        this->text.draw(window, this->x + x, this->y + y);
    }
    
    
    void set_string (const std::string& line){
        this->text.set_string(line);
    }
    
    void centering (const int left, const int right){
        Vector size = text.get_boundings();
        double offset = (right - left - size.x)/2;
        
        this->x = left + offset;

    }
    
    private:
    Text text;
};




#endif