#ifndef TOP_BAR_HEADER
#define TOP_BAR_HEADER

#include <vector>
#include "graphics_provider.hpp"
#include "widget.hpp"
#include "button_widget.hpp"
#include "resource_handler.hpp"
#include "sprite_widget.hpp"
// #include "main_window_widget.hpp"



class Top_bar : public Widget_manager {
    public:

    Top_bar(const int x, const int y, const int width, const int height, Widget* win);


};


class Dragable_top_bar_zone : public Widget_manager{
    public:

    Dragable_top_bar_zone (const int x, const int y, const int size_x, const int size_y);
    ~Dragable_top_bar_zone ();
  
    virtual void update (){

    }

    void set_controller (Controller* control){
        this->controller = control;
    }

    // virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    

    private:

    Controller* controller;

};





#endif