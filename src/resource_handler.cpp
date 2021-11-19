#include "resource_handler.hpp"


Resource_handler::Resource_handler (const std::string& error_file) :
    size((int)Resource_type::TYPE_CNT),
    map(new Pair_type_sprite[size]){

        Texture* error = new Texture;
        error->load_from_file(error_file);

        Sprite* error_sprite = new Sprite;
        error_sprite->set_texture(*error);



        for (int i = 0; i < size; i++){
            this->map[i].sprite = error_sprite;
        }

        this->loaded_sprites.push_back(error_sprite);
        this->loaded_textures.push_back(error);
}

Resource_handler::~Resource_handler (){
    for (int i = 0; i < this->loaded_sprites.size(); i++){
        delete this->loaded_sprites[i];
    }

    for (int i = 0; i < this->loaded_textures.size(); i++){
        delete this->loaded_textures[i];
    }

    delete[] this->map;
}

void Resource_handler::load_sprite (const Resource_type type, const std::string& file){
    this->map[(int)type].type   = type;

    Texture* texture = new Texture;
    texture->load_from_file(file);

    Sprite* sprite = new Sprite;
    sprite->set_texture(*texture);

    printf("%p loaded_sprite %d\n", sprite, type);
    this->loaded_sprites.push_back(sprite);
    this->loaded_textures.push_back(texture);

    this->map[(int)type].sprite = sprite;
}


void Resource_handler::load_resources (){
    this->load_sprite(Resource_type::EXIT               ,   "resources/exit.png");
    this->load_sprite(Resource_type::EXIT_HOVER         ,   "resources/exit_hover.png");
    this->load_sprite(Resource_type::SLIDER             ,   "resources/slider.png");
    this->load_sprite(Resource_type::MENU_BAR           ,   "resources/menubar.png");
    this->load_sprite(Resource_type::LEFT_MENU_BAR      ,   "resources/left_menu_bar.png");
    this->load_sprite(Resource_type::RIGHT_MENU_BAR     ,   "resources/right_menu_bar.png");
    this->load_sprite(Resource_type::MID_WINDOW         ,   "resources/mid.png");
    this->load_sprite(Resource_type::LEFT_WINDOW        ,   "resources/left.png");   
    this->load_sprite(Resource_type::BOTTOM_WINDOW      ,   "resources/bottom.png"); 
    this->load_sprite(Resource_type::RIGHT_WINDOW       ,   "resources/right.png");  
    this->load_sprite(Resource_type::LEFT_BOTTOM_WINDOW ,   "resources/left_bottom.png");    
    this->load_sprite(Resource_type::RIGHT_BOTTOM_WINDOW,   "resources/right_bottom.png");  
    this->load_sprite(Resource_type::ARROW_LEFT         ,   "resources/arrow-left.png");  
    this->load_sprite(Resource_type::ARROW_RIGHT        ,   "resources/arrow-right.png");  
    this->load_sprite(Resource_type::MID_SLIDER_BAR     ,   "resources/mid_slider_bar.png");  
    this->load_sprite(Resource_type::LEFT_SLIDER_BAR    ,   "resources/left_slidebar.png");  
    this->load_sprite(Resource_type::RIGHT_SLIDER_BAR   ,   "resources/right_slidebar.png");  
    this->load_sprite(Resource_type::OK_BUTTON          ,   "resources/ok/ok.png");  
    this->load_sprite(Resource_type::OK_HOVERED_BUTTON  ,   "resources/ok/ok_hovered.png");  

}
