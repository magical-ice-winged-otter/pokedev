#include "tileset/source_tile_browser.hpp"
#include <imgui.h>

using namespace PokeDev;

SourceTileBrowser::SourceTileBrowser(std::shared_ptr<SourceTileCollection> tileCollection)
    : m_tileCollection{tileCollection}, m_activeEditors{}
{
}

const char* SourceTileBrowser::getName()
{
    return "Tile Browser";
}

void SourceTileBrowser::renderWindow()
{
    if (ImGui::Button("New Tile"))
    {
        m_tileCollection->push_back(std::shared_ptr<SourceTile>{new SourceTile{}});
    }

    // what follows is wierd imgui math, look at `imgui_demo.cpp:3015` for the copypaste
    float windowVisiblex2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    ImGuiStyle &style = ImGui::GetStyle();

    for (int i = 0; i < m_tileCollection->size(); ++i)
    {
        std::shared_ptr<SourceTile> tile = m_tileCollection->at(i);

        if (ImGui::ImageButton(tile->getTexture(), ImVec2(static_cast<float>(tile->getSurface()->w), static_cast<float>(tile->getSurface()->h))))
        {
            SourceTileEditor newEditor{tile};
            newEditor.isActive = true;
            m_activeEditors.push_back(newEditor);
        }

        float lastButtonx2 = ImGui::GetItemRectMax().x;
        float nextButtonx2 = lastButtonx2 + style.ItemSpacing.x + static_cast<float>(tile->getSurface()->w);
        if (i + 1 < m_tileCollection->size() && nextButtonx2 < windowVisiblex2)
        {
            ImGui::SameLine();
        }
    }

    for (int i = m_activeEditors.size() - 1; i >= 0; i--)
    {
        ImGui::PushID(i);
        m_activeEditors[i].render();

        if (!m_activeEditors[i].isActive)
        {
            m_activeEditors.erase(m_activeEditors.begin() + i);
        }
        ImGui::PopID();
    }
}