#include "canvas_window.hpp"


void Brush::draw (const int x, const int y, Layer* layer){
    this->circle.set_position(x - this->brush_size/2, y - this->brush_size/2);
    this->circle.draw(*(layer->get_texture()));
}




Layer::Layer (const int x, const int y, const int width, const int height) :
    Widget(x, y, width, height),
    texture(width, height)
{
    texture.clear();
}


void Layer::draw (const int x, const int y, Window& window){
    
    Sprite sprite;
    sprite.set_texture(this->texture);
    
    
    sprite.draw(window, this->x + x, this->y + y);
}

void Layer::load_image(const std::string name){
    Texture texture;
    texture.load_from_file(name);
    
    Sprite sprite(0, 0, this->width, this->height);
    sprite.set_texture(texture);

    sprite.set_size(this->width, this->height);

    sprite.draw(this->texture);
}


static void draw_continuously (const Move& move_coords, Layer* layer, const int offset_x, const int offset_y){
    Vector move = move_coords.get_move_vector();
    
    int x = move_coords.end_move.x - move_coords.begin_move.x;
    int y = move_coords.end_move.y - move_coords.begin_move.y;

    int cnt = std::max(abs(x), abs(y));
    // printf("cnt = %d\n", cnt);
    if(cnt < 1){
        return;
    }

    double deltax = ((double)x) / ((double)cnt);
    double deltay = ((double)y) / ((double)cnt);


    for (int i = 0; i < cnt; i++){
        brush->draw(move_coords.begin_move.x + i * deltax - offset_x, move_coords.begin_move.y + i * deltay - offset_y, layer);
    }





}







Layer_manager_widget::Layer_manager_widget (const int x, const int y, const int width, const int height) : 
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

            brush->draw(event.click.x - this->x - x, event.click.y - this->y - y, this->get_active_layer());
            this->is_active = true;
        return true;
    }
    return false;
}

bool Layer_manager_widget::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (this->is_active){
        printf("brrrrrrrrrrrrrrr\n");
            brush->draw(event.move.begin_move.x - this->x - x, event.move.begin_move.y - this->y - y, this->get_active_layer());

            draw_continuously(event.move, this->get_active_layer(), this->x + x, this->y + y);
        return true;
    }
    
    return false;

}


bool Layer_manager_widget::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    this->is_active = false;
    return true;
}

    
void Layer_manager_widget::draw (const int x, const int y, Window& window){
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


class Text_button : public Rectangle_Button {
    public:
    Text_button (const int x, const int y, const int size_x, const int size_y, const char* line, const int font_size, const Color& text_color, const Color& background_color) :
            Rectangle_Button (x, y, size_x, size_y){
        Widget_manager* widget = new Widget_manager(0, 0, size_x, size_y);

        Rectangle_widget* back = new Rectangle_widget(0, 0, size_x, size_y, background_color);
        Text_widget* text = new Text_widget (0, 0, line, font_size, text_color);

        widget->register_widget(back);
        widget->register_widget(text);

        this->regular_button = widget;
    }


};






Layer_controller_widget::Layer_controller_widget (const int x, const int y, const int width, const int height, Layer_manager_widget* layers) : 
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

