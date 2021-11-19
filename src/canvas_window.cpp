
#include "sprite_widget.hpp"
#include "button_widget.hpp"
#include "palette_widget.hpp"
#include "canvas_window.hpp"




Canvas_widget::Canvas_widget (const int x, const int y, const int width, const int height) :
        Window_widget (x, y, width, height)
    {
    this->set_ability_to_move(true);
 
    Canvas_field* field = new Canvas_field (BORDER_THICKNESS, TOP_BAR_HEIGHT + BORDER_THICKNESS, width - 2 * BORDER_THICKNESS, height - TOP_BAR_HEIGHT - 2*BORDER_THICKNESS);
    this->field = field;
    this->widgets.push_back(field);
}

void Canvas_widget::load_image(const std::string name){
    this->field->load_image(name);
}



Canvas_field::Canvas_field (const int x, const int y, const int width, const int height) :
    Widget(x, y, width, height),
    texture(width, height)
{}


void Canvas_field::draw (const int x, const int y, Window& window){
    
    Sprite sprite;
    sprite.set_texture(this->texture);
    
    
    sprite.draw(window, this->x + x, this->y + y);
}

void Canvas_field::load_image(const std::string name){
    Texture texture;
    texture.load_from_file(name);
    
    Sprite sprite(0, 0, this->width, this->height);
    sprite.set_texture(texture);

    sprite.set_size(this->width, this->height);

    sprite.draw(this->texture);
}


static void draw_continuously (const Move& move_coords, Texture& texture, const int offset_x, const int offset_y){
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
        brush->draw(move_coords.begin_move.x + i * deltax - offset_x, move_coords.begin_move.y + i * deltay - offset_y, texture);
    }





}



bool Canvas_field::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){
    if (is_in_widget(x, y, event.click)){

            brush->draw(event.click.x - this->x - x, event.click.y - this->y - y, this->texture);
            this->is_active = true;
        return true;
    }
    return false;
}

bool Canvas_field::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    if (this->is_active){
        printf("brrrrrrrrrrrrrrr\n");
            brush->draw(event.move.begin_move.x - this->x - x, event.move.begin_move.y - this->y - y, this->texture);

            draw_continuously(event.move, this->texture, this->x + x, this->y + y);
        return true;
    }
    
    return false;

}


bool Canvas_field::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    this->is_active = false;
    return true;
}

    