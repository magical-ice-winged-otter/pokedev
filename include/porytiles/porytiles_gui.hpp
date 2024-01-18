#ifndef POKEDEV_PORYTILES_GUI_HPP
#define POKEDEV_PORYTILES_GUI_HPP

#include <imgui.h>
#include <SDL.h>
#include "porytiles_context.hpp"
#include "porytiles_command_generator.hpp"
#include "serializer.hpp"

class PorytilesGui
{
public:
    void init(SDL_Renderer* renderer);
    void shutdown();

    void drawSettings();
    void drawPrimaryCompilerWindow(bool* isOpen);
    void drawPrimaryDecompilerWindow(bool* isOpen);
    void drawSecondaryCompilerWindow(bool* isOpen);
    void drawSecondaryDecompilerWindow(bool* isOpen);

    template<class Archive>
    void serialize(Archive& archive) {
        archive(
                CUSTOM_NAME("porytilesContext", m_ctx),
                CUSTOM_NAME("commandGenerator", m_commandGenerator),
                CUSTOM_NAME("defaultSourcePath", m_defaultSourcePath),
                CUSTOM_NAME("defaultOutputPath", m_defaultOutputPath)
        );
    }

private:
    PorytilesContext m_ctx{};
    PorytilesCommandGenerator m_commandGenerator {};
    std::filesystem::path m_defaultSourcePath {};
    std::filesystem::path m_defaultOutputPath {};
    ImVec4 m_errorTextColor {1.0, 0.3f, 0.3f, 1};
    SDL_Texture* m_previewTexture {};
    SDL_Renderer* m_renderer {};
};

#endif // POKEDEV_PORYTILES_GUI_HPP
