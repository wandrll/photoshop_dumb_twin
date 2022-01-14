#include "drop_down_box.hpp"
#include "settings.hpp"
#include "open_file.hpp"
#include "singleton.hpp"

extern Singleton* global_singleton;

Drop_down_widget::Drop_down_widget (const int x, const int y, const int width, const int height, const char* name):
    Widget(x, y, width, height),
    Widget_manager(x, y, width, height),
    list(new Button_list(x, y + height, 0, 0))
{
    Rectangle_widget* rect = new Rectangle_widget(0, 0, width, height, BUTTON_COLOR, BORDER_THICKNESS, BORDER_COLOR);
    this->widgets.push_back(rect);

    Text_button* button = new Text_button (0, 0, width, height, name, height, FONT_COLOR, BUTTON_COLOR);
    this->widgets.push_back(button);
    
    Controller<Set_accept_events, Button_list>* control = new Controller<Set_accept_events, Button_list>(list);
    button->add_controller(control);


    this->widgets.push_back(list);
    // Text_widget* text = new Text_widget (3, -3, name, height, FONT_COLOR);
    // this->widgets.push_back(text);
}


void Drop_down_widget::add_button (Rectangle_Button* button){
    list->add_button(button);
}


Button_list::Button_list (const int x, const int y, const int width, const int height):
    Widget(x, y, width, height),
    Widget_manager(x, y, width, height){
    this->is_accept_events = false;
}


bool Button_list::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    bool res = Widget_manager::on_mouse_press (x, y, event);
    if (!res){
        this->is_accept_events = false;
    }

    return res;
}


void Button_list::add_button (Rectangle_Button* button){
    this->widgets.push_back(button);
    Controller<Set_refuse_events, Button_list>* control = new Controller<Set_refuse_events, Button_list>(this); 
    button->add_controller(control);
}


class Open_open_file_widget{
    public:

    void operator() (const Data_for_controller& data, Widget* widget){
        Open_file_widget new_widget(100, 100, 500, 500, global_singleton->get_main_widget());

        new_widget.run();
    }

};



File_sub_widget::File_sub_widget(const int x, const int y, const int width, const int height) :
    Widget(x, y, width, height),

    Drop_down_widget(x, y, width, height, "File"){
        Text_button* button1 = new Text_button (0,0, 300, height, "Open file", height, FONT_COLOR, BUTTON_COLOR);
        Controller<Open_open_file_widget, File_sub_widget>* control = new Controller<Open_open_file_widget, File_sub_widget>(this);
        button1->add_controller(control); 
        add_button(button1);
    }


