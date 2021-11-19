#ifndef CANVAS_HEADER
#define CANVAS_HEADER

#include "window_widget.hpp"
#include "settings.hpp"
#include "top_bar.hpp"
#include "graphics_provider.hpp"
#include <memory>
class Brush;

class Canvas_field;

Color convert_color(float h, float s, float l, int opacity);

extern Brush* brush;

class Canvas_widget : public Window_widget {
    public:

    Canvas_widget (const int x, const int y, const int width, const int height);

    Canvas_field* get_canvas_field_ptr(){
        return this->field;
    }

    void load_image(const std::string name);


    private:

    Canvas_field* field;



};


class Brush{
    public:

    Brush (const int size, const Color color = {0,0,0}, const double max_size = 20) :
        brush_size(size),
        color(color),
        circle(0, 0, brush_size, color),
        max_size(max_size)
    {}


    void draw (const int x, const int y, Texture& texture){
        this->circle.set_position(x - this->brush_size/2, y - this->brush_size/2);
        this->circle.draw(texture);
    }

    void set_color(const Color& color){
        this->color = color;
        this->circle.set_color(color);
    }

    void set_opacity(const double opacity){
        int new_alpha = opacity * 255;
        printf("new alpha = %d\n", new_alpha);
        color.color.a = new_alpha;
        circle.set_color(color);
    }

    void set_size (const double size){
        this->brush_size = size * max_size;
        // printf("new size is %lg\n", this-)
        this->circle.set_size(this->brush_size);
    }

    void update_color(){
        color = convert_color(hue, saturation, light, color.color.a);
        circle.set_color(color);
        color.print();
    }

    double hue;
    double saturation;
    double light;

    
    private:
    int brush_size;
    double max_size;
    Color color;
    Circle circle;
    

};

class Saturation_and_lightness_picker;
class Brush_size_picker;

template<typename Func>
class Controller_brush: public Controller {
    public:

    Controller_brush (Brush* brush) :
        brush(brush)
    {}

    virtual void operator() (const Data_for_controller& data){
        if (brush){
            Func()(data, brush);
        }
    }

    private:

    Brush* brush;

};




class Canvas_field : public Widget {
    public:
    Canvas_field (const int x, const int y, const int width, const int height);

    virtual void draw (const int x, const int y, Window& window);
    // virtual bool handle_event(const int x, const int y, const Event& event);

    void load_image(const std::string name);



    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);
    

    private:

    Texture texture;


};







#endif