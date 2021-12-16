#ifndef SLIDER_HEADER
#define SLIDER_HEADER

#include "widget.hpp"
#include "sprite_widget.hpp"
#include "text_field.hpp"

class Slider : public Widget_manager{
    public:

    Slider(const int x, const int y, const int width, const int height, const int left_x_limit, const int top_y_limit, const int right_x_limit, const int bottom_y_limit);

    virtual ~Slider(){
        // printf("slider destructor called\n");

        for (int i = 0; i < this->controller.size(); i++){
            delete this->controller[i];
        }
    }


    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    
    void proceed_controllers();

    void add_controller(Abstract_controller* controller){
        this->controller.push_back(controller);
    }

    void set (const Vector& value){
        this->x = left_x_limit + value.x * (right_x_limit  - left_x_limit) - this->width / 2;
        this->y = top_y_limit  + value.y * (bottom_y_limit - top_y_limit ) - this->height / 2;

    }


    Vector get_value() const{
        double x = 0;
        double y = 0;
        
        if (left_x_limit != right_x_limit){
            x = ((double)this->x + ((double)this->width/2) - ((double)left_x_limit)) / ((double)right_x_limit - left_x_limit);
        }

        if (top_y_limit != bottom_y_limit){
            y = ((double)this->y + ((double)this->height/2) - ((double)top_y_limit)) / ((double)bottom_y_limit - top_y_limit);
        }
        return Vector(x, y);
    }

    private:

    int left_x_limit;
    int top_y_limit;
    int right_x_limit;
    int bottom_y_limit;
    std::vector<Abstract_controller*> controller;

};



class Horizontal_slider_bar : public Widget_manager{
     public:

    Horizontal_slider_bar (int x, int y, int width, int height);

    void add_controller (Abstract_controller* control){
        this->slider->add_controller(control);
    }

    void proceed_controllers (){
        this->slider->proceed_controllers();
    }

    Slider* get_slider(){
        return slider;
    }

    protected:
    Slider* slider;



};


class Slider_bar_with_text_box : public Widget_manager{
    public:

    Slider_bar_with_text_box (const int x, const int y, const int width, const int height, const char* name = " ");

    void add_controller (Abstract_controller* control){
        this->slider_bar->add_controller(control);
    }

    protected:
    Horizontal_slider_bar* slider_bar;
    Text_field* text_field;


};



#endif