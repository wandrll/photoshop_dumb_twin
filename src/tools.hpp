#ifndef TOOLS_HEADER
#define TOOLS_HEADER
#include "graphics_provider.hpp"
#include "utils.hpp"
#include "window_widget.hpp"
#include "plugin_std.hpp"

class Layer;



class Abstract_tool{
    public:

    Abstract_tool (const char* name = "") :
        name(name)
    {}

    virtual ~Abstract_tool (){

    }

    virtual void update() = 0;

    virtual void on_press(const Vector& vector) = 0; 

    virtual void on_move(const Vector& start, const Vector& end) = 0; 

    virtual void on_release(const Vector& vector) = 0; 

    virtual void on_tick(const double delta_t){

    }


    Blend_mode merge_mode;


    std::string name;    

};



class Abstract_effect{
    public:

    virtual ~Abstract_effect (){

    }

    virtual void update() = 0;

    virtual void apply () = 0;

    virtual void on_tick(const double delta_t){

    }
    
    std::string name;    

};




class Brush : public Abstract_tool{
    public:

    Brush (const int size, const Color color = {0,0,0}, const double max_size = 20) :
        Abstract_tool("Brush"),
        brush_size(size),
        color(color),
        circle(0, 0, brush_size, color),
        max_size(max_size),
        mode(Blending::Factor::Src_color,
             Blending::Factor::Zero,
             Blending::Factor::One_minus_dst_alpha, 
             Blending::Factor::Src_alpha,
             Blending::Equation::Add, 
             Blending::Equation::Add)
        
    {}

    virtual ~Brush (){

    }

    virtual void update();


    
    virtual void on_press(const Vector& vector); 

    virtual void on_move(const Vector& start, const Vector& end); 

    virtual void on_release(const Vector& vector); 



    void set_color(const Color& color){
        this->color = color;
        this->circle.set_color(color);
    }

    void set_size (const double size){
        this->brush_size = size * max_size;
        // printf("new size is %lg\n", this-)
        this->circle.set_size(this->brush_size);
    }

    
    private:
    int brush_size;
    double max_size;
    Color color;
    Circle circle;
    Blend_mode mode;
    
    
};



class Eraser : public Abstract_tool{
    public:

    Eraser (const int size, const double max_size = 20) :
        Abstract_tool("Eraser"),
        eraser_size(size),
        color(0, 0, 0, 255),
        circle(0, 0, eraser_size),
        max_size(max_size),
        mode(Blending::Factor::Zero,
             Blending::Factor::One, 
             Blending::Factor::One, 
             Blending::Factor::Zero, 
             Blending::Equation::Add,
             Blending::Equation::Add)
    {}

    virtual ~Eraser (){

    }

    
    virtual void on_press(const Vector& vector); 

    virtual void on_move(const Vector& start, const Vector& end); 

    virtual void on_release(const Vector& vector); 

    virtual void update ();

    void set_opacity(const int opacity){
        // int new_alpha = opacity * 255;
        // printf("new alpha = %d\n", new_alpha);
        color.color.a = opacity;
        circle.set_color(color);
    }

    void set_size (const double size){
        this->eraser_size = size * max_size;
        // printf("new size is %lg\n", this-)
        this->circle.set_size(this->eraser_size);
    }


    private:
    Blend_mode mode;
    int eraser_size;
    double max_size;
    Color color;
    Circle circle;

};



class Tools_manager{
    public:
    Tools_manager ();

    ~Tools_manager ();

    Abstract_tool* get_tool(){
        return tools[active_tool];
    }
    
    void change_tool (const int id){
        if (id >= tools.size()){
            printf("You picked the wrong tool fool\n");
            return;
        }
        active_tool = id;
    }

    void add_tool (Abstract_tool* tool){
        this->tools.push_back(tool);
        active_tool = this->tools.size() - 1;
    }

    friend class Tools_widget;

    private:
    int active_tool;
    std::vector<Abstract_tool*> tools;
    
};


class Effects_manager{
    public:

    ~Effects_manager (){
        for (int i = 0; i < effects.size(); i++){
            delete effects[i];
        }
    }

    void add_effect (Abstract_effect* effect){
        this->effects.push_back(effect);
    }

    void apply_effect(int id){
        if (id >= 0 && id < effects.size()){
            effects[id]->apply();
        }
    }

    friend class Effects_widget;

    private:    
    std::vector<Abstract_effect*> effects;

};



class Tools_widget : public Window_widget{
    public:
    
    Tools_widget (const int x, const int y, const int width, const int height);
    
    
    virtual ~Tools_widget (){

    }



    private:
};


class Effects_widget : public Window_widget{
    public:
    Effects_widget (const int x, const int y, const int width, const int height);
    
    
    virtual ~Effects_widget (){

    }


};  



class Loaded_tool : public Abstract_tool{
    public:
    
    Loaded_tool (PPluginInterface* interface);


    virtual ~Loaded_tool (){
        interface->general.deinit();
    }

    virtual void update(){
        if (interface->general.on_settings_update != nullptr)
            interface->general.on_settings_update();
    }

    virtual void on_press(const Vector& vector){
        if (interface->tool.on_press != nullptr)
            interface->tool.on_press(PVec2f(vector.x, vector.y));

    }

    virtual void on_move(const Vector& start, const Vector& end){
        if (interface->tool.on_move != nullptr)
            interface->tool.on_move(PVec2f(start.x, start.y), PVec2f(end.x, end.y));

    }

    virtual void on_release(const Vector& vector){
        if (interface->tool.on_release != nullptr)
            interface->tool.on_release(PVec2f(vector.x, vector.y));

    }

    virtual void on_tick(const double delta_t){
        if (interface->general.on_tick != nullptr)
            interface->general.on_tick(delta_t);
        
    }

    private:

    PPluginInterface* interface;

};


class Loaded_effect : public Abstract_effect{
    public:

    Loaded_effect (PPluginInterface* interface);


    virtual ~Loaded_effect (){
        interface->general.deinit();
    }


    virtual void update(){
        if (interface->general.on_settings_update != nullptr)
            interface->general.on_settings_update();
    }

    virtual void apply(){
        
        if (interface->effect.apply != nullptr){
            interface->effect.apply();
            std::cout << "Apply loaded effect\n";
        }
    }


    virtual void on_tick(const double delta_t){
        if (interface->general.on_tick != nullptr)
            interface->general.on_tick(delta_t);
        
    }

    private:
    PPluginInterface* interface;
};


#endif
