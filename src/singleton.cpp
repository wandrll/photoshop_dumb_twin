#include "singleton.hpp"


Singleton::Singleton () :
        tools(),
        effects(),
        resources("resources/error.png"),
        layer(nullptr),
        size(0),
        color({}),
        window(nullptr),
        app_interface(new Plugin::AppInterface),
        begin(clock())
{
    resources.load_resources();
}
