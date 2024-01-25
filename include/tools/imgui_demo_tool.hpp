#ifndef POKEDEV_IMGUI_DEMO_TOOL_HPP
#define POKEDEV_IMGUI_DEMO_TOOL_HPP

#include "imgui_window.hpp"

class ImGuiDemoTool : public ImGuiWindow
{
public:
    void renderWindow() override;
    const char* getName() override;
};

#endif //POKEDEV_IMGUI_DEMO_TOOL_HPP 