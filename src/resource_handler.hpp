#ifndef RESOURCE_HANDLER_HEADER
#define RESOURCE_HANDLER_HEADER

#include "graphics_provider.hpp"
#include <unordered_map>

enum class Resource_type{
    ERROR,
    EXIT,
    EXIT_HOVER,
    YES,
    NO,
    SLIDER,
    MENU_BAR,
    LEFT_MENU_BAR,
    RIGHT_MENU_BAR,
    MID_WINDOW,
    LEFT_WINDOW,
    BOTTOM_WINDOW,
    RIGHT_WINDOW,
    LEFT_BOTTOM_WINDOW,
    RIGHT_BOTTOM_WINDOW,
    MID_SLIDER_BAR,
    LEFT_SLIDER_BAR,
    RIGHT_SLIDER_BAR,

    ARROW_LEFT,
    ARROW_RIGHT,

    OK_BUTTON,
    OK_HOVERED_BUTTON,

    CANVAS_BACKGROUND,

    TYPE_CNT
};

struct Pair_type_sprite{

    Pair_type_sprite () :
        type(Resource_type::ERROR),
        sprite(NULL)
    {}
    
    Pair_type_sprite (const Resource_type type, Sprite* sprite) :
        type(type),
        sprite(sprite)
    {}


    Resource_type type;
    Sprite* sprite;

};


class Resource_handler{
    public:

    Resource_handler () = delete;

    Resource_handler (const std::string& error_file);

    ~Resource_handler ();

    Resource_handler (const Resource_handler& copy) = delete;
    void operator= (const Resource_handler& copy) = delete;

    void load_resources ();

    void load_sprite (const Resource_type type, const std::string& error_file);

    Sprite* get_sprite (const Resource_type type){
        // printf("%p return_sprite, %d type\n", this->map[(int)type].sprite, type);

        return this->map[(int)type].sprite;
    }

    private:
    int size;
    Pair_type_sprite* map;
    std::vector<Sprite*>  loaded_sprites;
    std::vector<Texture*> loaded_textures;

};





#endif