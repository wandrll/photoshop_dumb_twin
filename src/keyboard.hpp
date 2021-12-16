#ifndef KEYBOARD_HEADER
#define KEYBOARD_HEADER


#include "graphics_provider.hpp"
#include "event.hpp"

class Keyboard{
    public:

    Keyboard ();

    bool poll_event (Event::Keyboard_event& event);

    void update ();

    ~Keyboard();

    private:

    bool* curr_status;
    bool* prev_status;

};



std::string get_string_from_clipboard();

#endif