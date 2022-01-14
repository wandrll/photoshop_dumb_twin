#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER


#include "graphics_provider.hpp"
#include "event.hpp"

class Keyboard{
    public:

    Keyboard ();

    bool poll_press_event (Event::Press_key& event);
    bool poll_release_event (Event::Release_key& event);
    bool poll_text_enter (Event::Text_enter& evnt);

    void update ();

    ~Keyboard();

    private:

    bool* curr_status;
    bool* prev_status;

};



std::string get_string_from_clipboard();

#endif