#include "application.hpp"
#include "platform.hpp"

using namespace Serializer;

Serializer::ConfigFile s_config {"pokedev_porytiles_gui_config.json"};
PorytilesGui s_porytilesGui {};

void Application::init()
{
    s_config.readData(nvp("porytilesGui", s_porytilesGui));
    s_porytilesGui.init(Platform::getRenderer());
}

void Application::render()
{
    s_porytilesGui.render();
}

void Application::shutdown()
{
    s_porytilesGui.shutdown();
    s_config.writeData(nvp("porytilesGui", s_porytilesGui));
}
