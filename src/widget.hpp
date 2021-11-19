#ifndef WIDGET_HEADER
#define WIDGET_HEADER

#include <vector>
#include "graphics_provider.hpp"
#include "event.hpp"


class Widget {
    public:

    Widget (const int x, const int y, const int width, const int height) :
        x(x),
        y(y),
        width(width),
        height(height),
        mark_for_delete(false),
        is_moveable(false)
    {}

    virtual ~Widget (){

    }

    virtual void draw (const int x, const int y, Window& window) = 0;
    
    virtual void update (){

    }



    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event);

    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event);

    virtual bool on_keyboard (const Event::Keyboard_event& event);


    // virtual bool on_click (const int x, const int y);

    void move (const Vector& vector){
        this->x += vector.x;
        this->y += vector.y; 
    }

    Vector get_coordinats (){
        return {this->x, this->y};
    }

    void set_coordinats (const Vector& coords){
        this->x = coords.x;
        this->y = coords.y;
    }

    void mark_close(){
        this->mark_for_delete = true;
    }

    bool is_marked_for_deletion(){
        return this->mark_for_delete;
    }

    void set_ability_to_move(bool val){
        this->is_moveable = val;
    }

    bool get_ability_to_move(){
        return this->is_moveable;
    }

    void set_size (const int size_x, const int size_y){
        this->width = size_x;
        this->height = size_y;
    }

    Vector get_size () const{
        return {this->width, this->height};
    }

    bool is_in_widget(int x, int y, const Click& click){
        // std::cout << "x " << this->x + x ;
        // std::cout << "    y " << this->y + y << std::endl;

        if (   this->x + x <= click.x && click.x <= this->width  + this->x + x 
            && this->y + y <= click.y && click.y <= this->height + this->y + y){
            
            return true;
        }
        return false;
    }

    protected: 

    int x;
    int y;

    int width;
    int height;

    bool is_active;
    bool mark_for_delete;
    bool is_moveable;

};


class Widget_manager : public Widget {
    public:

    Widget_manager (const int x, const int y, const int width, const int height) :
        Widget(x, y, width, height)
    {}
    

    virtual void draw (const int x, const int y, Window& window);

    virtual void update ();

    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event);
    
    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event);

    virtual bool on_keyboard (const Event::Keyboard_event& event);

    void open_image (const std::string& name);

    void register_widget (Widget* wid){
        widgets.push_back(wid);
    }

    virtual ~Widget_manager ();


    protected:
    
    std::vector<Widget*> widgets;

};




union Data_for_controller{
    Data_for_controller() {}
    
    Data_for_controller(const Vector& vector) : 
        vector(vector)
    {}

    Data_for_controller(const double value) : 
        value(value)
    {}

    Data_for_controller(const void* ptr) : 
        const_pointer(ptr)
    {}

    Data_for_controller(void* ptr) : 
        pointer(ptr)
    {}

    Vector vector;
    double value;
    const void* const_pointer;
    void* pointer;

};



class Controller {
    public:

    virtual void operator() (const Data_for_controller& data) = 0;

    virtual ~Controller () {

    }

};


class Close_widget{
    public:

    void operator() (const Data_for_controller& data, Widget* widget){
        printf("mark for future closing %p\n", widget);
        widget->mark_close();
    }

};


class Move_widget{
    public:

    void operator() (const Data_for_controller& data, Widget* widget){
        if(!widget->get_ability_to_move()){
            return;
        }
        
        widget->move(data.vector);

    }

};


template<typename Func>
class Controller_widget: public Controller {
    public:

    Controller_widget (Widget* widget) :
        widget(widget)
    {}

    virtual void operator() (const Data_for_controller& data){
        if (widget){
            Func()(data, widget);

        }
    }

    private:

    Widget* widget;

};




class Open{
    public:

    void operator() (const Data_for_controller& data, Widget_manager* widget){
        widget->open_image((const char*)(data.const_pointer));

    }

};


template<typename Func>
class Controller_widget_manager: public Controller {
    public:

    Controller_widget_manager (Widget_manager* widget) :
        widget(widget)
    {}

    virtual void operator() (const Data_for_controller& data){
        if (widget){
            Func()(data, widget);

        }
    }

    private:
    Widget_manager* widget;





};


#endif