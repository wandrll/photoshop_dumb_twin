#include "mouse.hpp"


void Mouse::update(){
    this->prev_coords = this->curr_coords;
    this->curr_coords = window->get_mouse_position();
    
    this->prev_left_status = this->curr_left_status;
    this->curr_left_status = is_mouse_left_button_pressed();

    this->prev_right_status = this->curr_right_status;
    this->curr_right_status = is_mouse_right_button_pressed();
}

bool Mouse::get_press_event        (Event::Left_Mouse_press& event){
    if (!prev_left_status && curr_left_status){
        event.click = this->curr_coords;
        return true;
    }
    return false;
}

bool Mouse::get_release_event      (Event::Mouse_release& event){
    if (prev_left_status && !curr_left_status){
        event.click = this->curr_coords;        
        return true;
    }
    return false;
}

bool Mouse::get_press_move_event   (Event::Mouse_pressed_move& event){
    if (prev_left_status && curr_left_status){
        if (this->prev_coords == this->curr_coords){
            return false;
        }

        event.move.begin_move = this->prev_coords;
        event.move.end_move = this->curr_coords;
        return true;
    }
    return false;
}

bool Mouse::get_right_press_event (Event::Right_Mouse_press& event){
    if (!prev_right_status && curr_right_status){
        event.click = this->curr_coords;
        return true;
    }
    return false;
}

bool Mouse::get_release_move_event (Event::Mouse_released_move& event){
    if (!prev_left_status && !curr_left_status){
        if (this->prev_coords == this->curr_coords){
            return false;
        }
        event.move.begin_move = this->prev_coords;
        event.move.end_move = this->curr_coords;
        return true;
    }
    return false;
}

