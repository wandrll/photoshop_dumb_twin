
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
#include "singleton.hpp"

#include "settings_widget.hpp"
#include "drop_down_box.hpp"

#include <dlfcn.h>
#include <unistd.h>

//Tool-bar->get_active_tool()->apply(); - общая идея
//dlopen, dlclose, находим адреса через dlsym. Имена конченые, избегается через extern "C"
//Размытие: - усреднение пикселей, квадрат 3х3 пикселя, считаем среднюю яркость, записываемв центральный пиксель усреднение по всем каналам. Оптимальнее всего - круг матрица коэффициентов фильтрация работает от матрицы коэф-ов нужно накапливать суммму коэффициентов для нормализации яркости
//Увелечение резкости - через размытие - берем сренюю яркость I', вычитаем яркость оригинального - на сколько отличается. Эту разность умножаем на коеф и прибавляем к оригинальной. (I' - I)A - I. Одна функция на рзекость и блюр. unsharp mask.  


Singleton* global_singleton = NULL;



int main(){

    




    global_singleton = new Singleton;


    srand(time(NULL));


    Main_window_widget app(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

    global_singleton->set_main_window(&app);

    Brush* brush = new Brush (0);   
    global_singleton->get_tools()->add_tool(brush);
    Eraser* eraser = new Eraser (10);
    global_singleton->get_tools()->add_tool(eraser);

    Plugin_manager plugins;
    plugins.load_dlls("plugin_dll");

    

    Tools_widget* tools = new Tools_widget(100, 100, 300, 800);
    Effects_widget* effects = new Effects_widget (1500, 500, 300, 500);


    const int settings_width = 350;
    Settings_widget* settings = new Settings_widget (0, TOP_BAR_HEIGHT, settings_width, RESOLUTION_HEIGHT - TOP_BAR_HEIGHT);

    Palette_widget* pallete = new Palette_widget ( 0, 0, settings_width, 490);
    settings->register_widget(pallete);

    app.register_widget(settings);

    app.register_widget(tools);
    app.register_widget(effects);

////////////////////////////////////////////////////////////////////////////////
    // std::shared_ptr<Brush> brush (new Brush(5, {128, 128, 128}));


    app.open_image("resources/image.jpg");



    // app.register_widget(pallete);
    
    Spline_widget* level = new Spline_widget (100, 100, 400, 400);
    app.register_widget(level);



    app.run();

    
    delete global_singleton;
    global_singleton = NULL;



    return 0;
}