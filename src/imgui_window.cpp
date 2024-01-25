#include <imgui.h>
#include "imgui_window.hpp"

void PokeDevWindow::render() {
    if (isActive && ImGui::Begin(getName(), &isActive)) {
        renderWindow();
        ImGui::End();
    }
}

void PokeDevWindow::renderAlwaysOpen() {
    ImGui::Begin(getName());
    renderWindow();
    ImGui::End();
}