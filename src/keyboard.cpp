#include "keyboard.hpp"


Keyboard::Keyboard () :
        curr_status(new bool[int(Key::key_count)]{false}),
        prev_status(new bool[int(Key::key_count)]{false})
{

}

bool Keyboard::poll_event (Event::Keyboard_event& event){
    for (int i = 0; i < int(Key::key_count); i++){
        if (curr_status[i] && !prev_status[i]){
            event.key = Key(i);
            return true;
        }
    }
    return false;
}

void Keyboard::update (){
    std::swap(curr_status, prev_status);

    for (int i = 0; i < int(Key::key_count); i++){
        if (is_key_pressed(Key(i))){
            curr_status[i] = true;
        }else{
            curr_status[i] = false;

        }
    }
}

Keyboard::~Keyboard(){
    delete[] curr_status;
    delete[] prev_status;
}