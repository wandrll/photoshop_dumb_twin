#ifndef DROP_DOWN_WIDGET
#define DROP_DOWN_WIDGET

#include "widget.hpp"
#include "widget_graphics.hpp"
#include "button_widget.hpp"


class Button_list : public Widget_manager{
    public:

    Button_list (const int x, const int y, const int width, const int height);
    
    
    void add_button (Rectangle_Button* button);
    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);

    virtual void draw (const int x, const int y, Texture& window){
        if (is_accept_events){
            Widget_manager::draw(x, y, window);
        }
    }

    private:




};



class Drop_down_widget : public Widget_manager{
    public:

    Drop_down_widget (const int x, const int y, const int width, const int height, const char* name);

    void add_button (Rectangle_Button* button);

    private:
    Button_list* list;

};





class File_sub_widget : public Drop_down_widget{
    public:
    File_sub_widget(const int x, const int y, const int width, const int height);
};








#endif