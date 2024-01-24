#include "tools/porytiles_tool.hpp"
#include "imgui.h"

PorytilesTool::PorytilesTool(SDL_Renderer* renderer) {
    name = "Porytiles";
    gui.init(renderer);
}

#define RENDER_SUB_WINDOW(name, isActive, renderFunc) \
    if (ImGui::Button(name)) { \
        isActive = true; \
    } \
    if (isActive) {\
        gui.renderFunc(&isActive); \
    }\

void PorytilesTool::renderWindow() {
    RENDER_SUB_WINDOW("Open Primary Compiler", compilePrimaryActive, drawPrimaryCompilerWindow)
    RENDER_SUB_WINDOW("Open Secondary Compiler", compileSecondaryActive, drawSecondaryCompilerWindow)
    RENDER_SUB_WINDOW("Open Primary Decompiler", decompilePrimaryActive, drawPrimaryDecompilerWindow)
    RENDER_SUB_WINDOW("Open Secondary Decompiler", decompileSecondaryActive, drawSecondaryDecompilerWindow)

    if (ImGui::Button("Open Settings")) {
        settingsActive = true;
    }
    if (settingsActive) {
        if (ImGui::Begin("Porytiles Settings", &settingsActive)) {
            gui.drawSettings();
            ImGui::End();
        }
    }
}
