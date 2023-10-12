#include <SDL.h>
#include "porytiles_gui.hpp"
#include "platform.hpp"
#include "serializer.hpp"

using namespace Serializer;

int main(int, char*[])
{
    Platform::init();
    PorytilesGui porytilesGui {};
    ConfigFile config("pokedev_porytiles_gui_config.json");
    config.readData(nvp("porytilesGui", porytilesGui));
    porytilesGui.init(Platform::getRenderer());

    while (!Platform::wantsToQuit())
    {
        Platform::startFrame();
        porytilesGui.render();
        Platform::endFrame();
    }

    porytilesGui.shutdown();
    config.writeData(nvp("porytilesGui", porytilesGui));
    Platform::shutdown();
    return 0;
}