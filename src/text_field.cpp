#include "text_field.hpp"
#include "widget_graphics.hpp"
#include "settings.hpp"
Text_field::Text_field (const int x, const int y, const int width, const int height, const char* line) :
        Widget(x, y, width, height),
        cursor_position(0),
        cursor_index(0),
        background(0, 0, width, height, Color(16, 16, 16, 200), BORDER_THICKNESS, BORDER_COLOR),
        content(line),
        text(x, y, content, height - height/5, Color(240, 97,86))
{
    update_cursor_position();
}


void Text_field::draw (const int x, const int y, Texture& window){
    background.set_position(x + this->x, y + this->y);
    background.draw(window);

    if (focus){
        Rectangle cursor (this->x + x + cursor_position, y + this->y + height/10, 1, height - height/5, {255, 255, 255});
        cursor.draw(window);
    }
    text.set_position(x + this->x, y + this->y);
    text.draw(window);

}


bool Text_field::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    if (is_in_widget(x, y, event.click)){
        this->focus = true;
        return true;
    }else{
        this->focus = false;
        return false;
    }

    return false;
}

bool Text_field::on_text_enter (const Event::Text_enter& event){
    content.insert(cursor_index, 1, event.keycode);
    update_text();
    move_cursor_right();

    return 0;
}



bool Text_field::on_key_press (const Event::Press_key& event){
    if (this->focus){
        
        if (handle_hotkeys(event)){
            return true;
        }
        
        switch (event.key){
            case Key::left_arrow:{
                move_cursor_left();
                break;
            }

            case Key::right_arrow:{
                move_cursor_right();
                break;
            }

            case Key::Backspace:{
                delete_character();
                break;
            }

            case Key::Enter:{
                proceed_controllers();
                break;
            }

        }


        return true;
    }
    return false;
}


void Text_field::update_cursor_position (){
    std::string begin = content.substr(0, cursor_index);
    Text begin_text(0, 0, begin, height - height/5);

    Vector size = begin_text.get_boundings();
    cursor_position = size.x;

}


void Text_field::update_text (){
    text.set_string(content);
}


void Text_field::move_cursor_left (){
    if (cursor_index > 0){
        cursor_index--;
        update_cursor_position();
    }

}

void Text_field::move_cursor_right (){
    if (cursor_index < content.size()){
        cursor_index++;
        update_cursor_position();
    }
}

bool Text_field::handle_hotkeys (const Event::Press_key& event){

    if (event.key == Key::v && (event.control_key & int(Control_keys::left_ctrl))){
        std::string clipboard = get_string_from_clipboard();
        int length = clipboard.size();
        content.insert(cursor_index, clipboard);
        
        cursor_index += length;

        update_text();
        update_cursor_position();
        return true;
    }

    return false;
}






void Text_field::delete_character (){
    if (cursor_index > 0){
        content.erase(cursor_index - 1, 1);
        update_text();
        move_cursor_left();
    }
}



Text_field::~Text_field (){
    int size = controllers.size();
    for (int i = 0; i < size; i++){
        delete controllers[i];
    }
}