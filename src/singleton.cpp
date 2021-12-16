#include "singleton.hpp"


Singleton::Singleton () :
        tools(),
        effects(),
        resources("resources/error.png"),
        layer(nullptr),
        size(0),
        color({}),
        window(nullptr),
        app_interface(build_app_interface())
{
    resources.load_resources();
}
