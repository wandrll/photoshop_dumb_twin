#ifndef MAIN_WIDGET_WINDOW_HEADER
#define MAIN_WIDGET_WINDOW_HEADER

#include "widget.hpp"
#include "top_bar.hpp"
// #include "event.hpp"
#include "mouse.hpp"
#include "keyboard.hpp"
#include "settings.hpp"
#include "canvas_window.hpp"

class Main_window_widget : public Widget_manager {
    public:

    Main_window_widget (const int width, const int height, const char* name = "APP NAME") :
        Widget_manager(0, 0, width, height),
        window(width, height, name),
        mouse(&window)
    {
        Top_bar* bar = new Top_bar(0, 0, RESOLUTION_WIDTH, TOP_BAR_HEIGHT, this);
        this->widgets.push_back(bar);
    }


   

    void draw (){
        this->window.clear();
        Widget_manager::draw(0, 0, this->window);
        this->window.display();
    }    

   

    void run ();
    


    private:



    Window window;
    Mouse mouse;
    Keyboard keyboard;

};



#endif