#ifndef MAIN_WIDGET_WINDOW_HEADER
#define MAIN_WIDGET_WINDOW_HEADER

#include "widget.hpp"
#include "graphics_provider.hpp"



class Main_window_widget : public Widget_event_reciever {
    public:

    Main_window_widget (const int width, const int height, const char* name = "APP NAME");





    private:



    Window window;
    

};



#endif