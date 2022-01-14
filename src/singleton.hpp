#ifndef SINGLETON_HEADER
#define SINGLETON_HEADER

#include "tools.hpp"
#include "resource_handler.hpp"
#include "utils.hpp"
#include "plugins.hpp"
#include "main_window_widget.hpp"
#include <ctime>

// #include "canvas_window.hpp"
class Layer;

class Singleton{
    public:

    Singleton ();

    Singleton (const Singleton& copy) = delete;
    void operator= (const Singleton& copy) = delete;

    void set_layer (Layer* layer){
        this->layer = layer;
    }

    void set_color (const Color& color){
        this->color.color = color;
    }

    void set_hue (const double val){
        this->color.hue = val;
    }

    void set_lightness (const double val){
        this->color.lightness = val;
    }

    void set_saturation (const double val){
        this->color.saturation = val;
    }

    void set_opacity (const double val){
        this->color.opacity = val;
    }

    void set_size (const double size){
        this->size = size;
    }
    
    void set_window (Window* window){
        this->window = window;
    }

    void set_main_window(Main_window_widget* widget){
        main_window = widget;
    }

    clock_t get_start(){
        return begin;
    }

    Widget_manager* get_main_widget(){
        return main_window;
    }

    Resource_handler* get_resource_handler (){
        return &(this->resources);
    }

    Tools_manager* get_tools (){
        return &(this->tools);
    }

    Effects_manager* get_effects (){
        return &(this->effects);
    }

    Layer* get_layer (){
        return this->layer;
    }

    Color get_color (){
        return this->color.color;
    }

    double get_size (){
        return this->size;
    }

    Window* get_window (){
        return this->window;
    }

    ~Singleton (){
        delete app_interface;
    }

    void update_color() {
        this->color.color = convert_color(color.hue, color.saturation, color.lightness, color.opacity * 255);
    }

    void set_plugin_manager(Plugin_manager* plugins){
        this->plugins = plugins;
    }

    Plugin_manager* get_plugin_manager(){
        return plugins;
    }

    Plugin::AppInterface* get_app_interface(){
        return this->app_interface;
    }

    private:

  
    Tools_manager tools;
    Effects_manager effects;

    Resource_handler resources;

    Layer* layer;
    struct{
        Color color;
        double hue;
        double saturation;
        double lightness;
        double opacity;
    }color;
    double size;
    clock_t begin;
    Window* window;
    Widget_manager* main_window;

    Plugin::AppInterface* app_interface;

    Plugin_manager* plugins;

};






#endif