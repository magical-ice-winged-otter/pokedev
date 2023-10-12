#ifndef PORYTILES_GUI_HPP
#define PORYTILES_GUI_HPP

#include <imgui.h>
#include <SDL.h>
#include "porytiles_context.hpp"
#include "porytiles_command_generator.hpp"

class PorytilesGui
{
public:
    void init(SDL_Renderer* renderer);
    void render();
    void shutdown();

    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
                cereal::make_nvp("porytilesContext", m_ctx),
                cereal::make_nvp("commandGenerator", m_commandGenerator),
                cereal::make_nvp("showPrimaryCompilerTool", m_showPrimaryCompilerTool),
                cereal::make_nvp("showPrimaryDecompilerTool", m_showPrimaryDecompilerTool),
                cereal::make_nvp("showSecondaryCompilerTool", m_showSecondaryCompilerTool),
                cereal::make_nvp("showSecondaryDecompilerTool", m_showSecondaryDecompilerTool),
                cereal::make_nvp("defaultSourcePath", m_defaultSourcePath),
                cereal::make_nvp("defaultOutputPath", m_defaultOutputPath)
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

#endif // PORYTILES_GUI_HPP