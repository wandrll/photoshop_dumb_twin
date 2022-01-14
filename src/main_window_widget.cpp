#include "singleton.hpp"
#include "drop_down_box.hpp"
#include "main_window_widget.hpp"
#include "top_bar.hpp"
// #include "event.hpp"
#include "mouse.hpp"
#include "keyboard.hpp"
#include "settings.hpp"
#include "canvas_window.hpp"

Main_window_widget::Main_window_widget (const int width, const int height, const char* name) :
        Widget(0, 0, width, height),
        Widget_event_reciever(0, 0, width, height),
        window(width, height, name)
    {   
        std::cout << "main window constructor begin" << std::endl;
        set_mouse(&window);
        global_singleton->set_window(&window);

        File_sub_widget* file = new File_sub_widget (0, 0, 100, TOP_BAR_HEIGHT);
        this->widgets.push_back(file);


        Top_bar* bar = new Top_bar(100, 0, RESOLUTION_WIDTH -100, TOP_BAR_HEIGHT, this);
        this->widgets.push_back(bar);

        std::cout << "main window constructor end" << std::endl;
    }