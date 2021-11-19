#ifndef PALETTE_HEADER
#define PALETTE_HEADER
#include "widget.hpp"
#include "top_bar.hpp"
#include "settings.hpp"
#include "slider.hpp"
#include "canvas_window.hpp"
#include "window_widget.hpp"



class Palette_widget : public Window_widget {
    public:

    Palette_widget (const int x, const int y, const int width, const int height);

    private:


};





class Saturation_and_lightness_picker : public Widget_manager {
    public:

    Saturation_and_lightness_picker (const int x, const int y, const int size, const double hue = 0);

    virtual void draw (const int x, const int y, Window& window);

    void set_hue(const double val){
        this->hue = val;
        update_palette();

    }


    void update_palette();

    double get_hue(){
        return this->hue;
    }
    
 

    private:
    double hue;

    Texture texture;



};

// class Hue_picker;

template<typename Func>
class Controller_saturation_and_lightness: public Controller {
    public:

    Controller_saturation_and_lightness (Saturation_and_lightness_picker* widget) :
        widget(widget)
    {}

    virtual void operator() (const Data_for_controller& data){
        if (widget){
            Func()(data, widget);
            // fc(win);
        }
    }

    private:
    Saturation_and_lightness_picker* widget;

};


class Hue_picker : public Widget_manager {
    public:

    Hue_picker (const int x, const int y, const int width, const int height, Saturation_and_lightness_picker* palette, const double hue = 0);

    virtual void draw (const int x, const int y, Window& window);

  

    private:
    double hue;
    double size_y;
    Texture texture;

};





class Brush_size_picker : public Horizontal_slider_bar {
    public:

    Brush_size_picker (const int x, const int y, const int width, const int height);

    private:

};


class Opacity_picker : public Horizontal_slider_bar {
    public:

    Opacity_picker (const int x, const int y, const int width, const int height);

    private:


};

#endif