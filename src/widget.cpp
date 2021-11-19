#include "widget.hpp"

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