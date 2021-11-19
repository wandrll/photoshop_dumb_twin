#ifndef SLIDER_HEADER
#define SLIDER_HEADER

#include "widget.hpp"
#include "sprite_widget.hpp"

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
    
    void proceed_controllers(const Data_for_controller& data);

    void add_controller(Controller* controller){
        this->controller.push_back(controller);
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
    std::vector<Controller*> controller;

};



class Horizontal_slider_bar : public Widget_manager{
     public:

    Horizontal_slider_bar (int x, int y, int width, int height);

    void add_controller (Controller* control){
        this->slider->add_controller(control);
    }


    protected:
    Slider* slider;



};


#endif