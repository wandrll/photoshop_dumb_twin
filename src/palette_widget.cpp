#include "palette_widget.hpp"
#include "window_widget.hpp"
#include "widget_graphics.hpp"


class Update_light_and_saturation{
    public:

    void operator() (const Data_for_controller& data, Singleton* global_singl){
        printf("update brush color light = %lg saturation = %g\n", data.vector.y, data.vector.x);
        
        global_singl->set_lightness(data.vector.y);
        global_singl->set_saturation(data.vector.x);
        global_singl->update_color();

        // brush->light = data.vector.y;
        // brush->saturation = data.vector.x;

        // brush->update_color();


    }

};




Palette_widget::Palette_widget (const int x, const int y, const int width, const int height) : 
    Widget_manager(x, y, width, height){
        
    Text_widget* pallete_name = new Text_widget (width/2 - 60, 5, "Colour picker", 20, Color(240, 97,86));
    this->widgets.push_back(pallete_name);

    Saturation_and_lightness_picker* picker = new Saturation_and_lightness_picker (10 , 40 , width - 20);
    this->widgets.push_back(picker);

    Hue_picker* hue_picker = new Hue_picker (10, 40 + width - 20 + 4, width - 20, 30, picker);
    this->widgets.push_back(hue_picker);

    // Text_widget* size_name = new Text_widget (width/2 - 30, 60 + width, "Size", 20, Color(240, 97,86));
    // this->widgets.push_back(size_name);

    Brush_size_picker* size_picker = new Brush_size_picker (10, 85 + width, width - 20, 30, "Size");
    this->widgets.push_back(size_picker); 

    // Text_widget* alpha_name = new Text_widget (width/2 - 70, 100 + width, "Alpha channel", 20, Color(240, 97,86));
    // this->widgets.push_back(alpha_name);



    Opacity_picker* opacity_picker = new Opacity_picker(10, 130 + width, width - 20, 30, "Alpha channel");
    this->widgets.push_back(opacity_picker);

}





Saturation_and_lightness_picker::Saturation_and_lightness_picker (const int x, const int y, const int size, const double hue) :
    Widget_manager(x, y, size, size),
    hue(hue),
    texture(size, size)
    {

        Controller<Update_light_and_saturation, Singleton>* contr = new Controller<Update_light_and_saturation, Singleton>(global_singleton); 

        Slider* slider = new Slider(0, 0, SLIDER_SIZE, SLIDER_SIZE, 0, 0, size, size);
        slider->add_controller(contr);

        this->widgets.push_back(slider);

        update_palette();   
}



void Saturation_and_lightness_picker::draw (const int x, const int y, Window& window){
    
    Sprite sprite;
    sprite.set_texture(this->texture);
    
    sprite.draw(window, this->x + x, this->y + y);

    Widget_manager::draw(x, y, window);
}



void Saturation_and_lightness_picker::update_palette(){
    Rectangle rect;

    const int size = 100;
    const float step = 1./((float)size);

    float size_x = ((float)this->width)/((float)size); 
    float size_y = ((float)this->height)/((float)size); 

    rect.set_size(size_x + 1, size_y + 1);
    
    float x_pos = 0;
    float y_pos = 0;

    float currx = 0;
    float curry = 0;

    for (int i = 0; i < size; i++){
        for (int j =0; j < size; j++){
            rect.set_position(x_pos, y_pos);

            Color col = convert_color(hue, currx, curry, 255);
            rect.set_color(col);

            rect.draw(texture);

            currx += step;
            x_pos += size_x; 
        }
        curry += step;
        y_pos += size_y;

        currx = 0;
        x_pos = 0;

    }
}



class Set_new_hue{
    public:

    void operator() (const Data_for_controller& data, Saturation_and_lightness_picker* widget){
        widget->set_hue(data.vector.x);
    }

};


class Update_brush_hue{
    public:

    void operator() (const Data_for_controller& data, Singleton* global_singl){
        printf("update brush color\n");
        
        global_singleton->set_hue(data.vector.x);
        global_singleton->update_color();

        // brush->hue = data.vector.x;
        // brush->update_color();

    }

};



Hue_picker::Hue_picker (const int x, const int y, const int width, const int height, Saturation_and_lightness_picker* palette, const double hue) :
    Widget_manager(x, y, width, 0),
    hue(hue),
    texture(width, height),
    size_y(height)
    {
        Slider* slider = new Slider(-SLIDER_SIZE/2, -SLIDER_SIZE/2 + height/2, SLIDER_SIZE, SLIDER_SIZE,0, height/2, width, height/2);        
        Controller<Update_brush_hue, Singleton>* contr = new Controller<Update_brush_hue, Singleton>(global_singleton); 
        slider->add_controller(contr);
        
        Controller<Set_new_hue, Saturation_and_lightness_picker>* contr2 = new Controller<Set_new_hue, Saturation_and_lightness_picker>(palette);
        slider->add_controller(contr2); 
        
        this->widgets.push_back(slider);

        Rectangle rect;
        rect.set_size(1, height);

        for (int i = 0; i < width; i++){
            rect.set_position(i, 0);

            Color col = convert_color(((double)i)/((double)width), 1, 0.5, 255);
            rect.set_color(col);

            rect.draw(texture);
        }
        
}

void Hue_picker::draw (const int x, const int y, Window& window){
    
    Sprite sprite;
    sprite.set_texture(this->texture);
    
    sprite.draw(window, this->x + x, this->y + y);

    Widget_manager::draw(x, y, window);
}


class Set_size{
    public:

    void operator() (const Data_for_controller& data, Singleton* global_singl){
        printf("set new size\n");
        global_singl->set_size(data.vector.x);
    }

};


Brush_size_picker::Brush_size_picker (const int x, const int y, const int width, const int height, const char* name) :
    Slider_bar_with_text_box(x, y, width, height, name){
        this->slider_bar->add_controller(new Controller<Set_size, Singleton>(global_singleton));
}


class Set_opacity{
    public:

    void operator() (const Data_for_controller& data, Singleton* global_singl){
        printf("set new opacity\n");
        global_singl->set_opacity(data.vector.x);
        global_singl->update_color();

    }
    
};


Opacity_picker::Opacity_picker (const int x, const int y, const int width, const int height, const char* name) :
    Slider_bar_with_text_box(x, y, width, height, name){
        this->slider_bar->add_controller(new Controller<Set_opacity, Singleton>(global_singleton));
}


