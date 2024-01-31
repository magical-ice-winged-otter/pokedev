#include <imgui.h>
#include <memory>
#include "tileset/tileset.hpp"
#include "tileset/tileset_editor.hpp"
#include "tileset/tileset_browser.hpp"

using namespace PokeDev;

TilesetBrowser::TilesetBrowser(std::shared_ptr<TilesetCollection> tilesetCollection)
    : m_tilesetCollection{tilesetCollection}, m_activeEditors{}
{
}

const char* TilesetBrowser::getName()
{
    return "Tileset Browser";
}

void TilesetBrowser::renderWindow()
{
    if (ImGui::Button("Import Tileset"))
    {
        // todo: import tileset
    }
    if (ImGui::Button("New Tileset"))
    {
        m_tilesetCollection->push_back(std::shared_ptr<Tileset>{new Tileset{}});
    }

    for (int i = 0; i < m_tilesetCollection->size(); ++i)
    {
        std::shared_ptr<Tileset> tileset = m_tilesetCollection->at(i);

        ImGui::Text("Tileset");
        ImGui::SameLine();

        if (ImGui::Button("Edit"))
        {
            TilesetEditor newEditor{tileset};
            newEditor.isActive = true;
            m_activeEditors.push_back(newEditor);
        }
    }

    for (int i = m_activeEditors.size() - 1; i >= 0; i--)
    {
        m_activeEditors[i].render();

        if (!m_activeEditors[i].isActive)
        {
            m_activeEditors.erase(m_activeEditors.begin() + i);
        }
    }
}