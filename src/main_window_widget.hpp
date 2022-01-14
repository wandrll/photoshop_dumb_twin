#ifndef MAIN_WIDGET_WINDOW_HEADER
#define MAIN_WIDGET_WINDOW_HEADER

#include "widget.hpp"
#include "graphics_provider.hpp"



class Main_window_widget : public Widget_event_reciever {
    public:

    Main_window_widget (const int width, const int height, const char* name = "APP NAME");

    virtual void draw (const int x, const int y, Texture& window) override{
        std::cout << "widget manager draw" << std::endl;
        Widget_event_reciever::draw(x, y, window);
    }




    private:



    Window window;
    

};



#endif