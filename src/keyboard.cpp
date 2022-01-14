#include "keyboard.hpp"
#include "singleton.hpp"


extern Singleton* global_singleton;

Keyboard::Keyboard () :
        curr_status(new bool[int(Key::key_count)]{false}),
        prev_status(new bool[int(Key::key_count)]{false})
{

}

bool Keyboard::poll_press_event (Event::Press_key& event){
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


bool Keyboard::poll_release_event (Event::Release_key& event){
    event.control_key = 0;
    if (curr_status[int(Key::LControl)]){
        event.control_key = event.control_key | int(Control_keys::left_ctrl);
    }
    
    for (int i = 0; i < int(Key::key_count); i++){
        if (i == int(Key::LControl)){
            continue;
        }

        if (!curr_status[i] && prev_status[i]){
            event.key = Key(i);
            return true;
        }
    }
    return false;
}

bool Keyboard::poll_text_enter (Event::Text_enter& evnt){
    sf::Event event;
    Window* win = global_singleton->get_window();


    while (win->get_win_ptr()->pollEvent(event))
    {
        if (event.type == sf::Event::TextEntered){
            if (event.text.unicode < 128 && event.text.unicode > 8){
                evnt.keycode = static_cast<char>(event.text.unicode);
                return true;
            }
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