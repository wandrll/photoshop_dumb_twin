#include "top_bar.hpp"





Top_bar::Top_bar(const int x, const int y, const int width, const int height, Widget* widget):
        Widget(x, y, width, height),
        Widget_manager(x, y, width, height)
    {

        double border = ((double)height) /10;
        Widget_sprite* exit = new Widget_sprite(0, 0, height- 2 * border, height - 2 * border ,Resource_type::EXIT);
        Widget_sprite* hover_exit = new Widget_sprite(0, 0, height- 2 * border, height - 2 * border ,Resource_type::EXIT_HOVER);



        Hover_rectangle_button* button_close = new Hover_rectangle_button(width - height + border, border, height - 2 * border, height - 2 * border);
        button_close->set_normal_button(exit);
        button_close->set_hovered_button(hover_exit);

        Controller<Close_widget, Widget>* control1 = new Controller<Close_widget, Widget>(widget);   
           
        button_close->add_controller(control1);

        // printf("%d\n", height);





        Widget_sprite* bar = new Widget_sprite(height, 0, width - 2 * height, height, Resource_type::MENU_BAR);


        Widget_sprite* left = new Widget_sprite(0, 0, height, height, Resource_type::LEFT_MENU_BAR);
        
        Widget_sprite* right = new Widget_sprite(width - height, 0, height, height, Resource_type::RIGHT_MENU_BAR);




        Dragable_top_bar_zone* move_zone = new Dragable_top_bar_zone(0, 0, width - height + border, height);
        Controller<Move_widget, Widget>* control2 = new Controller<Move_widget, Widget>(widget);





        // move_zone->register_widget(bar);
        move_zone->set_controller(control2);
        this->widgets.push_back(bar);
        this->widgets.push_back(left);
        this->widgets.push_back(right);
        
        
        this->widgets.push_back(move_zone);
        this->widgets.push_back(button_close);



}




Dragable_top_bar_zone::Dragable_top_bar_zone (const int x, const int y, const int size_x, const int size_y) :
    Widget(x, y, size_x, size_y),
    Widget_manager(x, y, size_x, size_y),
    controller(NULL)
{}

Dragable_top_bar_zone::~Dragable_top_bar_zone (){
    delete this->controller;
}




bool Dragable_top_bar_zone::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    if (is_in_widget(x, y, event.click)){
            this->focus = true;
            return true;
    }
    return false;
}

bool Dragable_top_bar_zone::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    this->focus = false;
    return true;
}

bool Dragable_top_bar_zone::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (this->focus){
        std::cout << "from "<< event.move.begin_move.x <<" "<< event.move.begin_move.y << std::endl;
        (*this->controller)(Data_for_controller(event.move.get_move_vector()));

        return true;
    }
    return false;
}