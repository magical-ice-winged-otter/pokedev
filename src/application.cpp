#include "application.hpp"
#include "platform.hpp"

ConfigFile s_config {"pokedev_porytiles_gui_config.json"};
PorytilesGui s_porytilesGui {};

void Application::init()
{
    s_config.readData(
            CUSTOM_NAME("porytilesGui", s_porytilesGui)
    );

    s_porytilesGui.init(Platform::getRenderer());
}

void Application::render()
{
    s_porytilesGui.render();
}

void Application::shutdown()
{
    s_porytilesGui.shutdown();

    s_config.writeData(
            CUSTOM_NAME("porytilesGui", s_porytilesGui)
    );
}
