#ifndef MOUSE_HEADER
#define MOUSE_HEADER

#include "graphics_provider.hpp"
// #include <chrono>
#include "event.hpp"




class Mouse{
public:
    Mouse (Window* win) :
        prev_coords(0,0),
        curr_coords(0,0),
        prev_left_status(false),
        curr_left_status(false),
        prev_right_status(false),
        curr_right_status(false),
      
        window(win)
    {}

    Mouse ():
        window(nullptr)
    {}
    
    void update();

    bool get_press_event        (Event::Left_Mouse_press& event);
    bool get_release_event      (Event::Mouse_release& event);
    bool get_press_move_event   (Event::Mouse_pressed_move& event);
    bool get_release_move_event (Event::Mouse_released_move& event);
    bool get_right_press_event  (Event::Right_Mouse_press& event);


private:
    Click prev_coords;
    Click curr_coords;

    bool prev_left_status;
    bool curr_left_status;


    bool prev_right_status;
    bool curr_right_status;

    // std::chrono::time_point<std::chrono::steady_clock> star;
    Window* window;

  

};



#endif