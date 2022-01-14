#ifndef PLUGIN_HPP
#define PLUGIN_HPP

#include "../plugins/plugin_std.hpp"
#include "graphics_provider.hpp"
#include "widget.hpp"
#include "window_widget.hpp"
#include "button_widget.hpp"


class Plugin_manager{
    public:

    ~Plugin_manager ();

    void load_dlls(const std::string& path);

    void deinit_plugins ();

    void on_tick(double dt);

    private:

    std::vector<void*> dlls;

    std::vector<PUPPY::PluginInterface*> plugins;

};


namespace Plugin{


class RenderTarget : public PUPPY::RenderTarget{
    public:

    RenderTarget (Texture* texture, bool delete_flag = false) :
        texture(texture),
        delete_flag(delete_flag)
    {}

    virtual ~RenderTarget() {
        if (delete_flag){
            delete texture;
        }
    }

    virtual PUPPY::RenderTarget *get_copy() const override;

    virtual PUPPY::Vec2s get_size() const override;

    virtual PUPPY::RGBA get_pixel(size_t x, size_t y) const override;
    virtual void set_pixel(size_t x, size_t y, const PUPPY::RGBA &color) override;

    virtual PUPPY::RGBA *get_pixels() const override;

    virtual void clear(const PUPPY::RGBA &color = 0) override; // fills the target with `color`

// render
    virtual void render_circle   (const PUPPY::Vec2f &position, float radius, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode = {}) override;
    virtual void render_line     (const PUPPY::Vec2f &start, const PUPPY::Vec2f &end, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode = {}) override;
    virtual void render_triangle (const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::Vec2f &p3, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode = {}) override;
    virtual void render_rectangle(const PUPPY::Vec2f &p1, const PUPPY::Vec2f &p2, const PUPPY::RGBA &color, const PUPPY::RenderMode &render_mode = {}) override;
    
    virtual void render_texture(const PUPPY::Vec2f &position, const PUPPY::RenderTarget *texture, const PUPPY::RenderMode &render_mode = {}) override;
    virtual void render_pixels (const PUPPY::Vec2f &position, const PUPPY::Vec2s &size, const PUPPY::RGBA *data, const PUPPY::RenderMode &render_mode = {}) override;

    virtual void apply_shader(const PUPPY::Shader *shader) override;

    void set_texture(RenderTarget* texture_);

    Texture* get_texture() const{
        return texture;
    }

    Texture* texture;
    private:
    bool delete_flag;
    
  
};





struct AppInterface : public PUPPY::AppInterface{
    AppInterface ();

// extension
    // enables specified extension
    virtual bool ext_enable(const char *name) const override;

    // returns given function, if it is implemented in the specified (enabled) extension
    virtual void *ext_get_func(const char *extension, const char *func) const override;
    
    // returns given interface, if it is implemented in the specified (enabled) extension
    virtual void *ext_get_interface(const char *extension, const char *name) const override;

    // registers plugin as the holder of functions and interfaces for an extension
    virtual void ext_register_as(const char *extension) const override;

// general
    virtual void log(const char *fmt, ...) const override;
    virtual double get_absolute_time()     const override;

    virtual PUPPY::RGBA get_color() const override;
    virtual float get_size() const override;

    virtual void set_color(const PUPPY::RGBA &color) const override;
    virtual void set_size(float size) const override;

    virtual const std::vector<PUPPY::WBody> get_windows() const override;
    virtual PUPPY::Widget *get_root_widget() const override;

// target
    virtual PUPPY::RenderTarget *get_target()  const override; // returns actual active  layer, drawing in it changes app's layer
    virtual PUPPY::RenderTarget *get_preview() const override; // returns actual preview layer, drawing in it changes app's layer
    virtual void flush_preview()        const override;

    virtual ~AppInterface() {}

    private:
    

};


struct RenderTargetFactory : public PUPPY::RenderTargetFactory {
    virtual ~RenderTargetFactory() {}

    virtual PUPPY::RenderTarget *spawn(const PUPPY::Vec2s &size, const PUPPY::RGBA &color = {0, 0, 0, 255}) const override;
    virtual PUPPY::RenderTarget *from_pixels(const PUPPY::Vec2s &size, const PUPPY::RGBA *data) const override;
    virtual PUPPY::RenderTarget *from_file(const char *path) const override;
};





struct WidgetFactory : public PUPPY::WidgetFactory {
    virtual ~WidgetFactory() {}

    virtual PUPPY::Button      *button       (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Button      *button       (const PUPPY::Vec2f &pos, const char *caption, PUPPY::Widget *parent = nullptr) const override; // button fit to contain caption
    virtual PUPPY::Slider      *slider       (PUPPY::Slider::Type type, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::TextField   *text_field   (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Window      *window       (const char *name, const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::ColorPicker *color_picker (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Label       *label        (const PUPPY::Vec2f &pos, const char *text, PUPPY::Widget *parent = nullptr) const override;
    virtual PUPPY::Widget      *abstract     (const PUPPY::WBody &body, PUPPY::Widget *parent = nullptr) const override;
};



class AbstractWidget : virtual public PUPPY::Widget{
    public:

    virtual ~AbstractWidget();

    AbstractWidget (::Widget* widget);

    virtual void set_position(const PUPPY::Vec2f &position_) override;
    virtual void set_size(const PUPPY::Vec2f &size_) override;

    virtual PUPPY::WBody get_body() override;
    virtual void set_body(const PUPPY::WBody &body_) override;

    virtual PUPPY::Widget *get_parent() const override;
    virtual void set_parent(PUPPY::Widget *parent_) override;

    virtual PUPPY::RenderTarget *get_texture() override;
    virtual void set_texture(PUPPY::RenderTarget *texture_) override;

    virtual bool is_active() override;
    virtual bool is_inside(const PUPPY::Vec2f &pos) override;

    virtual bool add_child(PUPPY::Widget *child) override;

    virtual void set_to_delete() override; // set to true -> app will try to delete it as soon as possible from its side
                                      // after once setting to true you can not use this widget anymore, it can 
                                      // already be deleted
    virtual bool delete_child(PUPPY::Widget *child) override;
    virtual bool delete_from_parent() override;
    
    virtual void on_render          (const PUPPY::Event::Render          &event) { }
    virtual void on_tick            (const PUPPY::Event::Tick            &event) { }
    virtual void on_mouse_press     (const PUPPY::Event::MousePress      &event) { }
    virtual void on_mouse_release   (const PUPPY::Event::MouseRelease    &event) { }
    virtual void on_mouse_move      (const PUPPY::Event::MouseMove       &event) { }
    virtual void on_key_down        (const PUPPY::Event::KeyDown         &event) { }
    virtual void on_key_up          (const PUPPY::Event::KeyUp           &event) { }
    virtual void on_text_enter      (const PUPPY::Event::TextEnter       &event) { }
    virtual void on_scroll          (const PUPPY::Event::Scroll          &event) { }
    virtual void on_hide            (const PUPPY::Event::Hide            &event) { }
    virtual void on_show            (const PUPPY::Event::Show            &event) { }

    virtual void hide()  override;
    virtual void show()  override;
    virtual void focus() override;

    virtual void set_caption(const char *text, size_t font_size, const PUPPY::Vec2f *pos = nullptr) override;
    virtual void set_base_color(const PUPPY::RGBA &color) override;

    virtual void *get_extra_data(void *arg) override;

    ::Widget* get_app_widget () const;

    protected:

    ::Widget* widget;
    PUPPY::Widget* parent;
    RenderTarget* texture;

};




class Widget_manager : public virtual AbstractWidget{
    public:

    virtual ~Widget_manager(){}

    Widget_manager (::Widget_manager* widget);

    

    virtual bool add_child(PUPPY::Widget *child) override;
    virtual bool delete_child(PUPPY::Widget *child) override;
   
    
    virtual void on_render          (const PUPPY::Event::Render          &event) override{};
    virtual void on_tick            (const PUPPY::Event::Tick            &event) override{};
    virtual void on_mouse_press     (const PUPPY::Event::MousePress      &event) override{};
    virtual void on_mouse_release   (const PUPPY::Event::MouseRelease    &event) override{};
    virtual void on_mouse_move      (const PUPPY::Event::MouseMove       &event) override{};
    virtual void on_key_down        (const PUPPY::Event::KeyDown         &event) override{};
    virtual void on_key_up          (const PUPPY::Event::KeyUp           &event) override{};
    virtual void on_text_enter      (const PUPPY::Event::TextEnter       &event) override{};
    virtual void on_scroll          (const PUPPY::Event::Scroll          &event) override{};
    virtual void on_hide            (const PUPPY::Event::Hide            &event) override{};
    virtual void on_show            (const PUPPY::Event::Show            &event) override{};


    void set_manager (::Widget_manager* widget_manager){
        this->widget_manager = widget_manager;
    }


    protected:
    ::Widget_manager* widget_manager;
};




class Window : public Widget_manager, public PUPPY::Window{
    public:

    Window (::Window_widget* widget);

    virtual void set_show_handler(HandlerType &handler_){
        show = handler_;
    }

    virtual HandlerType &get_show_handler(){
        return show;
    }

    virtual void set_hide_handler(HandlerType &handler_){
        hide = handler_;
    }

    virtual HandlerType &get_hide_handler(){
        return hide;
    }

    virtual bool set_name(const char *name){}
    virtual const char *get_name(){
        return name;
    }

    private:
    ::Window_widget* window;
    HandlerType show;
    HandlerType hide;
    const char* name = "default";
};




class Button : public AbstractWidget, public PUPPY::Button{
    public:

    Button (::Text_button* button);

    virtual void set_handler(const HandlerType &handler_){
        func = handler_;
    }

    virtual HandlerType &get_handler(){
        return func;
    }
    
    void proceed_handler (){
        func();
    }

    virtual void set_caption(const char *text, size_t font_size, const PUPPY::Vec2f *pos = nullptr) override{
        PUPPY::Vec2f vec = {};
        if (pos){
            vec = *pos;
        }
        button->set_caption(text, font_size, Vector(vec.x, vec.y));
    }

    private:
    ::Text_button* button;
    HandlerType func;

};






}


#endif
