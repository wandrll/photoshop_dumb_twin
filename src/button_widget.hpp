#ifndef WIDGET_BUTTON_HEADER
#define WIDGET_BUTTON_HEADER

#include "widget.hpp"
#include "widget_graphics.hpp"
#include "sprite_widget.hpp"

class Rectangle_Button : virtual public Widget{
    public:

    Rectangle_Button (const int x, const int y, const int size_x, const int size_y, const int id = -1) :
        Widget(x, y, size_x, size_y),
        controller(NULL),
        is_pressed(false),
        regular_button(NULL),
        id(id)
    {}

    virtual ~Rectangle_Button (){

        for (int i = 0; i < this->controller.size(); i++){
            delete this->controller[i];
        }
        delete regular_button;
    }

    

    void add_controller (Abstract_controller* control){
        this->controller.push_back(control);
    }

   

    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
        if (is_in_widget(x, y, event.click)){
                
            is_pressed = true;
            return true;
        }
        
        
        
        return false;    
    }

    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
        if(is_pressed){
            for (int i = 0; i < this->controller.size(); i++){
                
                (*this->controller[i])(Data_for_controller(id));
                
            }
            is_pressed = false;
        }
        return false;
    }

    void set_normal_button(Widget* regular_button){
        this->regular_button = regular_button;
    }


    virtual void draw (const int x, const int y, Texture& window){
        regular_button->draw(x + this->x, y + this->y, window);
    }

    protected:
    Widget* regular_button;

    std::vector<Abstract_controller*> controller;
    bool is_pressed;

    int id;

};


class Text_button : public Rectangle_Button {
    public:
    Text_button (const int x, const int y, const int size_x, const int size_y, const char* line, const int font_size, const Color& text_color, const Color& background_color, const int id = -1) :
            Widget(x, y, size_x, size_y),
            Rectangle_Button (x, y, size_x, size_y, id){
        Widget_manager* widget = new Widget_manager(0, 0, size_x, size_y);

        Rectangle_widget* back = new Rectangle_widget(0, 0, size_x, size_y, background_color);
        Text_widget* text = new Text_widget (+3, -3, line, font_size, text_color);

        widget->register_widget(back);
        widget->register_widget(text);

        this->regular_button = widget;
    }

    void set_caption(const char* line, int font_size, const Vector& pos){
        delete this->regular_button;

        ::Widget_manager* widget = new ::Widget_manager(0, 0, width, height);

        ::Rectangle_widget* back = new ::Rectangle_widget(0, 0, width, height, ::Color(64, 53, 54));
        ::Text_widget* text = new ::Text_widget (pos.x, pos.y, line, font_size, ::Color(186, 30, 48));

        widget->register_widget(back);
        widget->register_widget(text);

        this->regular_button = widget;
    }
};



class Hover_rectangle_button : public Rectangle_Button{
    public:
    Hover_rectangle_button (const int x, const int y, const int size_x, const int size_y) :
        Widget(x, y, size_x, size_y), 
        Rectangle_Button(x, y, size_x, size_y),
        is_hovered(false),
        hovered_button(NULL)
    {}

    virtual ~Hover_rectangle_button (){
        delete hovered_button;
    }

    void set_hovered_button(Widget* hovered_button){
        this->hovered_button = hovered_button;
    }

    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event){
        if (is_in_widget(x, y, event.move.end_move) && !is_in_widget(x, y, event.move.begin_move)){
                is_hovered = true;
                return true;
        }

        if (!is_in_widget(x, y, event.move.end_move) && is_in_widget(x, y, event.move.begin_move)){
                is_hovered = false;
                return true;
        }


        return false;
    }


    virtual void draw (const int x, const int y, Texture& window){
        if (is_hovered && hovered_button){
            hovered_button->draw(x + this->x, y + this->y, window);
        }else{
            regular_button->draw(x + this->x, y + this->y, window);
        }
    }

    protected:
    bool is_hovered;
    Widget* hovered_button;


};




class Button_ok : public Hover_rectangle_button{
    public:

    Button_ok(const int x, const int y, const int size_x, const int size_y) :
            Widget(x, y, size_x, size_y),
            Hover_rectangle_button(x, y, size_x, size_y)
            {
                this->regular_button = new Widget_sprite(0, 0, this->width, this->height, Resource_type::OK_BUTTON);
                this->hovered_button = new Widget_sprite(0, 0, this->width, this->height, Resource_type::OK_HOVERED_BUTTON);
            }


    private:



};




#endif