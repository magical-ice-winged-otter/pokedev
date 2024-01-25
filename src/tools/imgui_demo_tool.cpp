#include "tools/imgui_demo_tool.hpp"
#include "imgui.h"

const char* ImGuiDemoTool::getName() {
    return "ImGui Demo";
}

void ImGuiDemoTool::renderWindow() {
    ImGui::ShowDemoWindow(&isActive);
}
