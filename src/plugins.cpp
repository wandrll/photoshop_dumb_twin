#include "plugins.hpp"
#include <time.h>
#include <stdio.h>
#include "singleton.hpp"
#include "canvas_window.hpp"
#include "tools.hpp"
#include "text_field.hpp"
#include "adapter_plugin_widget.hpp"
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
    rect.draw(*texture, 0, 0, Blend_mode(Blending::Factor::Zero, Blending::Factor::One));
    
}

PUPPY::RGBA* Plugin::RenderTarget::get_pixels() const{
    return (PUPPY::RGBA*)(texture->get_array());
}

void Plugin::RenderTarget::clear(const PUPPY::RGBA &color){
    texture->fill_color(Color(color.r, color.g, color.b, color.a));
}

void Plugin::RenderTarget::render_circle   (const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode){
    // std::cout << "draw circle " << position.x << " " << position.y << " " << radius << std::endl; 
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
    // Blend_mode mode = get_mode(render_mode);
    // std::cout << "draw triangle " << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << std::endl; 
    render_line(p1, p2, color, render_mode);
    render_line(p2, p3, color, render_mode);
    render_line(p3, p1, color, render_mode);

    // Rectangle rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, Color(color.r, color.g, color.b, color.a));
    // rect.draw(*texture, 0, 0, mode);

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
    // std::cout << "set pixel\n";
    // Blend_mode mode = get_mode(render_mode);

    texture->set_pixels((const Color*)(data), position.x, position.y, size.x, size.y);
}

void Plugin::RenderTarget::apply_shader(const PUPPY::Shader *shader){

}


void Plugin::RenderTarget::set_texture(Plugin::RenderTarget* texture_){
    texture->clear();
    Sprite sprite;
    sprite.set_texture(*(texture_->texture));
    sprite.set_size(texture->get_size().x, texture->get_size().y);

    sprite.draw(*texture);

}





/////////////////////////////////RENDER TARGET INTERFACE END///////////////////////////////////////////



/////////////////////////////////APP INTERFACE BEGIN///////////////////////////////////////////
bool   Plugin::AppInterface::ext_enable(const char *name) const{
    return false;
}
void*  Plugin::AppInterface::ext_get_func(const char *extension, const char *func) const{}
void*  Plugin::AppInterface::ext_get_interface(const char *extension, const char *name) const{}
void   Plugin::AppInterface::ext_register_as(const char *extension) const{}


Plugin::AppInterface::AppInterface ()
    {
        std_version = PUPPY::STD_VERSION,
        feature_level = 0,
        factory = {new Plugin::WidgetFactory, 
                   nullptr, 
                   new Plugin::RenderTargetFactory};
    }


void   Plugin::AppInterface::log(const char* format_line, ...) const{
    va_list arg;
    va_start(arg, format_line);
    vprintf(format_line, arg);
    va_end(arg);
}

double Plugin::AppInterface::get_absolute_time()     const{
    clock_t clocks = clock() - global_singleton->get_start();
    // std::cout << "TIME FROM START: " << static_cast<double>(clocks) / CLOCKS_PER_SEC << std::endl;
    return static_cast<double>(clocks) / CLOCKS_PER_SEC;
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
    std::vector<PUPPY::WBody> res;
    std::vector<Widget*> widgets = global_singleton->get_main_widget()->get_widgets();

    // std::cout << "---------------------" << std::endl;
    for (int i = 0; i < widgets.size(); i++){
        // std::cout << widgets[i]->get_coordinats().x << " " << widgets[i]->get_coordinats().y << " " << widgets[i]->get_size().x << " " << widgets[i]->get_size().y << std::endl;
        if (dynamic_cast<::Window_widget*>(widgets[i])){
            res.push_back(PUPPY::WBody({widgets[i]->get_coordinats().x, widgets[i]->get_coordinats().y}, {widgets[i]->get_size().x, widgets[i]->get_size().y}));
        }
    }
        // std::cout << "\n---------------------" << std::endl;

    return res;
}

PUPPY::Widget* Plugin::AppInterface::get_root_widget() const{
    Plugin::Widget_manager* res ; 
    return new Plugin::Widget_manager(global_singleton->get_main_widget());
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
    Texture* texture = new Texture(1, 1);
    texture->load_from_memory(data, size.x, size.y);

    return new RenderTarget(texture, true);
}

PUPPY::RenderTarget* Plugin::RenderTargetFactory::from_file(const char* path) const{
    
    Texture* texture = new Texture(1, 1);
    if(!texture->load_from_file(path)){
        perror("CANT LOAD IMAGE");
        abort();
    }
    return new RenderTarget(texture, true);
    
}

/////////////////////////////////RENDER TARGET FACTORY END///////////////////////////////////////////







/////////////////////////////////WIDGET FACTORY BEGIN///////////////////////////////////////////

PUPPY::Button*       Plugin::WidgetFactory::button       (const PUPPY::WBody &body, PUPPY::Widget *parent) const{
    Adapter::Text_Button* adapter = new Adapter::Text_Button(body.position.x, body.position.y, body.size.x, body.size.y, "button", nullptr);
    Plugin::Button* button = new Plugin::Button(adapter);
    adapter->set_plugin_widget(button);

    if (parent){
        parent->add_child(button);
    }

    return button;
    // return nullptr;
}

PUPPY::Button*       Plugin::WidgetFactory::button       (const PUPPY::Vec2f &pos, const char *caption, PUPPY::Widget *parent) const{
    Adapter::Text_Button* adapter = new Adapter::Text_Button(pos.x, pos.y, strlen(caption)*10, 20, caption, nullptr);
    Plugin::Button* button = new Plugin::Button(adapter);
    adapter->set_plugin_widget(button);

    if (parent){
        parent->add_child(button);
    }

    return button;
}

PUPPY::Slider*       Plugin::WidgetFactory::slider       (PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget *parent) const{
    return nullptr;
}

PUPPY::TextField*    Plugin::WidgetFactory::text_field   (const PUPPY::WBody &body, PUPPY::Widget *parent) const{
    return nullptr;
}

PUPPY::Window*       Plugin::WidgetFactory::window       (const char *name, const PUPPY::WBody &body, PUPPY::Widget *parent) const{
    Adapter::Window_widget* adapter = new Adapter::Window_widget(body.position.x, body.position.y, body.size.x, body.size.y, nullptr);
    Plugin::Window* window = new Plugin::Window(adapter);
    
    adapter->set_plugin_widget(window);


    if (parent){
        parent->add_child(window);
    }else{
        ::Widget_manager* man = global_singleton->get_main_widget();
        man->register_widget(adapter);

    }

    return window;
}

PUPPY::ColorPicker*  Plugin::WidgetFactory::color_picker (const PUPPY::WBody &body, PUPPY::Widget *parent) const{
    return nullptr;
}

PUPPY::Label*        Plugin::WidgetFactory::label        (const PUPPY::Vec2f &pos, const char *text, PUPPY::Widget *parent) const{
    return nullptr;
}

PUPPY::Widget*       Plugin::WidgetFactory::abstract     (const PUPPY::WBody &body, PUPPY::Widget *parent) const{
    Adapter::Widget_manager* manager = new Adapter::Widget_manager(body.position.x, body.position.y, body.size.x, body.size.y, nullptr);
    Plugin::Widget_manager* widget = new Plugin::Widget_manager(manager);
    manager->set_plugin_widget(widget);

    if (parent){
        parent->add_child(widget);
    }

    return widget;
}


/////////////////////////////////WIDGET FACTORY END///////////////////////////////////////////


/////////////////////////////////ABSTRACT WIDGET  BEGIN///////////////////////////////////////////
Plugin::AbstractWidget::AbstractWidget (::Widget* widget) : 
    widget(widget),
    texture(new Plugin::RenderTarget(new ::Texture(widget->get_size().x, widget->get_size().y), true))
{
    texture->get_texture()->fill_color(Color(0, 0, 0, 0));
    // printf("Create Abstract Widget %p with Texture %p(%p)\n",this, texture, texture->get_texture());
    fflush(stdout);
}

Plugin::AbstractWidget::~AbstractWidget() {
    delete texture;
}


void Plugin::AbstractWidget::set_position(const PUPPY::Vec2f &position_){
    widget->set_coordinats({position_.x, position_.y});
}

void Plugin::AbstractWidget::set_size(const PUPPY::Vec2f &size_){
    widget->set_size(size_.x, size_.y);
}

PUPPY::WBody Plugin::AbstractWidget::get_body(){
    Vector size = widget->get_size();
    Vector coords = widget->get_coordinats();
    return PUPPY::WBody({coords.x, coords.y}, {size.x, size.y});
}

void Plugin::AbstractWidget::set_body(const PUPPY::WBody &body_){
    set_position(body_.position);
    set_size(body_.size);
}

PUPPY::Widget* Plugin::AbstractWidget::get_parent() const{
    return parent;
}

void Plugin::AbstractWidget::set_parent(PUPPY::Widget *parent_){
    this->parent = parent_;
}

PUPPY::RenderTarget* Plugin::AbstractWidget::get_texture(){
    return texture;
}

void Plugin::AbstractWidget::set_texture(PUPPY::RenderTarget *texture_){
    texture->set_texture(dynamic_cast<Plugin::RenderTarget*>(texture_));
}   

bool Plugin::AbstractWidget::is_active(){
    return widget->is_active_();
}

bool Plugin::AbstractWidget::is_inside(const PUPPY::Vec2f &pos){
    Vector size = widget->get_size();
    Vector position = widget->get_coordinats();

    // std::cout << " pos is " << pos.x << " " << pos.y << std::endl;
    // std::cout << " size " << size.x << " " << size.y << std::endl;

    return ((0 < pos.x && pos.x < size.x) && 
            (0 < pos.y && pos.y < size.y));
}

bool Plugin::AbstractWidget::delete_child(PUPPY::Widget *child_){
    return false;
}

bool Plugin::AbstractWidget::add_child(PUPPY::Widget *child_){
    return false;
}

void Plugin::AbstractWidget::hide() {
    widget->set_active(false);
}

void Plugin::AbstractWidget::show() {
    widget->set_active(true);
}

void Plugin::AbstractWidget::focus(){
    widget->set_focus();
}

void Plugin::AbstractWidget::set_caption(const char *text, size_t font_size, const PUPPY::Vec2f *pos){

}

void Plugin::AbstractWidget::set_base_color(const PUPPY::RGBA &color){

}

bool Plugin::AbstractWidget::delete_from_parent(){
    if (parent){
        return parent->delete_child(this);
    }
    return false;
}   

void Plugin::AbstractWidget::set_to_delete(){
    widget->mark_close();
}

void* Plugin::AbstractWidget::get_extra_data(void *arg){
    return nullptr;
}

::Widget* Plugin::AbstractWidget::get_app_widget () const{
    return this->widget;
}


/////////////////////////////////ABSTRACT WIDGET  END///////////////////////////////////////////

/////////////////////////////////PLUGIN WIDGET MANAGER BEGIN///////////////////////////////////////////
Plugin::Widget_manager::Widget_manager (::Widget_manager* widget) :
    AbstractWidget(widget),
    widget_manager(widget)
{}

bool Plugin::Widget_manager::add_child(PUPPY::Widget *child_){
    // printf("Try to add child %p  into %p\n", child_, this);
    if (!child_){
        return false;
    }

    Plugin::AbstractWidget* child = dynamic_cast<Plugin::AbstractWidget*>(child_);
    // printf("Child casted into %p\n", child);
    child_->set_parent(this);
    
    if (child){
        widget_manager->register_widget(child->get_app_widget());
    }else{
        PUPPY::WBody body = child_->get_body();
        Adapter::Widget_manager* manager = new Adapter::Widget_manager(body.position.x, body.position.y, body.size.x, body.size.y, child_);


        widget_manager->register_widget(manager);

        // printf("Wrap custom plugin widget %p in my %p\n", child_, manager);
        fflush(stdout);
    }

    return true;
    
}


bool Plugin::Widget_manager::delete_child(PUPPY::Widget *child_){
    Plugin::Widget_manager* child = dynamic_cast<Plugin::Widget_manager*>(child_);
    
    widget_manager->delete_widget(child->widget);
    child->widget = nullptr;
    delete child_;

    return true;
}


// void Plugin::Widget_manager::on_render          (const PUPPY::Event::Render          &event){
//     widget_manager->::Widget_manager::draw(0, 0, *(texture->texture));
// }

// void Plugin::Widget_manager::on_tick            (const PUPPY::Event::Tick            &event){
//     widget_manager->::Widget_manager::on_tick(event.dt);
// }

// void Plugin::Widget_manager::on_mouse_press     (const PUPPY::Event::MousePress      &event){
//     widget_manager->on_mouse_press(widget_manager->get_coordinats().x, widget_manager->get_coordinats().y, Event::Left_Mouse_press({event.position.x, event.position.y}));
// }

// void Plugin::Widget_manager::on_mouse_release   (const PUPPY::Event::MouseRelease    &event){
//     widget_manager->on_mouse_release(widget_manager->get_coordinats().x, widget_manager->get_coordinats().y, Event::Mouse_release({event.position.x, event.position.y}));
// }

// void Plugin::Widget_manager::on_mouse_move      (const PUPPY::Event::MouseMove       &event){
//     widget_manager->on_mouse_pressed_move(widget_manager->get_coordinats().x, widget_manager->get_coordinats().y, 
//                                 Event::Mouse_pressed_move({{event.from.x, event.from.y}, {event.to.x, event.to.y}}));
// }


// void Plugin::Widget_manager::on_key_down        (const PUPPY::Event::KeyDown         &event){
//     Event::Press_key key;
//     key.key = Key(int(event.code));
//     widget_manager->on_key_press(key);
// }

// void Plugin::Widget_manager::on_key_up          (const PUPPY::Event::KeyUp           &event){
//     Event::Release_key key;
//     key.key = Key(int(event.code));
//     widget_manager->on_key_release(key);
// }

// void Plugin::Widget_manager::on_text_enter      (const PUPPY::Event::TextEnter       &event){
//     widget_manager->on_text_enter(Event::Text_enter(event.keycode));
// }

// void Plugin::Widget_manager::on_scroll          (const PUPPY::Event::Scroll          &event){

// }

// void Plugin::Widget_manager::on_hide            (const PUPPY::Event::Hide            &event){

// }

// void Plugin::Widget_manager::on_show            (const PUPPY::Event::Show            &event){

// }

/////////////////////////////////PLUGIN WIDGET END///////////////////////////////////////////

/////////////////////////////////WINDOW WIDGET START///////////////////////////////////////////

Plugin::Window::Window (::Window_widget* widget) :
    Plugin::AbstractWidget(widget),
    Plugin::Widget_manager(widget),
    window(widget)
{}

/////////////////////////////////WINDOW WIDGET END///////////////////////////////////////////



/////////////////////////////////BUTTON WIDGET START///////////////////////////////////////////


class Handle_func{
    public:

    void operator() (const Data_for_controller& data, Plugin::Button* button){
        printf("Call button handler in plugin\n");
        button->proceed_handler();
    }

};

Plugin::Button::Button (::Text_button* button) : 
    Plugin::AbstractWidget(button),
    button(button)
{
    Controller<Handle_func, Plugin::Button>* control = new Controller<Handle_func, Plugin::Button>(this);
    button->add_controller(control);
}


/////////////////////////////////BUTTON WIDGET END///////////////////////////////////////////





Plugin_manager::~Plugin_manager (){
    for (int i = 0; i < dlls.size(); i++){
        dlclose(dlls[i]);
    }
}

void Plugin_manager::load_dlls(const std::string& dir){

    Tools_manager* tools = global_singleton->get_tools();
    Effects_manager* effects = global_singleton->get_effects();

    std::filesystem::path path(dir);

    for (auto it = std::filesystem::directory_iterator(dir); it != std::filesystem::end(it); it++){
        if (it->is_directory()){
            continue;
        }

        void* curr_lib = dlopen((*it).path().c_str()    , RTLD_LAZY);

        

        this->dlls.push_back(curr_lib);

        
        PUPPY::PluginInterface* (*get_plugin_interface)() = (PUPPY::PluginInterface*(*)())dlsym(curr_lib, PUPPY::GET_INTERFACE_FUNC);
        
        if (get_plugin_interface == nullptr){
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
        }

        PUPPY::PluginInterface* plugin_interface = get_plugin_interface();

        plugin_interface->init(global_singleton->get_app_interface(), path);
        plugin_interface->show_settings();
        plugins.push_back(plugin_interface);

        switch(plugin_interface->get_info()->type){
            case PUPPY::TOOL:{

                std::cout << "tool loaded" << std::endl;
                Loaded_tool* tool = new Loaded_tool(plugin_interface);
                tools->add_tool(tool);

                break;
            }

            case PUPPY::EFFECT:{
                std::cout << "effect loaded" << std::endl;
        
                Loaded_effect* effect = new Loaded_effect(plugin_interface);
                effects->add_effect(effect);

                break;
            }

            case PUPPY::EXTENSION:{
                

            }

        }



   }
}


void Plugin_manager::deinit_plugins (){
    for (int i = 0; i < plugins.size(); i++){
        plugins[i]->deinit();
    }
}

void Plugin_manager::on_tick(double dt){
    // printf("Plugin manager on tick %lg  size is %d\n", dt, plugins.size());
    for (int i = 0; i < plugins.size(); i++){
        plugins[i]->on_tick(dt * 10);
    }
}






