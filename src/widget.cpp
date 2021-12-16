#include "widget.hpp"
#include <ctime>

bool Widget::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    return false;    
}

bool Widget::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    return false;
}
bool Widget::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    return false;
}
bool Widget::on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event){
    return false;
}
bool Widget::on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event){
    return false;
}

bool Widget::on_keyboard (const Event::Keyboard_event& event){
    return false;
}


void Widget_manager::draw (const int x, const int y, Window& window){
    int size = widgets.size();

    for (int i = 0; i < size; i++){
        this->widgets[i]->draw(this->x + x, this->y + y, window);
    }
}




bool Widget_manager::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    if (!is_accept_events){
        return false;
    }

    int size = widgets.size();
    bool res = false;

    for (int i = size - 1; i >= 0; i--){
        if(this->widgets[i]->on_mouse_press(x + this->x, y + this->y, event)){
            std::swap(this->widgets[i], this->widgets[size-1]);       
            res = true;
            break;
        }
    }

    
    res = res || (    this->x + x <= event.click.x && event.click.x <= this->width  + this->x + x 
                   && this->y + y <= event.click.y && event.click.y <= this->height + this->y + y);
    
    this->is_active = res;
    return res;   
}

bool Widget_manager::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    if (!is_accept_events){
        return false;
    }

    int size = widgets.size();
    bool res = false;

    for (int i = size - 1; i >= 0; i--){
        if(this->widgets[i]->on_mouse_release(x + this->x, y + this->y, event)){
            res = true;
            break;
        }
    }
    this->is_active = false;
    return res;

}

bool Widget_manager::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (!is_accept_events){
        return false;
    }

    int size = widgets.size();
    bool res = false;

    for (int i = size - 1; i >= 0; i--){
        if(this->widgets[i]->on_mouse_pressed_move(x + this->x, y + this->y, event)){
            res = true;
            break;
        }
    }

    return res;
}

bool Widget_manager::on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event){
    if (!is_accept_events){
        return false;
    }

    int size = widgets.size();
    bool res = false;

    for (int i = size - 1; i >= 0; i--){
        if(this->widgets[i]->on_mouse_released_move(x + this->x, y + this->y, event)){
            res = true;
            break;
        }
    }
    return res;
}

bool Widget_manager::on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event){
    if (!is_accept_events){
        return false;
    }

    int size = widgets.size();
    bool res = false;

    for (int i = size - 1; i >= 0; i--){
        if(this->widgets[i]->on_right_mouse_press(x + this->x, y + this->y, event)){
            res = true;
            break;
        }
    }
    return res;
}

bool Widget_manager::on_keyboard (const Event::Keyboard_event& event){
    if (!is_accept_events){
        return false;
    }
    
    int size = widgets.size();
    bool res = false;

    for (int i = size - 1; i >= 0; i--){
        if(this->widgets[i]->on_keyboard(event)){
            res = true;
            break;
        }
    }
    return res;
}

Widget_manager::~Widget_manager (){
    int size = widgets.size();

    for (int i = 0; i < size; i++){
        delete this->widgets[i];
    }
}


void Widget_manager::update (){
    for (int i = 0; i < this->widgets.size(); i++){
        widgets[i]->update();
    }
    
    // printf("\n--------------------------\n");
    for (int i = 0; i < this->widgets.size() - 1; i++){
        // printf("%p %d\n", widgets[i], widgets[i]->is_mark_for_close());
        if(widgets[i]->is_marked_for_deletion()){
            // printf("Close\n");
            delete widgets[i];
            std::swap(widgets[i], widgets[widgets.size() - 1]);
            widgets.pop_back();
            i--;
        }
    }

    if(widgets.back()->is_marked_for_deletion()){
        // printf("%p %d\n", widgets.back(), widgets.back()->is_mark_for_close());
        // printf("Close\n");
        delete widgets.back();
        widgets.pop_back();
    }
}

#include "canvas_window.hpp"

void Widget_manager::open_image (const std::string& name){
    Canvas_widget* canv = new Canvas_widget (400, 400, 600, 600);
    canv->load_image(name);
    this->widgets.push_back(canv);
    //app.register_widget(canv);
}


Widget_event_reciever::Widget_event_reciever (const int x, const int y, const int width, const int height) :
        Widget_manager(x, y, width, height),
        mouse(global_singleton->get_window())
{}

void Widget_event_reciever::run(){
    // Widget* main_widget = global_singleton->get_main_widget();
    Texture clear_texture(width, height);
    clear_texture.fill_color({32, 32, 32});
    
    Sprite clear_sprite;
    clear_sprite.set_texture(clear_texture);

    // clear_texture.clear(Color(32, 32, 32, 255));
    clear_sprite.set_position(x, y);
    clear_sprite.set_size(width, height);

    Blend_mode mode(Blending::Factor::One, Blending::Factor::Zero);


    Event::Left_Mouse_press mouse_press;
    Event::Mouse_release mouse_release;
    Event::Mouse_pressed_move mouse_pressed_move;
    Event::Mouse_released_move mouse_released_move;
    Event::Right_Mouse_press mouse_right_press;
    Event::Keyboard_event keyboard_event;
    clock_t curr = clock();
    clock_t prev = curr;
    while(true){
        prev = curr;
        curr = clock();
        
        global_singleton->get_tools()->get_tool()->on_tick(((double)(curr - prev))/CLOCKS_PER_SEC);
        // global_singleton->get_window()->clear();

        clear_sprite.draw(*(global_singleton->get_window()), x, y, mode);

        Widget_manager::draw(0, 0, *(global_singleton->get_window()));
        
        global_singleton->get_window()->display();

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
void Widget_event_reciever::set_mouse(Window* window){
    this->mouse = Mouse(window);
}
