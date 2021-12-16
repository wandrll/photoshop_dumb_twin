#ifndef WINDOW_WIDGET_HEADER
#define WINDOW_WIDGET_HEADER

#include "widget.hpp"
#include "top_bar.hpp"
#include "sprite_widget.hpp"
#include "settings.hpp"


class Window_widget : public Widget_manager{
    public:
    Window_widget (const int x, const int y, const int width, const int height);

    virtual ~Window_widget ();

    virtual void draw (const int x, const int y, Window& window);
    // virtual bool handle_event(const int x, const int y, const Event& event);


    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event);
    
    private:

    Top_bar* bar;

    
};






#endif