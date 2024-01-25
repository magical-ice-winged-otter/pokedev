#include <imgui.h>
#include "imgui_window.hpp"

void ImGuiWindow::render() {
    if (isActive && ImGui::Begin(getName(), &isActive)) {
        renderWindow();
        ImGui::End();
    }
}