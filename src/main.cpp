#include <cstdio>
#include <SDL.h>
#include <cereal/archives/json.hpp>
#include <fstream>
#include "porytiles_gui.hpp"
#include "platform.hpp"
#include "serializer.hpp"

int main(int, char*[])
{
    PorytilesGui porytilesGui {};

    // Read everything from a file
    const char* configName = "pokedev_porytiles_gui_config.json";

    if (std::filesystem::exists(configName))
    {
        std::ifstream configFile {configName};
        cereal::JSONInputArchive archive {configFile};
        archive(
                cereal::make_nvp("porytilesGui", porytilesGui)
        );
    }

    Platform::init();
    porytilesGui.init(Platform::getRenderer());

    while (!Platform::wantsToQuit())
    {
        Platform::startFrame();
        porytilesGui.render();
        Platform::endFrame();
    }

    porytilesGui.shutdown();
    Platform::shutdown();

    // Save everything to a file.
    std::ofstream configFile {configName};
    cereal::JSONOutputArchive archive {configFile};
    archive(
            cereal::make_nvp("porytilesGui", porytilesGui)
    );

    return 0;
}