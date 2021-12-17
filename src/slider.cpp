#include "slider.hpp"
#include "settings.hpp"
#include "widget_graphics.hpp"

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
            this->focus = true;
            return true;
    }

    return false;

}

bool Slider::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    this->focus = false;
    return true;
}

bool Slider::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (this->focus){

        int c_x = mid(left_x_limit, event.move.end_move.x - x, right_x_limit) - this->width/2;
        int c_y = mid(top_y_limit, event.move.end_move.y - y, bottom_y_limit) - this->height/2;

        this->set_coordinats({c_x, c_y});

        proceed_controllers();

        return true;
    }
    return false;
}
    


void Slider::proceed_controllers(){
    Data_for_controller data(get_value());
    for (int i = 0; i < this->controller.size(); i++){
        (*this->controller[i])(data);
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


class Set_value_in_slyder{
    public:

    void operator() (const Data_for_controller& data, Slider* slider){
        double value = ((double)atoi((const char*)(data.const_pointer))) / 100.;

        if (value > 1){
            value = 1;
        }
        if (value < 0){
            value = 0;
        }

        slider->set(Vector(value, 0));
        slider->proceed_controllers();
    }
    
};


class Set_line_in_text_field_from_slider{
    public:

    void operator() (const Data_for_controller& data, Text_field* text_field){
        int value = data.vector.x * 100;
        
        text_field->set_line(std::to_string(value));

    }
    
};




Slider_bar_with_text_box::Slider_bar_with_text_box (const int x, const int y, const int width, const int height, const char* name) :
        Widget_manager (x, y, width, height){
    
        
    Rectangle_widget* frame = new Rectangle_widget (0, 0, width, height, Color(0, 0, 0, 0), 2, Color(0 ,0 ,0, 255));
    this->widgets.push_back(frame);



    Text_widget* text = new Text_widget (0 ,-5 , name, 2 * height / 3, FONT_COLOR);
    text->centering(0, width); 
    this->widgets.push_back(text);

    Horizontal_slider_bar* slider_bar = new Horizontal_slider_bar (0, 2 * height / 3, width - 100, height / 3);
    Text_field* text_field = new Text_field (width - 90, 0, 90, height, "0");

    Controller<Set_value_in_slyder, Slider>* control = new Controller<Set_value_in_slyder, Slider>(slider_bar->get_slider()); 
    text_field->add_controller(control);

    Controller<Set_line_in_text_field_from_slider, Text_field>* control2 = new Controller<Set_line_in_text_field_from_slider, Text_field>(text_field); 
    slider_bar->add_controller(control2);


    this->widgets.push_back(slider_bar);
    this->widgets.push_back(text_field);

    this->slider_bar = slider_bar;
    this->text_field = text_field;

}
