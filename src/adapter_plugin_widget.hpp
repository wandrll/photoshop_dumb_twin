#ifndef ADAPTER_WIDGET_HEADER
#define ADAPTER_WIDGET_HEADER

#include "widget.hpp"
#include "../plugins/plugin_std.hpp"
#include "button_widget.hpp"
#include "window_widget.hpp"


namespace Adapter{


class Widget : virtual public ::Widget{
    public:

    Widget (const int x, const int y, const int width, const int height, PUPPY::Widget* widget) : 
        ::Widget(x, y, width, height),
        plugin_widget(widget)
    {
        // std::cout << "adapter constructor called\n";
    }

    virtual ~Widget (){
        delete plugin_widget;
    }


    void set_plugin_widget(PUPPY::Widget* plugin_widget){
        this->plugin_widget = plugin_widget;
    }

    PUPPY::Widget* get_plugin_widget (){
        return plugin_widget;
    }

    protected:

    PUPPY::Widget* plugin_widget;
};


class Widget_manager : public Widget, public ::Widget_manager{
    public:
    Widget_manager (const int x, const int y, const int width, const int height, PUPPY::Widget* widget) :
        ::Widget(x, y, width, height),
        ::Widget_manager(x, y, width, height),
        Widget(x, y, width, height, widget)
    {}
    
    virtual void draw (const int x, const int y, Texture& window) override;
    
    virtual void on_tick (double dt) override;


    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event) override;
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event) override;

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event) override;
    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event) override;

    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event) override;

    virtual bool on_key_press (const Event::Press_key& event) override;
    virtual bool on_key_release (const Event::Release_key& event) override;
    virtual bool on_text_enter (const Event::Text_enter& event) override;



    virtual ~Widget_manager (){
        
    }




};



// template<typename type>
// class Adapter_class : public Widget, public ::type{
//     public:
    
//     Adapter_class(int x, int y, int width, int height, PUPPY::Widget* widget) :
//         ::Widget(x, y, width, height),
//         Widget(x, y, width, height, widget),
//         ::type(x, y, width, height)
//     {}

//     virtual ~Adapter_class (){

//     }

//     private:


// };




class Text_Button : public Widget, public ::Text_button{
    public:
    
    Text_Button(int x, int y, int width, int height, const char *caption, PUPPY::Widget* widget) :
        ::Widget(x, y, width, height),
        Widget(x, y, width, height, widget),
        ::Text_button(x, y, width, height, caption, height - 2 , ::Color(186, 30, 48), ::Color(64, 53, 54))
    {}

    


    virtual ~Text_Button (){

    }

    private:


};

class Window_widget : public Widget, public ::Window_widget{
    public:
    
    Window_widget(int x, int y, int width, int height, PUPPY::Widget* widget) :
        ::Widget(x, y, width, height),
        Widget(x, y, width, height, widget),
        ::Window_widget(x, y, width, height)
    {}

    virtual ~Window_widget (){
        
    }

    private:


};





}






#endif