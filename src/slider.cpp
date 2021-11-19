#include "slider.hpp"

Slider::Slider(const int x, const int y, const int width, const int height, const int left_x_limit, const int top_y_limit, const int right_x_limit, const int bottom_y_limit) :
    Widget_manager(x, y, width, height),
    left_x_limit(left_x_limit),
    top_y_limit(top_y_limit),
    right_x_limit(right_x_limit),
    bottom_y_limit(bottom_y_limit)
    {
        printf("slider constructor called\n");
        
        Widget_sprite* sprite = new Widget_sprite(0, 0, width, height, Resource_type::SLIDER);

        this->widgets.push_back(sprite);

}


double mid (double a, double b, double c){
    // printf("%lg %lg %lg\n", a, b, c);
    if ((a >= b && b >= c) || (a <= b && b <= c)){
        // printf("%lg\n", b);
        return b;
    }

    if((b >= c && c >= a) || (b <= c && c <= a)){
        // printf("%lg\n", c);

        return c;
    }
        // printf("%lg\n", a);

    return a;

}



bool Slider::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    if (   this->x + x <= event.click.x && event.click.x <= this->width  + this->x + x 
        && this->y + y <= event.click.y && event.click.y <= this->height + this->y + y){
            this->is_active = true;
            return true;
    }

    return false;

}

bool Slider::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    this->is_active = false;
    return true;
}

bool Slider::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (this->is_active){

        int c_x = mid(left_x_limit, event.move.end_move.x - x, right_x_limit) - this->width/2;
        int c_y = mid(top_y_limit, event.move.end_move.y - y, bottom_y_limit) - this->height/2;

        this->set_coordinats({c_x, c_y});

        proceed_controllers(get_value());

        return true;
    }
    return false;
}
    


void Slider::proceed_controllers(const Data_for_controller& data){
    for (int i = 0; i < this->controller.size(); i++){
        (*this->controller[i])(Data_for_controller(get_value()));
    }

}



Horizontal_slider_bar::Horizontal_slider_bar (int x, int y, int width, int height) :
    Widget_manager (x, y, width, height){
        

        Widget_sprite* sprite = new Widget_sprite(height, 0, width - height*2, height, Resource_type::MID_SLIDER_BAR);
        this->widgets.push_back(sprite);

        Widget_sprite* left_sprite = new Widget_sprite(0, 0, height, height, Resource_type::LEFT_SLIDER_BAR);
        this->widgets.push_back(left_sprite);

        Widget_sprite* right_sprite = new Widget_sprite(width - height, 0, height, height, Resource_type::RIGHT_SLIDER_BAR);
        this->widgets.push_back(right_sprite);

        Slider* slider = new Slider(-height, -height/2, height * 2, height * 2,0, height/2, width, height/2); 
        this->slider = slider;
        this->widgets.push_back(slider);
}


