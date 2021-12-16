#ifndef TEXT_FIELD_HEADER
#define TEXT_FIELD_HEADER

#include "widget.hpp"
#include "graphics_provider.hpp"
#include <string>

class Text_field : public Widget{
    public:

    Text_field (const int x, const int y, const int width, const int height, const char* line = "Test");

    virtual void draw (const int x, const int y, Window& window);

    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_keyboard (const Event::Keyboard_event& event);


    virtual ~Text_field ();


    void add_controller (Abstract_controller* control){
        controllers.push_back(control);
    }

    void proceed_controllers (){
        int size = controllers.size();
        Data_for_controller const_ptr(content.c_str());
        for (int i = 0; i < size; i++){
            (*controllers[i])(const_ptr);
        }
    }

    const char* get_line (){
        return content.c_str();
    }

    void set_line (const std::string& line){
        content = line;
        cursor_index = 0;
        update_cursor_position();
        update_text();
    }

    private:


    void add_character (const Event::Keyboard_event& event);
    void delete_character ();
    void move_cursor_left ();
    void move_cursor_right ();

    bool handle_hotkeys (const Event::Keyboard_event& event);


    void update_cursor_position ();
    void update_text ();



    std::string content;
    int cursor_index;
    int cursor_position;

    Text text;
    Rectangle background;

    std::vector<Abstract_controller*> controllers;


};

class Proceed_controllers{
    public:

    void operator() (const Data_for_controller& data, Text_field* widget){
        widget->proceed_controllers();
    }

};




#endif