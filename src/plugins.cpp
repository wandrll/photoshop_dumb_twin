#include "plugin_std.hpp"
#include <time.h>
#include <stdio.h>
#include "../src/singleton.hpp"
#include "../src/canvas_window.hpp"
#include "../src/tools.hpp"

#include <dlfcn.h>
#include <unistd.h>

#include <stdarg.h>

#include <filesystem>
#include <iostream>
#include <fstream>

extern Singleton* global_singleton;

void log (const char* format_line, ...){
    va_list arg;
    va_start(arg, format_line);
    
    vprintf(format_line, arg);
    
    va_end(arg);
}

double get_absolute_time (){
    clock_t clocks = clock();

    return ((double)clocks)/CLOCKS_PER_SEC;
}

void release_pixels(PRGBA *pixels){
    delete[] pixels;
}

PRGBA get_color (){
    Color col = global_singleton->get_color();

    PRGBA res (col.color.r, col.color.g, col.color.b, col.color.a);

    return res;
}

float get_size (){
    return global_singleton->get_size() * 20;
}

PRGBA* get_pixels (){
    return (PRGBA*)(global_singleton->get_layer()->get_array());

}

void get_size (size_t* width, size_t* height){
    Vector size = global_singleton->get_layer()->get_size();
    *width = size.x;
    *height = size.y;

}


Texture* get_texture(const PRenderMode *render_mode){

    switch (render_mode->draw_policy){
        case PPDP_PREVIEW:{
            return global_singleton->get_layer()->get_preview_texture();
        }

        case PPDP_ACTIVE:{
            return global_singleton->get_layer()->get_active_texture();
        }
        default:
            return NULL;
    }

}

Blend_mode get_mode(const PRenderMode *render_mode){
    switch (render_mode->blend)
    {
        case PPBM_COPY:{
            return Blend_mode(Blending::Factor::One, Blending::Factor::Zero);
        }
        
        default:{
            return {};
        }
    }
}


void circle (PVec2f position, float radius, PRGBA color, const PRenderMode *render_mode){
    Texture* texture = get_texture(render_mode);
    Blend_mode mode = get_mode(render_mode);

    Circle circle (position.x - radius, position.y - radius, radius, Color(color.r, color.g, color.b, color.a));

    circle.draw(*texture, 0, 0, mode);

}

void line(PVec2f start, PVec2f end, PRGBA color, const PRenderMode *render_mode){
    Texture* texture = get_texture(render_mode);
    Blend_mode mode = get_mode(render_mode);

    Line line (start.x, start.y, end.x, end.y, Color(color.r, color.g, color.b, color.a));
    line.draw(*texture, 0, 0, mode);
}

 void triangle(PVec2f p1, PVec2f p2, PVec2f p3, PRGBA color, const PRenderMode *render_mode){

 }
       


void rectangle(PVec2f p1, PVec2f p2, PRGBA color, const PRenderMode *render_mode){
    Texture* texture = get_texture(render_mode);
    Blend_mode mode = get_mode(render_mode);

    Rectangle rect(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y, Color(color.r, color.g, color.b, color.a));
    rect.draw(*texture, 0, 0, mode);

}

void pixels(PVec2f position, const PRGBA *data, size_t width, size_t height, const PRenderMode *render_mode){
    Texture* texture = get_texture(render_mode);
    
    texture->set_pixels((const Color*)(data), position.x, position.y, width, height);

}
    

PAppInterface* build_app_interface(){
    PAppInterface* interface = new PAppInterface;

    interface->std_version = 0;

    interface->extensions.enable = nullptr;
    interface->extensions.get_func = nullptr;


    interface->general.feature_level = 0;
    interface->general.log = log;
    interface->general.get_absolute_time = get_absolute_time;
    interface->general.release_pixels = release_pixels; 
    interface->general.get_color = get_color; 
    interface->general.get_size = get_size;
    

    interface->target.get_pixels = get_pixels;
    interface->target.get_size = get_size;


    interface->render.circle = circle;
    interface->render.line = line;
    interface->render.triangle = triangle;
    interface->render.rectangle = rectangle;
    interface->render.pixels = pixels;


    interface->settings.create_surface = nullptr;
    interface->settings.destroy_surface = nullptr;
    interface->settings.add = nullptr;
    interface->settings.get = nullptr;
    
    interface->shader.apply = nullptr;
    interface->shader.compile = nullptr;
    interface->shader.release = nullptr;
    interface->shader.set_uniform_int = nullptr;
    interface->shader.set_uniform_int_arr = nullptr;
    interface->shader.set_uniform_float = nullptr;
    interface->shader.set_uniform_float_arr = nullptr;
    

    return interface;
}





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

        
        PPluginInterface* (*get_plugin_interface)() = (PPluginInterface* (*)())dlsym(curr_lib, "get_plugin_interface");
        
        if (get_plugin_interface == nullptr){
            std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
        }

        PPluginInterface* plugin_interface = get_plugin_interface();

        switch(plugin_interface->general.get_info()->type){
            case PPT_TOOL:{

                Loaded_tool* tool = new Loaded_tool(plugin_interface);
                tools->add_tool(tool);

                break;
            }

            case PPT_EFFECT:{
                
                Loaded_effect* effect = new Loaded_effect(plugin_interface);
                effects->add_effect(effect);

                break;
            }

        }



   }
}






