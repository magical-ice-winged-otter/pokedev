#include <cstdio>
#include <imgui.h>
#include <nfd.h>

namespace PorytilesGui
{
    void render()
    {
        ImGui::ShowDemoWindow();

        if (ImGui::Button("Open file dialog"))
        {
            printf("opening file dialog\n");
            nfdchar_t *outPath = nullptr;
            nfdresult_t result = NFD_OpenDialog(nullptr, nullptr, &outPath);

            if (result == NFD_OKAY)
            {
                printf("selected %s", outPath);
                delete outPath;
            }
            else if (result == NFD_CANCEL)
            {
                printf("cancelled operation\n");
            }
        }
    }
}
