#include "plugins.hpp"
#include <time.h>
#include <stdio.h>
#include "singleton.hpp"
#include "canvas_window.hpp"
#include "tools.hpp"

#include <dlfcn.h>
#include <unistd.h>

#include <stdarg.h>

#include <filesystem>
#include <iostream>
#include <fstream>

extern Singleton* global_singleton;

const int multiplier = 20;


Blend_mode get_mode(const PUPPY::RenderMode& render_mode){
    switch (render_mode.blend)
    {
        case PUPPY::COPY:{
            return Blend_mode(Blending::Factor::One, Blending::Factor::Zero);
        }
        
        default:{
            return {};
        }
    }
}

/////////////////////////////////RENDER TARGET INTERFACE BEGIN///////////////////////////////////////////

PUPPY::RenderTarget* Plugin::RenderTarget::get_copy() const{

}

PUPPY::Vec2s Plugin::RenderTarget::get_size() const{
    Vector size = texture->get_size();
    return {size.x, size.y};
}

PUPPY::RGBA Plugin::RenderTarget::get_pixel(size_t x, size_t y) const{
    Color color = texture->get_pixel(x, y);
    return PUPPY::RGBA(color.color.r, color.color.g, color.color.b, color.color.a);
}

void Plugin::RenderTarget::set_pixel(size_t x, size_t y, const PUPPY::RGBA &color){
    Rectangle rect (x, y, 1, 1, Color(color.r, color.g, color.b, color.a));
}

PUPPY::RGBA* Plugin::RenderTarget::get_pixels() const{
    return (PUPPY::RGBA*)(texture->get_array());
}

void Plugin::RenderTarget::clear(const PUPPY::RGBA &color){
    texture->fill_color(Color(color.r, color.g, color.b, color.a));
}

void Plugin::RenderTarget::render_circle   (const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode){
    Blend_mode mode = get_mode(render_mode);

    Circle circle (position.x - radius, position.y - radius, radius, Color(color.r, color.g, color.b, color.a));

    circle.draw(*texture, 0, 0, mode);
}

void Plugin::RenderTarget::render_line     (const PUPPY::Vec2f &start, const PUPPY::Vec2f &end, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode){
    Blend_mode mode = get_mode(render_mode);

    Line line (start.x, start.y, end.x, end.y, Color(color.r, color.g, color.b, color.a));
    line.draw(*texture, 0, 0, mode);
}

void Plugin::RenderTarget::render_triangle (const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::Vec2f &p3, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode){

}

void Plugin::RenderTarget::render_rectangle(const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode){
    Blend_mode mode = get_mode(render_mode);

    Rectangle rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, Color(color.r, color.g, color.b, color.a));
    rect.draw(*texture, 0, 0, mode);
}

void Plugin::RenderTarget::render_texture(const PUPPY::Vec2f &position, const PUPPY::RenderTarget *texture, const PUPPY::RenderMode &render_mode){
    Blend_mode mode = get_mode(render_mode);

    const RenderTarget* target = dynamic_cast<const RenderTarget*>(texture);

    Sprite sprite;
    sprite.set_texture(*(target->get_texture()));
    // Rectangle rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, Color(color.r, color.g, color.b, color.a));
    sprite.draw(*(this->texture), 0, 0, mode);

}

void Plugin::RenderTarget::render_pixels (const PUPPY::Vec2f &position, const PUPPY::Vec2s &size, const PUPPY::RGBA *data, const PUPPY::RenderMode &render_mode){
    texture->set_pixels((const Color*)(data), position.x, position.y, size.x, size.y);
}

void Plugin::RenderTarget::apply_shader(const PUPPY::Shader *shader){

}







/////////////////////////////////RENDER TARGET INTERFACE END///////////////////////////////////////////



/////////////////////////////////APP INTERFACE BEGIN///////////////////////////////////////////
bool   Plugin::AppInterface::ext_enable(const char *name) const{}
void*  Plugin::AppInterface::ext_get_func(const char *extension, const char *func) const{}
void*  Plugin::AppInterface::ext_get_interface(const char *extension, const char *name) const{}
void   Plugin::AppInterface::ext_register_as(const char *extension) const{}



void   Plugin::AppInterface::log(const char* format_line, ...) const{
    va_list arg;
    va_start(arg, format_line);
    vprintf(format_line, arg);
    va_end(arg);
}

double Plugin::AppInterface::get_absolute_time()     const{
    clock_t clocks = clock();
    return ((double)clocks)/CLOCKS_PER_SEC;
}

PUPPY::RGBA Plugin::AppInterface::get_color() const{
    Color col = global_singleton->get_color();
    PUPPY::RGBA res (col.color.r, col.color.g, col.color.b, col.color.a);
    return res;
}

float Plugin::AppInterface::get_size() const{
    return global_singleton->get_size() * multiplier;
}

void  Plugin::AppInterface::set_color(const PUPPY::RGBA &color) const{
    global_singleton->set_color(Color(color.r, color.g, color.b, color.a));
}

void  Plugin::AppInterface::set_size(float size) const{
    global_singleton->set_size(size);
}

const std::vector<PUPPY::WBody> Plugin::AppInterface::get_windows() const{
    return std::vector<PUPPY::WBody>();
}

PUPPY::Widget* Plugin::AppInterface::get_root_widget() const{
    return NULL;
}

PUPPY::RenderTarget* Plugin::AppInterface::get_target()  const{
    return new RenderTarget(global_singleton->get_layer()->get_active_texture());
}

PUPPY::RenderTarget* Plugin::AppInterface::get_preview() const{
    return new RenderTarget(global_singleton->get_layer()->get_preview_texture());
}

void    Plugin::AppInterface::flush_preview()        const{
    global_singleton->get_layer()->merge_with_preview();
}


/////////////////////////////////APP INTERFACE END///////////////////////////////////////////






/////////////////////////////////RENDER TARGET FACTORY BEGIN///////////////////////////////////////////

PUPPY::RenderTarget* Plugin::RenderTargetFactory::spawn(const PUPPY::Vec2s &size, const PUPPY::RGBA &color) const{
    Texture* texture = new Texture(size.x, size.y);
    texture->fill_color(Color(color.r, color.g, color.b, color.a));

    return new RenderTarget(texture, true);

}

PUPPY::RenderTarget* Plugin::RenderTargetFactory::from_pixels(const PUPPY::Vec2s &size, const PUPPY::RGBA *data) const{
    Texture* texture = new Texture(0, 0);
    texture->load_from_memory(data, size.x, size.y);

    return new RenderTarget(texture, true);
}

PUPPY::RenderTarget* Plugin::RenderTargetFactory::from_file(const char* path) const{
    Texture* texture = new Texture(0, 0);
    texture->load_from_file(path);
    
    return new RenderTarget(texture, true);
}

/////////////////////////////////RENDER TARGET FACTORY END///////////////////////////////////////////







/////////////////////////////////WIDGET FACTORY BEGIN///////////////////////////////////////////

// PUPPY::Button*       WidgetFactory::button       (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::Button*       WidgetFactory::button       (const PUPPY::Vec2f &pos, const char *caption, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::Slider*       WidgetFactory::slider       (PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::TextField*    WidgetFactory::text_field   (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::Window*       WidgetFactory::window       (const char *name, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::ColorPicker*  WidgetFactory::color_picker (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::Label*        WidgetFactory::label        (const PUPPY::Vec2f &pos, const char *text, PUPPY::Widget *parent = nullptr) const{

// }

// PUPPY::Widget*       WidgetFactory::abstract     (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const{

// }


/////////////////////////////////WIDGET FACTORY END///////////////////////////////////////////



/////////////////////////////////PLUGIN WIDGET BEGIN///////////////////////////////////////////
Plugin::Widget::Widget (::Widget_manager* widget) :
    widget(widget)
{}

Plugin::Widget::~Widget() {
    delete widget;
}

void Plugin::Widget::set_position(const PUPPY::Vec2f &position_){
    widget->set_coordinats({position_.x, position_.y});
}

void Plugin::Widget::set_size(const PUPPY::Vec2f &size_){
    widget->set_size(size_.x, size_.y);
}

PUPPY::WBody Plugin::Widget::get_body(){
    Vector size = widget->get_size();
    Vector coords = widget->get_coordinats();
    return PUPPY::WBody({coords.x, coords.y}, {size.x, size.y});
}

void Plugin::Widget::set_body(const PUPPY::WBody &body_){
    set_position(body_.position);
    set_size(body_.size);
}

PUPPY::Widget* Plugin::Widget::get_parent() const{
    return parent;
}

void Plugin::Widget::set_parent(PUPPY::Widget *parent_){
    this->parent = dynamic_cast<Plugin::Widget*>(parent_);
}

PUPPY::RenderTarget* Plugin::Widget::get_texture(){
    return nullptr;
}

void Plugin::Widget::set_texture(PUPPY::RenderTarget *texture_){

}

bool Plugin::Widget::is_active(){
    return widget->is_active_();
}

bool Plugin::Widget::is_inside(const PUPPY::Vec2f &pos){
    Vector size = widget->get_size();
    Vector position = widget->get_coordinats();

    return ((position.x < pos.x && pos.x < position.x + size.x) && 
            (position.y < pos.y && pos.y < position.y + size.y));
}

bool Plugin::Widget::add_child(PUPPY::Widget *child_){
    Plugin::Widget* child = dynamic_cast<Plugin::Widget*>(child_);
    child->parent = this;
    widget->register_widget(child->widget);
}

void Plugin::Widget::set_to_delete(){
    widget->mark_close();
}

bool Plugin::Widget::delete_child(PUPPY::Widget *child_){
    Plugin::Widget* child = dynamic_cast<Plugin::Widget*>(child_);
    
    widget->delete_widget(child->widget);
    child->widget = nullptr;
    delete child_;
}

bool Plugin::Widget::delete_from_parent(){

}

void Plugin::Widget::on_render          (const PUPPY::Event::Render          &event){

}

void Plugin::Widget::on_tick            (const PUPPY::Event::Tick            &event){

}

void Plugin::Widget::on_mouse_press     (const PUPPY::Event::MousePress      &event){

}

void Plugin::Widget::on_mouse_release   (const PUPPY::Event::MouseRelease    &event){

}

void Plugin::Widget::on_mouse_move      (const PUPPY::Event::MouseMove       &event){

}

void Plugin::Widget::on_key_down        (const PUPPY::Event::KeyDown         &event){

}

void Plugin::Widget::on_key_up          (const PUPPY::Event::KeyUp           &event){

}

void Plugin::Widget::on_text_enter      (const PUPPY::Event::TextEnter       &event){

}

void Plugin::Widget::on_scroll          (const PUPPY::Event::Scroll          &event){

}

void Plugin::Widget::on_hide            (const PUPPY::Event::Hide            &event){

}

void Plugin::Widget::on_show            (const PUPPY::Event::Show            &event){

}

void Plugin::Widget::hide() {
    widget->set_active(false);
}

void Plugin::Widget::show() {
    widget->set_active(true);
}

void Plugin::Widget::focus(){
    widget->set_focus();
}

void Plugin::Widget::set_caption(const char *text, size_t font_size, const PUPPY::Vec2f *pos){

}

void Plugin::Widget::set_base_color(const PUPPY::RGBA &color){

}




/////////////////////////////////PLUGIN WIDGET END///////////////////////////////////////////










Plugin_manager::~Plugin_manager (){
    for (int i = 0; i < dlls.size(); i++){
        dlclose(dlls[i]);
    }
}

void Plugin_manager::load_dlls(const std::string& dir){

    Tools_manager* tools = global_singleton->get_tools();
    Effects_manager* effects = global_singleton->get_effects();

    for (auto it = std::filesystem::directory_iterator(dir); it != std::filesystem::end(it); it++){
        void* curr_lib = dlopen((*it).path().c_str()    , RTLD_LAZY);

        

        this->dlls.push_back(curr_lib);

        
        PUPPY::PluginInterface* (*get_plugin_interface)() = (PUPPY::PluginInterface*(*)())dlsym(curr_lib, PUPPY::GET_INTERFACE_FUNC);
        
        if (get_plugin_interface == nullptr){
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
        }

        PUPPY::PluginInterface* plugin_interface = get_plugin_interface();

        switch(plugin_interface->get_info()->type){
            case PUPPY::TOOL:{

                Loaded_tool* tool = new Loaded_tool(plugin_interface);
                tools->add_tool(tool);

                break;
            }

            case PUPPY::EFFECT:{
                
                Loaded_effect* effect = new Loaded_effect(plugin_interface);
                effects->add_effect(effect);

                break;
            }

        }



   }
}






