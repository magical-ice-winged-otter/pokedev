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
                cereal::make_nvp("porytilesContext", s_ctx),
                cereal::make_nvp("commandGenerator", s_commandGenerator),
                cereal::make_nvp("showPrimaryCompilerTool", s_showPrimaryCompilerTool),
                cereal::make_nvp("showPrimaryDecompilerTool", s_showPrimaryDecompilerTool),
                cereal::make_nvp("showSecondaryCompilerTool", s_showSecondaryCompilerTool),
                cereal::make_nvp("showSecondaryDecompilerTool", s_showSecondaryDecompilerTool),
                cereal::make_nvp("defaultSourcePath", s_defaultSourcePath),
                cereal::make_nvp("defaultOutputPath", s_defaultOutputPath)
        );
    }

private:
    PorytilesContext s_ctx{};
    PorytilesCommandGenerator s_commandGenerator {};
    bool s_showPrimaryCompilerTool {};
    bool s_showPrimaryDecompilerTool {};
    bool s_showSecondaryDecompilerTool {};
    bool s_showSecondaryCompilerTool {};
    std::filesystem::path s_defaultSourcePath {};
    std::filesystem::path s_defaultOutputPath {};
    ImVec4 s_errorTextColor {1.0, 0.3, 0.3, 1};
    SDL_Texture* s_previewTexture {};
    SDL_Renderer* s_renderer {};
};

#endif // PORYTILES_GUI_HPP