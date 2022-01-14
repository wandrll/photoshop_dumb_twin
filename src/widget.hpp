#ifndef WIDGET_HEADER
#define WIDGET_HEADER

#include <vector>
#include "graphics_provider.hpp"
#include "event.hpp"
#include "mouse.hpp"
#include "keyboard.hpp"

class Widget {
    public:

    

    virtual ~Widget (){

    }

    virtual void draw (const int x, const int y, Texture& window) = 0;
    
    virtual void on_tick (double dt) {}


    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event);
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event);

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event);
    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event);

    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event);

    virtual bool on_key_press (const Event::Press_key& event);
    virtual bool on_key_release (const Event::Release_key& event);
    virtual bool on_text_enter (const Event::Text_enter& event);

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

    void unmark_close(){
        this->mark_for_delete = false;
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

    bool is_active_(){
        return is_active;
    }

    void set_focus (){
        focus = true;
    }

    void set_active (bool flag){
        is_active = flag;
    }

    protected: 

    Widget (const int x, const int y, const int width, const int height) :
        x(x),
        y(y),
        width(width),
        height(height),
        mark_for_delete(false),
        is_moveable(false),
        is_active(true),
        focus(false)
    {}


    int x;
    int y;

    int width;
    int height;

    bool is_active;
    bool mark_for_delete;
    bool is_moveable;

    bool focus;


};


class Widget_manager : virtual public Widget {
    public:

    Widget_manager (const int x, const int y, const int width, const int height) :
        Widget(x, y, width, height),
        is_accept_events(true)
    {}
    

    virtual void draw (const int x, const int y, Texture& window) override;

    virtual void on_tick (double dt) override;

    virtual bool on_mouse_press (const int x, const int y, const Event::Left_Mouse_press& event) override;
    virtual bool on_mouse_release (const int x, const int y, const Event::Mouse_release& event) override;

    virtual bool on_mouse_pressed_move (const int x, const int y, const Event::Mouse_pressed_move& event) override;
    virtual bool on_mouse_released_move (const int x, const int y, const Event::Mouse_released_move& event) override;
    
    virtual bool on_right_mouse_press (const int x, const int y, const Event::Right_Mouse_press& event) override;

    virtual bool on_key_press (const Event::Press_key& event) override;
    virtual bool on_key_release (const Event::Release_key& event) override;
    virtual bool on_text_enter (const Event::Text_enter& event) override;

    void open_image (const std::string& name);

    void set_event_policy(bool rule){
        this->is_accept_events = rule;
    }

    void register_widget (Widget* wid){
        widgets.push_back(wid);
    }

    virtual ~Widget_manager ();


    void delete_widget(Widget* widget);

    const std::vector<Widget*>& get_widgets()const{
        return widgets;
    }

    protected:
    bool is_accept_events;
    std::vector<Widget*> widgets;

};


class Widget_event_reciever :public Widget_manager{
    public:
    Widget_event_reciever (const int x, const int y, const int width, const int height);

    void run();
    void set_mouse(Window* window);

    protected:
    Mouse mouse;
    Keyboard keyboard;
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

    Data_for_controller(int value) :
        int_value(value)
    {}

    Vector vector;
    double value;
    const void* const_pointer;
    void* pointer;
    int int_value;

};



class Abstract_controller {
    public:

    virtual void operator() (const Data_for_controller& data) = 0;

    virtual ~Abstract_controller () {

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

class Set_accept_events{
    public:

    void operator() (const Data_for_controller& data, Widget_manager* widget){
        // printf("mark for future closing %p\n", widget);
        widget->set_event_policy(true);
    }

};

class Set_refuse_events{
    public:

    void operator() (const Data_for_controller& data, Widget_manager* widget){
        // printf("mark for future closing %p\n", widget);
        widget->set_event_policy(false);
    }

};


template<typename Func, typename Reciever>
class Controller: public Abstract_controller {
    public:

    Controller (Reciever* widget) :
        widget(widget)
    {}

    virtual void operator() (const Data_for_controller& data){
        if (widget){
            Func()(data, widget);

        }
    }

    private:

    Reciever* widget;

};




class Open{
    public:

    void operator() (const Data_for_controller& data, Widget_manager* widget){
        widget->open_image((const char*)(data.const_pointer));

    }

};



#endif