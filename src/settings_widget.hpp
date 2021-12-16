#ifndef SETTINGS_WIDGET_HEADER
#define SETTINGS_WIDGET_HEADER

#include "widget.hpp"
#include "widget_graphics.hpp"


class Settings_widget : public Widget_manager{
    public:
    Settings_widget (const int x, const int y, const int width, const int height) :
            Widget_manager (x, y, width, height){
        Rectangle_widget* background = new Rectangle_widget(0, 0, width, height, Color(15, 15, 15, 180));
        this->widgets.push_back(background);
    }

    

};





#endif