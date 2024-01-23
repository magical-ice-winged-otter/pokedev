#include "tools/imgui_demo_tool.hpp"
#include "imgui.h"

ImGuiDemoTool::ImGuiDemoTool() {
    name = "ImGui Demo";
}

void ImGuiDemoTool::renderWindow() {
    ImGui::ShowDemoWindow(isActive);
}
