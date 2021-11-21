#ifndef WIDGET_GRAPHICS_HEADER
#define WIDGET_GRAPHICS_HEADER


#include "widget.hpp"
#include "graphics_provider.hpp"





class Rectangle_widget : public Widget {
    public:

    Rectangle_widget (const int x, const int y, const int size_x, const int size_y, const Color& col = {255, 0, 0}) :
        Widget(x, y, size_x, size_y),
        rect(0, 0, size_x, size_y, col)
    {}

    virtual void draw (const int x, const int y, Window& window){
        this->rect.draw(window, this->x + x, this->y + y);
    }
    
 

    private:

    Rectangle rect;

};



class Text_widget : public Widget {
    public:

    Text_widget (const int x, const int y, const char* text, const int font_size, const Color& col = {0, 0, 0}) :
        Widget(x, y, font_size * strlen(text), font_size),
        text(0, 0, text, font_size, col)
    {}

    virtual void draw (const int x, const int y, Window& window){
        this->text.draw(window, this->x + x, this->y + y);
    }
    
    
    void set_string (const std::string& line){
        this->text.set_string(line);
    }
   
    
    private:
    Text text;
};


template<typename Func>
class Controller_text_widget: public Text_widget {
    public:

    Controller_text_widget (Text_widget* widget) :
        widget(widget)
    {}

    virtual void operator() (const Data_for_controller& data){
        if (widget){
            Func()(data, widget);

        }
    }

    private:

    Text_widget* widget;

};


#endif