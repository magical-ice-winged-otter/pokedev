#ifndef POKEDEV_IMGUI_DEMO_TOOL_HPP
#define POKEDEV_IMGUI_DEMO_TOOL_HPP

#include "pokedev_tool.hpp"

class ImGuiDemoTool : public PokeDevTool
{
public:
    ImGuiDemoTool();
    void renderWindow() override;
};

#endif //POKEDEV_IMGUI_DEMO_TOOL_HPP 