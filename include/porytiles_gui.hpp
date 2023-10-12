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

    void renderSettings();
    void renderPrimaryCompilerWindow(bool* isOpen);
    void renderPrimaryDecompilerWindow(bool* isOpen);
    void renderSecondaryCompilerWindow(bool* isOpen);
    void renderSecondaryDecompilerWindow(bool* isOpen);

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
                CUSTOM_NAME("porytilesContext", m_ctx),
                CUSTOM_NAME("commandGenerator", m_commandGenerator),
                CUSTOM_NAME("showPrimaryCompilerTool", m_showPrimaryCompilerTool),
                CUSTOM_NAME("showPrimaryDecompilerTool", m_showPrimaryDecompilerTool),
                CUSTOM_NAME("showSecondaryCompilerTool", m_showSecondaryCompilerTool),
                CUSTOM_NAME("showSecondaryDecompilerTool", m_showSecondaryDecompilerTool),
                CUSTOM_NAME("defaultSourcePath", m_defaultSourcePath),
                CUSTOM_NAME("defaultOutputPath", m_defaultOutputPath)
        );
    }

private:
    PorytilesContext m_ctx{};
    PorytilesCommandGenerator m_commandGenerator {};
    bool m_showPrimaryCompilerTool {};
    bool m_showPrimaryDecompilerTool {};
    bool m_showSecondaryDecompilerTool {};
    bool m_showSecondaryCompilerTool {};
    std::filesystem::path m_defaultSourcePath {};
    std::filesystem::path m_defaultOutputPath {};
    ImVec4 m_errorTextColor {1.0, 0.3, 0.3, 1};
    SDL_Texture* m_previewTexture {};
    SDL_Renderer* m_renderer {};
};

#endif // POKEDEV_PORYTILES_GUI_HPP
