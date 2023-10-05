#include <cstdio>
#include <imgui.h>
#include "file_dialog.hpp"
#include "porytiles_gui.hpp"
#include "serializer.hpp"

using namespace std;

namespace PorytilesGui
{
    static filesystem::path s_projectPath {};

    void init()
    {
        s_projectPath = Serializer::readPath("projectPath");
    }

    void shutdown()
    {
        Serializer::writePath("projectPath", s_projectPath);
    }

    void render()
    {
        ImGui::ShowDemoWindow();

        if (ImGui::Button("Change Path"))
        {
            if (FileDialog::tryPickFolder(s_projectPath))
            {
                printf("changed folder to %s\n", s_projectPath.string().c_str());
            }
        }

        ImGui::SameLine();
        ImGui::Text("Project path: %s", s_projectPath.string().c_str()); 
    }
}
