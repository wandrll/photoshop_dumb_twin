#ifndef EVENT_HEADER
#define EVENT_HEADER


#include "graphics_provider.hpp"
#include <iostream>


struct Move{
    Click begin_move;
    Click end_move;

    Vector get_move_vector() const{
        return end_move - begin_move;
    }

};




namespace Event{
    class Left_Mouse_press{
        public:
        Left_Mouse_press() {}
        Left_Mouse_press(const Click& click) :
            click(click)
        {}

        void print(){
            std::cout <<"left press "<< click.x << " " << click.y << std::endl;
        }

        Click click;
    };

    class Right_Mouse_press{
        public:
        Right_Mouse_press() {}
        Right_Mouse_press(const Click& click) :
            click(click)
        {}

        void print(){
            std::cout <<"right press "<< click.x << " " << click.y << std::endl;
        }

        Click click;
    };

    class Mouse_release{
        public:
        Mouse_release() {}
        Mouse_release(const Click& click) :
            click(click)
        {}

        void print(){
            std::cout <<"release "<< click.x << " " << click.y << std::endl;
        }

        Click click;
    };


    class Mouse_pressed_move{
        public:
        Mouse_pressed_move() {}
        Mouse_pressed_move(const Move& move) :
            move(move)
        {}
        
        void print(){
            std::cout <<"pressed move "<< move.begin_move.x << " " << move.begin_move.y << " " << move.end_move.x << " " << move.end_move.y << " " << std::endl;
        }

        Move move;
    };

    class Mouse_released_move{
        public:
        Mouse_released_move() {}
        Mouse_released_move(const Move& move) :
            move(move)
        {}
        void print(){
            std::cout <<"released move "<< move.begin_move.x << " " << move.begin_move.y << " " << move.end_move.x << " " << move.end_move.y << " " << std::endl;
        }

        Move move;
    };


    class Press_key{
        public:

        Press_key() :
            control_key(0)
        {}


        Key key;
        int control_key;

        void print(){
            printf("keyboard event %d  %d\n", key, control_key);
        }

    };


    class Release_key{
        public:

        Release_key() :
            control_key(0)
        {}


        Key key;
        int control_key;

        void print(){
            printf("keyboard event %d  %d\n", key, control_key);
        }

    };



    class Text_enter{
        public:
        Text_enter() : keycode('^') {}
        char keycode;

        Text_enter(char unicode) : keycode(unicode) {}

       
    };

}   











#endif