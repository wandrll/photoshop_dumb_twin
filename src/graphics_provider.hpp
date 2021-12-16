#ifndef GRAPHICS_PROVIDER_HEADER
#define GRAPHICS_PROVIDER_HEADER

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>

#include "vector.hpp"

#include <SFML/Graphics.hpp>

const char font_file[] = "/usr/share/fonts/TTF/Inconsolata-Regular.ttf";

bool is_mouse_left_button_pressed();
bool is_mouse_right_button_pressed();

enum class Key{
    Unknown = -1,
    a = 0,
    b,
    c,
    d, 
    e,
    f, 
    g,
    h, 
    i, 
    j, 
    k,
    l, 
    m, 
    n, 
    o,
    p, 
    q, 
    r, 
    s,
    t, 
    u, 
    v, 
    w,
    x, 
    y, 
    z, 
    Num0,
    Num1, Num2, Num3, Num4,
    Num5, Num6, Num7, Num8,
    Num9,

    Escape, LControl, LShift,
    LAlt, LSystem, RControl, RShift,
    RAlt, RSystem, Menu, LBracket,
    RBracket, Semicolon, Comma, Period,
    Quote, Slash, Backslash, Tilde,
    Equal, Hyphen, Space, Enter,
    Backspace, Tab, PageUp, PageDown,
    End, Home, Insert, Delete,
    Add, Subtract, Multiply, Divide,

    left_arrow, 
    right_arrow, 
    up_arrow, 
    down_arrow,

    Numpad0, Numpad1, Numpad2, Numpad3,
    Numpad4, Numpad5, Numpad6, Numpad7,
    Numpad8, Numpad9, F1, F2,
    F3, F4, F5, F6,
    F7, F8, F9, F10,
    F11, F12, F13, F14,
    F15, Pause, 
    
    key_count
  
};

void fill_array_of_pressed_keys(bool* array);

enum class Control_keys{
    left_ctrl = 1,
    right_ctrl = 2,
    left_shift = 4,
    right_shift = 8
};

bool is_key_pressed(Key key);

namespace Blending{
    enum class Factor{
        Zero                = sf::BlendMode::Zero,
        One                 = sf::BlendMode::One,
        Src_color           = sf::BlendMode::SrcColor,
        One_minus_src       = sf::BlendMode::OneMinusSrcColor,
        Dst_color           = sf::BlendMode::DstColor,
        One_minus_dst       = sf::BlendMode::OneMinusDstColor,
        Src_alpha           = sf::BlendMode::SrcAlpha,
        One_minus_src_alpha = sf::BlendMode::OneMinusSrcColor,
        Dst_alpha           = sf::BlendMode::DstAlpha,
        One_minus_dst_alpha = sf::BlendMode::OneMinusDstColor

    };

    enum class Equation{
        Add     = sf::BlendMode::Add,
        Sub     = sf::BlendMode::Subtract,
        Rev_sub = sf::BlendMode::ReverseSubtract
    };

}


class Blend_mode{
    public:
    Blend_mode ();
    Blend_mode (Blending::Factor src_factor, Blending::Factor dest_factor);
    Blend_mode (Blending::Factor src_color_factor, Blending::Factor dest_color_factor, Blending::Factor src_alpha_factor, Blending::Factor dest_alpha_factor, Blending::Equation color_eq, Blending::Equation alpha_eq);


    const sf::BlendMode& get_mode() const{
        return mode;
    }

    private:

    sf::BlendMode mode;

};



class Click{
    public:
    
    Click () :
        x(0),
        y(0)
    {}

    Click (int x, int y) :
        x(x),
        y(y)
    {}
    
    bool operator== (const Click& click) const{
        return this->x == click.x && this->y == click.y;
    }

    Vector operator- (const Click& click) const{
        return {this->x - click.x, this->y - click.y};
    }

    int x;
    int y;

};


class Color{
    public:

    Color ();

    Color (int red, int green, int blue, int alpha = 255);

    Color (const sf::Color& col);
    
    void print(){
        printf("red = %d  green = %d  blue = %d\n", color.r, color.g, color.b);
        // std::cout << color.r << " " << color.g << " " << color.b << std::endl;
    }

    sf::Color color;

};

static Color rand_color(){
    return {rand()%256, rand()%256, rand()%256};
}



class Window{
    public:

    Window ();

    Window (int size_x, int size_y, const char* name);

    ~Window ();



    void display();
    void clear(const Color& color = {32, 32, 32});


    Click get_mouse_position() const;

    sf::RenderWindow* get_win_ptr(){
        return this->window;
    }

    private:

    sf::RenderWindow* window;

};

class Texture;

class Drawable_object{
    public:


    Drawable_object ();
    Drawable_object (const int x, const int y);
    Drawable_object (const Vector& vec);

    virtual ~Drawable_object ();

    virtual void draw (Window& window, const int x = 0, const int y = 0, const Blend_mode& mode = {}) = 0;
    virtual void draw (Texture& texture, const int x = 0, const int y = 0, const Blend_mode& mode = {}) = 0;

    Vector get_position();

    virtual void set_position(const int x, const int y);
    virtual void set_position(const Vector& vec);



    protected:
    Vector position;


};


class Rectangle : public virtual Drawable_object{
    public:

    Rectangle ();
    Rectangle (const int x, const int y);
    Rectangle (const int x, const int y, const int size_x, const int size_y, const Color& col = {255, 0, 0}, const int thickness = 0, const Color& outline_color = {0, 0, 0, 255});

    virtual void draw (Window& window, const int x = 0, const int y = 0, const Blend_mode& mode = {});
    virtual void draw (Texture& texture, const int x = 0, const int y = 0, const Blend_mode& mode = {});


    void set_size (const int width, const int height);
    void set_color(const Color& color);

    protected:

    
    int width;
    int height;
    Color color;
    sf::RectangleShape shape;


};

class Line :public virtual Drawable_object{
    public:
    Line ();
    Line (const int x, const int y);
    Line (const int x1, const int y1, const int x2, const int y2, const Color& col = {255, 0, 0});

    virtual void draw (Window& window, const int x = 0, const int y = 0, const Blend_mode& mode = {});
    virtual void draw (Texture& texture, const int x = 0, const int y = 0, const Blend_mode& mode = {});

    

    void set_end (const int x, const int y);
    void set_color(const Color& color);

    protected:

    Vector second_position;

    Color color;
};



class Circle : public virtual Drawable_object{
    public:

    Circle ();
    Circle (const int x, const int y, const int radius = 10, const Color& col = {128, 0, 0});

    virtual void draw (Window& window, const int x = 0, const int y = 0, const Blend_mode& mode = {});
    virtual void draw (Texture& texture, const int x = 0, const int y = 0, const Blend_mode& mode = {});


    void set_size (const int radius);
    void set_color(const Color& color);

    protected:

    int radius;
    Color color;


};



class Text : public virtual Drawable_object{
    public:
    Text ();
    Text (const int x, const int y, const std::string& line, const int font_size, const Color& col = {0, 0, 0});


    virtual void draw (Window& window, const int x = 0, const int y = 0, const Blend_mode& mode = {});
    virtual void draw (Texture& texture, const int x = 0, const int y = 0, const Blend_mode& mode = {});

    void set_size (const int radius);
    void set_color(const Color& color);

    void set_string (const std::string& line);

    Vector get_boundings();

    protected:



    int font_size;
    sf::Text text;
    sf::Font font;
    Color color;

};



class Texture{
    public:
    Texture ();
    Texture (const int size_x, const int size_y);
    virtual ~Texture ();
    sf::RenderTexture* get_texture_prt() const{
        return this->texture;
    }

    bool create (const int size_x, const int size_y){
        if(!this->texture->create(size_x, size_y)){
            return false;
        }
        this->width = size_x;
        this->height = size_y;
        return true;
    }

    bool load_from_file (const std::string& filename);

    void clear(){
        texture->clear({0,0,0,0});
    }

    void display(){
        this->texture->display();
    }

    Vector get_size() const{
        return Vector(this->width, this->height);
    }

    Color* get_array (){
        display();
        Color* array = new Color[width * height];

        this->image = this->texture->getTexture().copyToImage();
        memcpy(array, this->texture->getTexture().copyToImage().getPixelsPtr(), width * height * sizeof(Color));
        
        return array;
    }

    void set_pixels(const Color* data, int x, int y, int width, int height);


    void fill_color (const Color& color);

    private:

    int width;
    int height;

    sf::RenderTexture* texture;
    sf::Image image;


};

class Sprite : public virtual Drawable_object{
    public:
    Sprite ();
    Sprite (const int x, const int y, const int size_x, const int size_y);
    ~Sprite ();

    virtual void draw (Window& window, const int x = 0, const int y = 0, const Blend_mode& mode = {});
    virtual void draw (Texture& texture, const int x = 0, const int y = 0, const Blend_mode& mode = {});

    void set_size (const int size_x, const int size_y);

    void set_position (const int x, const int y);



    void set_texture (Texture& texture);

    Texture* get_texture (){
        return this->texture;
    }

    private:

    int width;
    int height;

    sf::Sprite* sprite;
    Texture* texture;


};





#endif