#include "main_window_widget.hpp"
#include "event.hpp"



void Main_window_widget::run (){
        Event::Left_Mouse_press mouse_press;
        Event::Mouse_release mouse_release;
        Event::Mouse_pressed_move mouse_pressed_move;
        Event::Mouse_released_move mouse_released_move;
        Event::Right_Mouse_press mouse_right_press;
        Event::Keyboard_event keyboard_event;

        while(true){
            draw();
            update();

            mouse.update();
            keyboard.update();

            if (this->mouse.get_press_event(mouse_press)){
                mouse_press.print();
                on_mouse_press(0, 0, mouse_press);
            }

            if (this->mouse.get_right_press_event(mouse_right_press)){
                mouse_press.print();
                on_right_mouse_press(0, 0, mouse_right_press);
            }
            
            if (this->mouse.get_release_event(mouse_release)){
                mouse_release.print();
                on_mouse_release(0, 0, mouse_release);
            }
            
            if (this->mouse.get_press_move_event(mouse_pressed_move)){
                mouse_pressed_move.print();
                on_mouse_pressed_move(0, 0, mouse_pressed_move);
            }
            
            if (this->mouse.get_release_move_event(mouse_released_move)){
                mouse_released_move.print();
                on_mouse_released_move(0, 0, mouse_released_move);
            }

            if (this->keyboard.poll_event(keyboard_event)){
                keyboard_event.print();
                on_keyboard(keyboard_event);
            }

            // if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
                // mark_close();
            // }


            if(this->mark_for_delete){
                break;
            }
        }

    }
