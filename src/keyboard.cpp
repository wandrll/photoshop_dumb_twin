#include "keyboard.hpp"


Keyboard::Keyboard () :
        curr_status(new bool[int(Key::key_count)]{false}),
        prev_status(new bool[int(Key::key_count)]{false})
{

}

bool Keyboard::poll_event (Event::Keyboard_event& event){
    event.control_key = 0;
    if (curr_status[int(Key::LControl)]){
        event.control_key = event.control_key | int(Control_keys::left_ctrl);
        // std::cout << event.control_key << " update control " << int(Control_keys::left_ctrl) << "\n";
        // return true;
    }
    
    
    for (int i = 0; i < int(Key::key_count); i++){
        if (i == int(Key::LControl)){
            continue;
        }

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

std::string get_string_from_clipboard(){
    return sf::Clipboard::getString();
}