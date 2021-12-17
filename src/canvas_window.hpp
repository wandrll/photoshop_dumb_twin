#ifndef CANVAS_HEADER
#define CANVAS_HEADER

#include "window_widget.hpp"
#include "settings.hpp"
#include "top_bar.hpp"
#include "graphics_provider.hpp"
#include "tools.hpp"
#include "utils.hpp"
#include "singleton.hpp"

extern Singleton* global_singleton;


class Layer_manager_widget;



class Layer : public Widget {
    public:
    Layer (const int x, const int y, const int width, const int height);

    virtual void draw (const int x, const int y, Window& window);
    // virtual bool handle_event(const int x, const int y, const Event& event);

    void load_image(const std::string name);

    Texture* get_active_texture(){
        return &texture;
    }

    Texture* get_preview_texture(){
        return &preview_texture;
    }

    void merge_with_preview (){
        Sprite sprite;
        sprite.set_texture(preview_texture);

        sprite.draw(texture);
        preview_texture.clear();
    }


    void update_texture_to_draw (){
        // texture_to_draw.clear

        Sprite sprite;
        sprite.set_texture(texture);
        sprite.draw(texture_to_draw, 0, 0, Blend_mode(Blending::Factor::One, Blending::Factor::Zero));

        sprite.set_texture(preview_texture);
        sprite.draw(texture_to_draw);

    }
 

    Vector get_size (){
        return texture.get_size();
    }

    const Color* get_array (){
        return texture.get_array();
    } 


    private:

    Texture texture;
    Texture preview_texture;


    Texture texture_to_draw;
};




class Layer_manager_widget : public Widget_manager {
    public:
    Layer_manager_widget (const int x, const int y, const int width, const int height);

    ~Layer_manager_widget ();
    
    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);
    
    virtual void draw (const int x, const int y, Window& window);


    void add_layer ();
    void next_layer ();

    

    Layer* get_active_layer(){
        if (active_layer == -1){
            return nullptr;
        }
        return layers[active_layer];
    }

    int get_active_layer_int(){
        return active_layer;
    }

    private:

    int active_layer;

    std::vector<Layer*> layers;

};


class Canvas_widget : public Window_widget {
    public:

    Canvas_widget (const int x, const int y, const int width, const int height);

    // Layer* get_canvas_field_ptr(){
    //     return this->field;
    // }

    void load_image(const std::string name);

    Layer* get_active_layer(){
        return field->get_active_layer();
    }

    private:

    Layer_manager_widget* field;



};





class Next_layer{
    public:

    void operator() (const Data_for_controller& data, Layer_manager_widget* widget){
       
        widget->next_layer();
        printf ("active layer = %d\n", widget->get_active_layer_int());
    }

};

class Add_layer{
    public:

    void operator() (const Data_for_controller& data, Layer_manager_widget* widget){
       
        widget->add_layer();

    }

};



class Layer_controller_widget : public Widget_manager {
    public:

    Layer_controller_widget (const int x, const int y, const int width, const int height, Layer_manager_widget* layers);


    private:

    Layer_manager_widget* layers;

};








#endif