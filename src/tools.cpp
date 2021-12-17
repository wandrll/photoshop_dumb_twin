#include "tools.hpp"
#include "button_widget.hpp"
#include "canvas_window.hpp"
#include "singleton.hpp"

extern Singleton* global_singleton;


void Brush::on_press (const Vector& vector){
    update();
    // Texture* textr = global_singleton->get_layer()->get_active_texture();
    // Color* pixls = textr->get_array();
    // textr->set_pixels(pixls, 0, 0, textr->get_size().x, textr->get_size().y, Blend_mode{});
    this->circle.set_position(vector.x - this->brush_size, vector.y - this->brush_size);
    this->circle.draw(*(global_singleton->get_layer()->get_preview_texture()), 0, 0, mode);
}

void Brush::on_release (const Vector& vector){
    
}

void Brush::on_move(const Vector& start, const Vector& end){
    Layer* layer = global_singleton->get_layer();
    Vector move = end - start;
    
    int x = end.x - start.x;
    int y = end.y - start.y;

    int cnt = std::max(abs(x), abs(y));

    if(cnt < 1){
        return;
    }

    double deltax = ((double)x) / ((double)cnt);
    double deltay = ((double)y) / ((double)cnt);
    double currx = 0;
    double curry = 0;


    for (int i = 0; i < cnt; i++){
        this->circle.set_position(start.x - this->brush_size/2 + currx, start.y - this->brush_size/2 + curry);
        this->circle.draw(*(layer->get_preview_texture()), 0, 0, mode);

        currx += deltax;
        curry += deltay;
    }

} 




Tools_manager::Tools_manager () : 
    active_tool(-1)
{}


Tools_manager::~Tools_manager (){
    for (int i = 0; i < tools.size(); i++){
        delete tools[i];
    }
}


// void Eraser::apply (const int x, const int y, Layer* layer){
//     this->circle.set_position(x - this->eraser_size/2, y - this->eraser_size/2);
//     this->circle.draw(*(layer->get_active_texture()), 0, 0, mode);
// }

void Eraser::on_press (const Vector& vector){
    update();
    
    this->circle.set_position(vector.x - this->eraser_size/2, vector.y - this->eraser_size/2);
    this->circle.draw(*(global_singleton->get_layer()->get_active_texture()), 0, 0, mode);
}

void Eraser::on_release (const Vector& vector){
    
}

void Eraser::on_move(const Vector& start, const Vector& end){
    Layer* layer = global_singleton->get_layer();
    Vector move = end - start;
    
    int x = end.x - start.x;
    int y = end.y - start.y;

    int cnt = std::max(abs(x), abs(y));

    if(cnt < 1){
        return;
    }

    double deltax = ((double)x) / ((double)cnt);
    double deltay = ((double)y) / ((double)cnt);
    double currx = 0;
    double curry = 0;


    for (int i = 0; i < cnt; i++){
        this->circle.set_position(start.x - this->eraser_size + currx, start.y - this->eraser_size + curry);
        this->circle.draw(*(layer->get_active_texture()), 0, 0, mode);

        currx += deltax;
        curry += deltay;
    }

} 

class Change_tool_in_manager{
    public:

    void operator() (const Data_for_controller& data, Tools_manager* manager){
        // printf("mark for future closing %p\n", widget);
        manager->change_tool(data.int_value);
    }

};

Tools_widget::Tools_widget (const int x, const int y, const int width, const int height) : 
    Window_widget(x, y, width, height)
{
    Tools_manager* tools = global_singleton->get_tools();
    int cnt = tools->tools.size();

    for (int i = 0; i < cnt; i++){



        Text_button* button = new Text_button (10, 100 + i * 20, 50, 16, tools->tools[i]->name.c_str(), 10, Color(255, 255, 255), Color(0, 0, 0), i);
        Controller<Change_tool_in_manager, Tools_manager>* control2 = new Controller<Change_tool_in_manager, Tools_manager>(global_singleton->get_tools()); 
        button->add_controller(control2);

        this->widgets.push_back(button);
    }
}


class Apply_effect{
    public:

    void operator() (const Data_for_controller& data, Effects_manager* effects){
        // printf("mark for future closing %p\n", widget);
        effects->apply_effect(data.int_value);
    }

};


Effects_widget::Effects_widget (const int x, const int y, const int width, const int height) : 
    Window_widget(x, y, width, height)
{
    Effects_manager* effects = global_singleton->get_effects();
    int cnt = effects->effects.size();

    for (int i = 0; i < cnt; i++){

        Text_button* button = new Text_button (10, 100 + i * 20, 50, 16, effects->effects[i]->name.c_str(), 10, Color(255, 255, 255), Color(0, 0, 0), i);
        Controller<Apply_effect, Effects_manager>* control2 = new Controller<Apply_effect, Effects_manager>(global_singleton->get_effects()); 
        button->add_controller(control2);

        this->widgets.push_back(button);
    }
}

void Brush::update(){
    set_size (global_singleton->get_size());
    set_color (global_singleton->get_color());

    this->color.print();
    std::cout << "size = " << this->brush_size <<std::endl; 
}

void Eraser::update (){
    set_size (global_singleton->get_size());
    set_opacity(global_singleton->get_color().color.a);
}










Loaded_tool::Loaded_tool (PUPPY::PluginInterface* interface) : 
        interface(interface)
{
    this->interface->init(global_singleton->get_app_interface());

    const PUPPY::PluginInfo* info = interface->get_info();

    this->name = std::string(info->name);

}


Loaded_effect::Loaded_effect (PUPPY::PluginInterface* interface) : 
        interface(interface)
{
    this->interface->init(global_singleton->get_app_interface());

    const PUPPY::PluginInfo* info = interface->get_info();
    this->name = std::string(info->name);
}


void Effects_manager::apply_effect(int id){
    if (id >= 0 && id < effects.size()){
        effects[id]->apply();
        global_singleton->get_layer()->update_texture_to_draw();
    }
}