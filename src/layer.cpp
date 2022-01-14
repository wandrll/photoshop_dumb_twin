#include "canvas_window.hpp"


Layer::Layer (const int x, const int y, const int width, const int height) :
    Widget(x, y, width, height),
    texture(width, height),
    preview_texture(width, height),
    texture_to_draw(width, height)
{
    texture.clear();
    preview_texture.clear();
    texture_to_draw.clear();
}


void Layer::draw (const int x, const int y, Texture& window){
    
    update_texture_to_draw();

    Sprite sprite;
    sprite.set_texture(this->texture_to_draw);
    sprite.draw(window, this->x + x, this->y + y);

}

void Layer::load_image(const std::string name){
    Texture texture;
    texture.load_from_file(name);
    
    Sprite sprite(0, 0, this->width, this->height);
    sprite.set_texture(texture);
    sprite.set_size(this->width, this->height);
    
    

    sprite.draw(this->texture, 0, 0, Blend_mode(Blending::Factor::One, Blending::Factor::Zero));


    update_texture_to_draw();

}










Layer_manager_widget::Layer_manager_widget (const int x, const int y, const int width, const int height) : 
        Widget(x, y, width, height),
        Widget_manager(x, y, width, height),
        active_layer(0)
{
    this->widgets.push_back(new Widget_sprite(0, 0, width, height, Resource_type::CANVAS_BACKGROUND));
    this->layers.push_back(new Layer (0, 0, width, height));
}

    
Layer_manager_widget::~Layer_manager_widget (){
    for (int i = 0; i < layers.size(); i++){
        delete layers[i];
    }
}



bool Layer_manager_widget::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    if (is_in_widget(x, y, event.click)){

            Abstract_tool* curr_tool = global_singleton->get_tools()->get_tool(); 

            global_singleton->set_layer(this->get_active_layer());

            curr_tool->on_press(Vector(event.click.x - this->x - x, event.click.y - this->y - y));

            get_active_layer()->update_texture_to_draw();

            // global_singleton->get_layer()->update_texture_to_draw(curr_tool->merge_mode);

            this->focus = true;
        return true;
    }
    return false;
}

bool Layer_manager_widget::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (this->focus){
        printf("brrrrrrrrrrrrrrr\n");

            Abstract_tool* curr_tool = global_singleton->get_tools()->get_tool(); 


            curr_tool->on_move(Vector(event.move.begin_move.x - this->x - x, event.move.begin_move.y - this->y - y),
                                                   Vector(event.move.end_move.x   - this->x - x, event.move.end_move.y   - this->y - y));
            
            get_active_layer()->update_texture_to_draw();

            // global_singleton->get_layer()->update_texture_to_draw(curr_tool->merge_mode);

        return true;
    }
    
    return false;

}


bool Layer_manager_widget::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    Layer* layer = global_singleton->get_layer();
    if (layer != nullptr){ 
        
        Abstract_tool* curr_tool = global_singleton->get_tools()->get_tool(); 
        
        curr_tool->on_release(Vector(event.click.x - this->x - x, event.click.y - this->y - y));

        layer->merge_with_preview();

        layer->update_texture_to_draw();

        this->focus = false;
    }
    return true;

}

    
void Layer_manager_widget::draw (const int x, const int y, Texture& window){
    Widget_manager::draw (x, y, window);

    for (int i = 0; i < layers.size(); i++){
        layers[i]->draw(x + this->x, y + this->y, window);
    }


}


void Layer_manager_widget::add_layer (){
    this->layers.push_back(new Layer (0, 0, width, height));
}
void Layer_manager_widget::next_layer (){
    this->active_layer = (this->active_layer + 1) % this->layers.size();
}








Layer_controller_widget::Layer_controller_widget (const int x, const int y, const int width, const int height, Layer_manager_widget* layers) : 
        Widget(x, y, width, height),
        Widget_manager (x, y, width, height),
        layers(layers){
    Text_button* button1 = new Text_button (0, 0, 100, 30, "Add layer", 20,  Color(255, 255, 255), Color (0, 0 ,0));
    this->widgets.push_back(button1);

    Controller<Add_layer, Layer_manager_widget>* control1 = new Controller<Add_layer, Layer_manager_widget>(layers);
    button1->add_controller(control1); 


    Text_button* button2 = new Text_button (0, 40, 100, 30, "Next layer", 20,  Color(255, 255, 255), Color (0, 0 ,0));
    this->widgets.push_back(button2);

    Controller<Next_layer, Layer_manager_widget>* control2 = new Controller<Next_layer, Layer_manager_widget>(layers);
    button2->add_controller(control2);


}

