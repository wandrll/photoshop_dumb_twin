#include "adapter_plugin_widget.hpp"
#include "plugins.hpp"


void Adapter::Widget_manager::draw (const int x, const int y, Texture& window){
    

    plugin_widget->on_render({true});

    PUPPY::Vec2f pos = plugin_widget->get_body().position;



    Sprite sprite;
    Plugin::RenderTarget* texture = dynamic_cast<Plugin::RenderTarget*>(plugin_widget->get_texture());

    if (!texture){
        return;
    }

    Texture* plugin_texture = texture->get_texture(); 

    sprite.set_texture(*plugin_texture);

    sprite.draw(window, x + this->x, y + this->y);

    ::Widget_manager::draw(x, y, window);
}



void Adapter::Widget_manager::on_tick (double dt){

    plugin_widget->on_tick({dt});
    ::Widget_manager::on_tick(dt);
}

bool Adapter::Widget_manager::on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event){

    plugin_widget->on_mouse_press({{event.click.x - x, event.click.y - y}, PUPPY::MouseButton::left});

    return ::Widget_manager::on_mouse_press(x, y, event);


}

bool Adapter::Widget_manager::on_mouse_release (const int x, const int y, const Event::Mouse_release& event){
    plugin_widget->on_mouse_release({{event.click.x - x, event.click.y - y}, PUPPY::MouseButton::left});

    return ::Widget_manager::on_mouse_release(x, y, event);
    

}

bool Adapter::Widget_manager::on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event){
    plugin_widget->on_mouse_move({{event.move.begin_move.x - x, event.move.begin_move.y - y}, {event.move.end_move.x - x, event.move.end_move.y - y}});

    return ::Widget_manager::on_mouse_pressed_move(x, y, event);


}

bool Adapter::Widget_manager::on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event){
    return false;

}

bool Adapter::Widget_manager::on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event){
    plugin_widget->on_mouse_press({{event.click.x - x, event.click.y - y}, PUPPY::MouseButton::right});

    return ::Widget_manager::on_right_mouse_press(x, y, event);
    
}

bool Adapter::Widget_manager::on_key_press (const Event::Press_key& event){
    plugin_widget->on_key_down({PUPPY::Keyboard::Key(event.key)});
    
    return ::Widget_manager::on_key_press(event);

}

bool Adapter::Widget_manager::on_key_release (const Event::Release_key& event){
    plugin_widget->on_key_up({PUPPY::Keyboard::Key(event.key)});

    return ::Widget_manager::on_key_release(event);

}

bool Adapter::Widget_manager::on_text_enter (const Event::Text_enter& event){
    plugin_widget->on_text_enter({event.keycode});
    
    return ::Widget_manager::on_text_enter(event);

}




    

