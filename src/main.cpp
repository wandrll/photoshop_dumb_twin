
#include "widget.hpp"

#include "widget_graphics.hpp"
#include "button_widget.hpp"

#include "main_window_widget.hpp"
#include "resource_handler.hpp"
#include "canvas_window.hpp"
#include "palette_widget.hpp"
#include <ctime>

#include "window_widget.hpp"
#include "level_widget.hpp"
#include "open_file.hpp"





//Tool-bar->get_active_tool()->apply(); - общая идея
//dlopen, dlclose, находим адреса через dlsym. Имена конченые, избегается через extern "C"


Resource_handler handler("resources/error.png");

Brush* brush = NULL;


int main(){
    brush = new Brush(5, {128, 128, 128});

    handler.load_resources();

    srand(time(NULL));


    Main_window_widget app(1000, 1000);

   


////////////////////////////////////////////////////////////////////////////////
    // std::shared_ptr<Brush> brush (new Brush(5, {128, 128, 128}));


    app.open_image("resources/image.jpg");

    Canvas_widget* canv1 = new Canvas_widget (800, 800, 400, 400);
    app.register_widget(canv1);

    Palette_widget* pallete = new Palette_widget ( 200, 200, 320, 490);
    app.register_widget(pallete);
    
    Spline_widget* level = new Spline_widget (100, 100, 400, 400);
    app.register_widget(level);


    Open_file_widget* field = new Open_file_widget (500, 100, 400, 300, &app);

    app.register_widget(field);


    app.run();

    
    delete brush;
    brush = NULL;

    return 0;
}