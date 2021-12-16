#include "graphics_provider.hpp"


Color::Color () :
        color(sf::Color::Cyan) {}

Color::Color (int red, int green, int blue, int alpha) :
        color(sf::Color(red, green, blue, alpha)) {}

Color::Color (const sf::Color& col) :
        color(col) {}
    




Window::Window () :
        window(new sf::RenderWindow (sf::VideoMode(300, 300), "default window", sf::Style::Fullscreen)) {
                window->setFramerateLimit(60);
        }

Window::Window (int size_x, int size_y, const char* name) :
        window(new sf::RenderWindow (sf::VideoMode(size_x, size_y), name, sf::Style::Fullscreen)) {
                window->setFramerateLimit(60);

        }

Window::~Window (){
        delete this->window;
}

void Window::display(){
        this->window->display();
}

void Window::clear(const Color& color){
        this->window->clear(color.color);
}

bool is_mouse_left_button_pressed(){
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
}

bool is_mouse_right_button_pressed(){
    return sf::Mouse::isButtonPressed(sf::Mouse::Right);
}




Click Window::get_mouse_position() const{
    sf::Vector2i position = sf::Mouse::getPosition(*(this->window));
    return {position.x, position.y};
}



Drawable_object::Drawable_object () {}


Drawable_object::Drawable_object (const int x, const int y) :
        position({x, y}) {}

Drawable_object::Drawable_object (const Vector& vec) :
        position(vec) {}


Drawable_object::~Drawable_object () {}


Vector Drawable_object::get_position(){
        return this->position;
}




Rectangle::Rectangle () : 
        width(100),
        height(100)
        {}

Rectangle::Rectangle (const int x, const int y) : 
        Drawable_object(x, y),
        width(100),
        height(100)
        {}

Rectangle::Rectangle (const int x, const int y, const int size_x, const int size_y, const Color& col, const int thickness, const Color& outline_color) : 
        Drawable_object(x, y),
        width(size_x),
        height(size_y),
        color(col),
        shape()
        {
                this->shape.setFillColor(col.color);
                this->shape.setOutlineThickness(thickness);
                this->shape.setOutlineColor(outline_color.color);
                this->shape.setPosition(x, y);
                this->shape.setSize({size_x, size_y});
        }



void Rectangle::draw (Window& window, const int x, const int y, const Blend_mode& mode){
        // sf::RectangleShape shape;
        // shape.setFillColor(this->color.color);
        // shape.setPosition(this->position.x + x, this->position.y + y);
        // shape.setSize({this->width, this->height});
        this->shape.setPosition({this->position.x + x, this->position.y + y});
        window.get_win_ptr()->draw(this->shape, sf::RenderStates(mode.get_mode()));
}


void Rectangle::draw (Texture& texture, const int x, const int y, const Blend_mode& mode){
        // sf::RectangleShape shape;
        // shape.setFillColor(this->color.color);
        // shape.setPosition(this->position.x + x, this->position.y + y);
        // shape.setSize({this->width, this->height});
        this->shape.setPosition({this->position.x + x, this->position.y + y});

        texture.get_texture_prt()->draw(this->shape, sf::RenderStates(mode.get_mode()));
}








void Rectangle::set_color (const Color& color){
        this->color = color;
        this->shape.setFillColor(color.color);
        this->shape.setOutlineColor(color.color);

}

void Rectangle::set_size(const int width, const int height){
        this->width = width;
        this->height = height;
        this->shape.setSize({width, height});
}


void Drawable_object::set_position(const int x, const int y){
        this->position.x = x;
        this->position.y = y;
}

void Drawable_object::set_position(const Vector& vec){
        this->position.x = vec.x;
        this->position.y = vec.y;
}

Circle::Circle () :
        radius(10) {}


Circle::Circle (const int x, const int y, const int radius, const Color& col) :
        Drawable_object(x, y),
        radius(radius),
        color(col) {}

void Circle::set_color (const Color& color){
        this->color = color;
}


void Circle::set_size(const int radius){
        this->radius = radius;
}



void Circle::draw(Window& window, const int x, const int y, const Blend_mode& mode){
        sf::CircleShape shape;
        shape.setFillColor(this->color.color);
        shape.setOutlineColor(color.color);


        shape.setPosition(this->position.x + x, this->position.y + y);
        shape.setRadius(this->radius);

        window.get_win_ptr()->draw(shape, sf::RenderStates(mode.get_mode()));

}

void Circle::draw(Texture& texture, const int x, const int y, const Blend_mode& mode){
        sf::CircleShape shape;
        // this->color.color.a = 64;
        shape.setFillColor(this->color.color);
        shape.setOutlineColor(color.color);
       sf::Color tmp_col = shape.getFillColor();
        // printf("color = %d %d %d %d\n", tmp_col.r, tmp_col.g, tmp_col.b, tmp_col.a);
       
        shape.setPosition(this->position.x + x, this->position.y + y);
        shape.setRadius(this->radius);

        texture.get_texture_prt()->draw(shape, sf::RenderStates(mode.get_mode()));

}


Text::Text () : 
        font_size(0),
        color(0,0,0)
{}
Text::Text (const int x, const int y, const std::string& line, const int font_size, const Color& col) : 
        Drawable_object(x, y),
        font_size(font_size),
        color(col)
{
        if(!font.loadFromFile(font_file)){
                printf("Cannot load font\n");
        }
        text.setFont(font);
        text.setString(line);
        text.setCharacterSize(this->font_size);
        text.setFillColor(this->color.color);
        text.setOutlineColor(this->color.color);
}





void Text::draw (Window& window, const int x, const int y, const Blend_mode& mode){


        text.setPosition(this->position.x + x, this->position.y + y);
        window.get_win_ptr()->draw(text, sf::RenderStates(mode.get_mode()));

}

void Text::draw (Texture& texture, const int x, const int y, const Blend_mode& mode){

        text.setPosition(this->position.x + x, this->position.y + y);
        texture.get_texture_prt()->draw(text, sf::RenderStates(mode.get_mode()));

}

void Text::set_string (const std::string& line){
        this->text.setString(line);
}






void Text::set_color (const Color& color){
        this->color = color;
}


void Text::set_size(const int font_size){
        this->font_size = font_size;
}

Vector Text::get_boundings(){

        sf::FloatRect bounds = text.getLocalBounds();

        return Vector(bounds.width, bounds.height);

}


Line::Line () : 
        Drawable_object(0, 0),
        second_position(0, 0)
{}

Line::Line (const int x1, const int y1, const int x2, const int y2, const Color& col) : 
        Drawable_object(x1, y1),
        second_position(x2, y2),
        color(col)
{}

void Line::draw (Window& window, const int x, const int y, const Blend_mode& mode){
        double length = (second_position - position).length();
    
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(length, 1));
        rectangle.setFillColor(this->color.color);
        rectangle.setOutlineColor(this->color.color);
        rectangle.setOutlineThickness(2);
        rectangle.setPosition(x + position.x, y + position.y);

        double angle = atan2(second_position.y - position.y, second_position.x - position.x) / M_PI * 180;
        rectangle.rotate(angle);
        
        window.get_win_ptr()->draw(rectangle, sf::RenderStates(mode.get_mode()));

}


void Line::draw (Texture& texture, const int x, const int y, const Blend_mode& mode){
        double length = (second_position - position).length();
    
        sf::RectangleShape rectangle;
        rectangle.setSize(sf::Vector2f(length, 1));
        rectangle.setFillColor(this->color.color);
        rectangle.setOutlineColor(this->color.color);
        rectangle.setOutlineThickness(2);
        rectangle.setPosition(x + position.x, y + position.y);

        double angle = atan2(second_position.y - position.y, second_position.x - position.x) / M_PI * 180;
        rectangle.rotate(angle);
        
        texture.get_texture_prt()->draw(rectangle, sf::RenderStates(mode.get_mode()));

}


   

void Line::set_end (const int x, const int y){
        this->second_position.x = x;
        this->second_position.y = y;
}
void Line::set_color(const Color& color){
        this->color = color;
}





Texture::Texture () :
        width(0),
        height(0),
        texture(new sf::RenderTexture)
{}

Texture::Texture (const int size_x, const int size_y):

        width(size_x),
        height(size_y),
        texture(new sf::RenderTexture){
                this->texture->create(size_x, size_y);
                sf::RectangleShape rect;
                rect.setFillColor(sf::Color::White);
                rect.setOutlineColor(sf::Color::White);
                rect.setPosition(0,0);
                rect.setSize({size_x, size_y});
                this->texture->draw(rect);
                this->image = this->texture->getTexture().copyToImage();
        }


bool Texture::load_from_file (const std::string& filename){
    sf::Texture tmp_texture;
    if (!tmp_texture.loadFromFile(filename)){
        return false;
    }

    sf::Vector2u size = tmp_texture.getSize();
    
    this->width = size.x;
    this->height = size.y;

    if(!this->texture->create(size.x, size.y)){
            return false;
    }
    
    sf::Sprite sprite;
    sprite.setTexture(tmp_texture);
    
    this->texture->draw(sprite);
    this->image = this->texture->getTexture().copyToImage();
    return true;
}


void Texture::set_pixels(const Color* data, int x, int y, int width, int height){
        sf::Texture textr;
        textr.create(width, height);
        textr.update((const sf::Uint8*)(data));

        sf::Sprite sprite;
        sprite.setTexture(textr);
        sprite.setPosition(x, y);
        
        this->texture->draw(sprite, sf::BlendMode(sf::BlendMode::Factor::One, sf::BlendMode::Factor::Zero));
        this->texture->display();
}



Texture::~Texture (){
        delete this->texture;
}

void Texture::fill_color (const Color& color){
        sf::RectangleShape rect;
        rect.setFillColor(color.color);
        rect.setPosition(0, 0);
        rect.setSize({this->width, this->height});

        this->texture->draw(rect);
}




Sprite::Sprite ():
        Drawable_object(0, 0),
        width(0),
        height(0),
        sprite(new sf::Sprite),
        texture(nullptr)
{}

Sprite::Sprite (const int x, const int y, const int size_x, const int size_y) :
        Drawable_object(x, y),
        width(size_x),
        height(size_y),
        sprite(new sf::Sprite)
{}

Sprite::~Sprite (){
        delete this->sprite;
}


void Sprite::draw (Window& window, const int x, const int y, const Blend_mode& mode){
        this->texture->display();

        // int orig_x = this->position.x;
        // int orig_y = this->position.y;
        this->sprite->setPosition(x, y);

        window.get_win_ptr()->draw(*(this->sprite), sf::RenderStates(mode.get_mode()));

        // this->sprite->setPosition(orig_x, orig_y);
}

void Sprite::draw (Texture& texture, const int x, const int y, const Blend_mode& mode){
        this->texture->display();
        // int orig_x = this->position.x;
        // int orig_y = this->position.y;
        // this->sprite->setPosition(x + orig_x, y + orig_y);
        this->sprite->setPosition(x, y);

        

        texture.get_texture_prt()->draw(*(this->sprite), sf::RenderStates(mode.get_mode()));
        // this->sprite->setPosition(orig_x, orig_y);
}





void Sprite::set_texture (Texture& texture){
    this->sprite->setTexture(texture.get_texture_prt()->getTexture());
    this->texture = &texture;
    Vector size = texture.get_size();
//     this->sprite->scale(1, -1);

    this->width = size.x;
    this->height = size.y;


    this->sprite->setPosition(this->position.x, this->position.y + this->height);
    this->position.y += this->height;
}





void Sprite::set_size (const int size_x, const int size_y){
    this->position.y -= this->height;
    
    if (this->width == 0 || this->height == 0){
        return;
    }


    this->sprite->scale(((double)size_x)/((double)this->width), ((double)size_y)/((double)this->height));
    this->width = size_x;
    this->height = size_y;

//     this->position.y += this->height;
}

void Sprite::set_position (const int x, const int y){
    this->position.x = x;
    this->position.y = y;
    
    this->sprite->setPosition(x,y);
}


bool is_key_pressed(Key key){
        return sf::Keyboard::isKeyPressed(sf::Keyboard::Key(int(key)));
}




Blend_mode::Blend_mode () : 
        mode()
{}


Blend_mode::Blend_mode (Blending::Factor src_factor, Blending::Factor dst_factor) : 
        mode(sf::BlendMode::Factor(src_factor),  sf::BlendMode::Factor(dst_factor))
{}

Blend_mode::Blend_mode (Blending::Factor src_color_factor, Blending::Factor dest_color_factor, Blending::Factor src_alpha_factor, Blending::Factor dest_alpha_factor, Blending::Equation color_eq, Blending::Equation alpha_eq) : 
        mode(sf::BlendMode::Factor(src_color_factor),
             sf::BlendMode::Factor(dest_color_factor),
             sf::BlendMode::Equation(color_eq),
             sf::BlendMode::Factor(src_alpha_factor),
             sf::BlendMode::Factor(dest_alpha_factor),
             sf::BlendMode::Equation(alpha_eq)){
}




