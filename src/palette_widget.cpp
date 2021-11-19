#include "palette_widget.hpp"
#include "window_widget.hpp"



double  hue2rgb(float p, float q, float t){
            if(t < 0) t += 1;
            if(t > 1) t -= 1;
            if(6 * t < 1) return p + (q - p) * 6 * t;
            if(2 * t < 1) return q;
            if(3 * t < 2) return p + (q - p) * (2./3. - t) * 6;
            return p;
        }


Color convert_color(float h, float s, float l, int opacity){
    float R = 0;
    float G = 0;
    float B = 0;

    if(s == 0){
        R = G = B = l; // achromatic
    }else{
        

        float q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        float p = 2 * l - q;
        R = hue2rgb(p, q, ((float)h) + 1./3.);
        G = hue2rgb(p, q, ((float)h));
        B = hue2rgb(p, q, ((float)h) - 1./3.);
    }

    Color result(R*255, G*255, B*255);
    result.color.a = opacity;
    return result;
}


class Update_brush_light_and_saturation{
    public:

    void operator() (const Data_for_controller& data, Brush* brush){
        printf("update brush color light = %lg saturation = %g\n", data.vector.y, data.vector.x);
        
        brush->light = data.vector.y;
        brush->saturation = data.vector.x;

        brush->update_color();


    }

};




Palette_widget::Palette_widget (const int x, const int y, const int width, const int height) : 
    Window_widget(x, y, width, height){
    

    Saturation_and_lightness_picker* picker = new Saturation_and_lightness_picker (10 , 10 + TOP_BAR_HEIGHT , 300);
    this->widgets.push_back(picker);

    Hue_picker* hue_picker = new Hue_picker (10, 20 + TOP_BAR_HEIGHT + 300, 300, 30, picker);
    this->widgets.push_back(hue_picker);

    Brush_size_picker* size_picker = new Brush_size_picker (10, TOP_BAR_HEIGHT + 300 + 75, 300, 10);
    this->widgets.push_back(size_picker); 

    Opacity_picker* opacity_picker = new Opacity_picker(10, TOP_BAR_HEIGHT + 405, 300, 10);
    this->widgets.push_back(opacity_picker);

}





Saturation_and_lightness_picker::Saturation_and_lightness_picker (const int x, const int y, const int size, const double hue) :
    Widget_manager(x, y, size, size),
    hue(hue),
    texture(size, size)
    {

        Controller_brush<Update_brush_light_and_saturation>* contr = new Controller_brush<Update_brush_light_and_saturation>(brush); 

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

    void operator() (const Data_for_controller& data, Brush* brush){
        printf("update brush color\n");
        
        brush->hue = data.vector.x;
        brush->update_color();

    }

};



Hue_picker::Hue_picker (const int x, const int y, const int width, const int height, Saturation_and_lightness_picker* palette, const double hue) :
    Widget_manager(x, y, width, 0),
    hue(hue),
    texture(width, height),
    size_y(height)
    {
        Slider* slider = new Slider(-SLIDER_SIZE/2, -SLIDER_SIZE/2 + height/2, SLIDER_SIZE, SLIDER_SIZE,0, height/2, width, height/2);        
        Controller_brush<Update_brush_hue>* contr = new Controller_brush<Update_brush_hue>(brush); 
        slider->add_controller(contr);
        
        Controller_saturation_and_lightness<Set_new_hue>* contr2 = new Controller_saturation_and_lightness<Set_new_hue>(palette);
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


class Set_brush_size{
    public:

    void operator() (const Data_for_controller& data, Brush* brush){
        printf("set new size\n");
        brush->set_size(data.vector.x);
    }

};


Brush_size_picker::Brush_size_picker (const int x, const int y, const int width, const int height) :
    Horizontal_slider_bar(x, y, width, height){
        this->slider->add_controller(new Controller_brush<Set_brush_size>(brush));
}


class Set_brush_opacity{
    public:

    void operator() (const Data_for_controller& data, Brush* brush){
        printf("set new opacity\n");
        brush->set_opacity(data.vector.x);
    }
    
};


Opacity_picker::Opacity_picker (const int x, const int y, const int width, const int height) :
    Horizontal_slider_bar(x, y, width, height){
        this->slider->add_controller(new Controller_brush<Set_brush_opacity>(brush));
}


