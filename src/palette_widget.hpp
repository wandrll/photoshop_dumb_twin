#ifndef PALETTE_HEADER
#define PALETTE_HEADER
#include "widget.hpp"
#include "top_bar.hpp"
#include "settings.hpp"
#include "slider.hpp"
#include "canvas_window.hpp"
#include "window_widget.hpp"

extern Singleton* global_singleton;

class Palette_widget : public Widget_manager {
    public:

    Palette_widget (const int x, const int y, const int width, const int height);

    private:


};





class Saturation_and_lightness_picker : public Widget_manager {
    public:

    Saturation_and_lightness_picker (const int x, const int y, const int size, const double hue = 0);

    virtual void draw (const int x, const int y, Texture& window);

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


class Hue_picker : public Widget_manager {
    public:

    Hue_picker (const int x, const int y, const int width, const int height, Saturation_and_lightness_picker* palette, const double hue = 0);

    virtual void draw (const int x, const int y, Texture& window);

  

    private:
    double hue;
    double size_y;
    Texture texture;

};





class Brush_size_picker : public Slider_bar_with_text_box {
    public:

    Brush_size_picker (const int x, const int y, const int width, const int height, const char* name);

    private:

};


class Opacity_picker : public Slider_bar_with_text_box {
    public:

    Opacity_picker (const int x, const int y, const int width, const int height, const char* name);

    private:


};

#endif