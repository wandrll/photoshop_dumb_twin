

#include "sprite_widget.hpp"
#include "singleton.hpp"

extern Singleton* global_singleton;

void Widget_sprite::draw (const int x, const int y, Texture& window){
    Sprite* sprite = global_singleton->get_resource_handler()->get_sprite(this->sprite_id);
    sprite->set_position(this->x, this->y);
    sprite->set_size(this->width, this->height);
    
    // printf("call Widget_sprite_draw x = %d   y = %d\n", x, y);

    sprite->draw(window, this->x + x, this->y + y);
}