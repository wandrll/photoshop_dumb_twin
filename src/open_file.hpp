#ifndef OPEN_FILE_WIDGET_HEADER
#define OPEN_FILE_WIDGET_HEADER

#include "widget.hpp"
#include "text_field.hpp"
#include "button_widget.hpp"
#include "window_widget.hpp"


class Open_file_widget : public Window_widget{
    public:

    Open_file_widget (const int x, const int y, const int width, const int height, Widget_manager* widget);


    private:

    Widget* parent;


};



#endif
