#ifndef LEVEL_WIDGET_HEADER
#define LEVEL_WIDGET_HEADER

#include "window_widget.hpp"
#include "sprite_widget.hpp"
#include "slider.hpp"

class Slider_for_field : public Slider{
    public:
    Slider_for_field (const int x, const int y, const int width, const int height, const int left_x_limit, const int top_y_limit, const int right_x_limit, const int bottom_y_limit, std::vector<Slider_for_field*>* ptr);
    ~Slider_for_field ();

    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event);
    private:
    std::vector<Slider_for_field*>* vector_ptr;
};


class Spline_widget : public Window_widget{
    public:
    Spline_widget (const int x, const int y, const int width, const int height);
};



class Spline_field : public Widget_manager{
    public:
    Spline_field (const int x, const int y, const int width, const int height, const int cnt);
    virtual void draw (const int x, const int y, Texture& window);

    void sort_sliders();
    void update_texture();
    void update_points();
    
    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event);

    private:

    

    std::vector<Slider_for_field*> sliders;
    std::vector<Vector> points;
    Texture texture;


};


class Move_slider{
    public:

    void operator() (const Data_for_controller& data, Spline_field* widget){
        widget->sort_sliders();
        widget->update_points();
        widget->update_texture();

    }

};







#endif