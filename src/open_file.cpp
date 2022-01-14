#include "open_file.hpp"


Open_file_widget::Open_file_widget (const int x, const int y, const int width, const int height, Widget_manager* widget) :
        Widget(x, y, width, height),

        Widget_event_reciever(x, y, width, height),
        parent(this)
{
    Rectangle_widget* background = new Rectangle_widget (0, 0, width, height, Color(255, 255, 255, 0), BORDER_THICKNESS, BORDER_COLOR);
    this->widgets.push_back(background);

    Text_field* text_field = new Text_field(10, 10 + TOP_BAR_HEIGHT, width - 20, 30);

    Controller<Open, Widget_manager>* control = new Controller<Open, Widget_manager>(widget);
    text_field->add_controller(control);

    Controller<Close_widget, Widget_manager>* close_widget = new Controller<Close_widget, Widget_manager>(this);
    text_field->add_controller(close_widget); 

    this->widgets.push_back(text_field);



    Button_ok* button = new Button_ok (width/4, height/2, width/2, height/4);

    Controller<Proceed_controllers, Text_field>* button_control = new Controller<Proceed_controllers, Text_field>(text_field);
    button->add_controller(button_control);

    
    this->widgets.push_back(button);

}

