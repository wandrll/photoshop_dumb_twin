

#include "sprite_widget.hpp"

extern Resource_handler handler;

void Widget_sprite::draw (const int x, const int y, Window& window){
    Sprite* sprite = handler.get_sprite(this->sprite_id);
    sprite->set_position(this->x, this->y);
    sprite->set_size(this->width, this->height);
    
    // printf("call Widget_sprite_draw x = %d   y = %d\n", x, y);

    sprite->draw(window, this->x + x, this->y + y);
}