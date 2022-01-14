
#include "sprite_widget.hpp"
#include "button_widget.hpp"
#include "palette_widget.hpp"
#include "canvas_window.hpp"




Canvas_widget::Canvas_widget (const int x, const int y, const int width, const int height) :
        Widget(x, y, width, height),
        Window_widget (x, y, width, height)
    {
    this->set_ability_to_move(true);
 
    Layer_manager_widget* field = new Layer_manager_widget (BORDER_THICKNESS, TOP_BAR_HEIGHT + BORDER_THICKNESS, (width * 2) / 3 - 2 * BORDER_THICKNESS, height - TOP_BAR_HEIGHT - 2*BORDER_THICKNESS - 30);
    this->field = field;

    this->widgets.push_back(field);

    Layer_controller_widget* buttons = new Layer_controller_widget ( 2 * BORDER_THICKNESS + (width * 2) / 3, TOP_BAR_HEIGHT + BORDER_THICKNESS, width/3, height - TOP_BAR_HEIGHT, field);
    this->widgets.push_back(buttons);

}

void Canvas_widget::load_image(const std::string name){
    this->field->get_active_layer()->load_image(name);
}


