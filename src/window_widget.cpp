#include "window_widget.hpp"



Window_widget::Window_widget (const int x, const int y, const int width, const int height) :
    Widget_manager(x, y, width, height){
        this->set_ability_to_move(true);

        this->bar = new Top_bar(0, 0, width, TOP_BAR_HEIGHT, this);

        Widget_sprite* mid = new Widget_sprite(TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, width - TOP_BAR_HEIGHT * 2, height - TOP_BAR_HEIGHT * 2, Resource_type::MID_WINDOW);
        this->widgets.push_back(mid);

        Widget_sprite* left = new Widget_sprite(0, TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, height - TOP_BAR_HEIGHT * 2, Resource_type::LEFT_WINDOW);
        this->widgets.push_back(left);

        Widget_sprite* right = new Widget_sprite(width - TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, height - TOP_BAR_HEIGHT * 2, Resource_type::RIGHT_WINDOW);
        this->widgets.push_back(right);

        Widget_sprite* bottom = new Widget_sprite(TOP_BAR_HEIGHT, height - TOP_BAR_HEIGHT, width - TOP_BAR_HEIGHT*2, TOP_BAR_HEIGHT, Resource_type::BOTTOM_WINDOW);
        this->widgets.push_back(bottom);

        Widget_sprite* left_bottom = new Widget_sprite(0, height - TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, Resource_type::LEFT_BOTTOM_WINDOW);
        this->widgets.push_back(left_bottom);

        Widget_sprite* right_bottom = new Widget_sprite(width - TOP_BAR_HEIGHT, height - TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, TOP_BAR_HEIGHT, Resource_type::RIGHT_BOTTOM_WINDOW);
        this->widgets.push_back(right_bottom);



}

void Window_widget::draw (const int x, const int y, Texture& window){
    Widget_manager::draw(x, y, window);
    this->bar->draw(x + this->x, y + this->y, window);
}

bool Window_widget::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){

    bool res = Widget_manager::on_mouse_pressed_move(x, y, event);
    if(res){
        this->focus = res;
        return true;
    }

    res = this->bar->on_mouse_pressed_move(x + this->x, y + this->y, event);
    this->focus = res;
    return res;
}

Window_widget::~Window_widget (){
    delete this->bar;
}


bool Window_widget::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    bool res = bar->on_mouse_press(x + this->x, y + this->y, event);
    if(res){
        this->focus = true;
        return true;
    }
    this->focus = Widget_manager::on_mouse_press(x, y , event);
    return this->focus;
}

bool Window_widget::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    bool res = bar->on_mouse_release(x + this->x, y + this->y, event);
    // if(res){
        // return true;
    // }
    return Widget_manager::on_mouse_release(x, y , event) | res;
}


bool Window_widget::on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event){
    bool res = bar->on_mouse_released_move(x + this->x, y + this->y, event);
    this->focus = false;
    if(res){
        return true;
    }
    return Widget_manager::on_mouse_released_move(x, y , event);
}
    