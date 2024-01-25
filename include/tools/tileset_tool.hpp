#ifndef POKEDEV_TILESET_TOOL_HPP
#define POKEDEV_TILESET_TOOL_HPP

#include <vector>
#include <memory>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include "imgui_window.hpp"
#include "imgui_utils.hpp"
#include "platform.hpp"
#include <fmt/format.h>

namespace PokeDev {
    // todo: serialize
    class SourceTile {
    public:
        SourceTile() 
            : m_surface {}
            , m_texture {}
            , m_imagePath {}
            , m_name {"DEFAULT TILE NAME"}
        {
            setSurface("missing_tile.png");
        }

        void drawEditor() {
            ImGui::Image(m_texture, ImVec2 {static_cast<float>(m_surface->w), static_cast<float>(m_surface->h)});

            if (ImGuiUtils::filePicker("Tile Image File", m_imagePath, Platform::FilePickerOptions{})) {
                setSurface(m_imagePath.string().c_str());
            }

            ImGui::InputText("Tile Name", &m_name);
        }

        SDL_Surface* getSurface() {
            return m_surface;
        }

        SDL_Texture* getTexture() {
            return m_texture;
        }

        const std::string& getName() {
            return m_name;
        }

        const std::filesystem::path& getPath() {
            return m_imagePath;
        }

    private:
        SDL_Surface* m_surface;
        SDL_Texture* m_texture;
        std::filesystem::path m_imagePath;
        std::string m_name;

        void setSurface(const char* file) {
                if (m_surface != nullptr) { SDL_FreeSurface(m_surface); }
                if (m_texture != nullptr) { SDL_DestroyTexture(m_texture); }
                m_surface = IMG_Load(file);
                m_texture = SDL_CreateTextureFromSurface(Platform::getRenderer(), m_surface);
        }
    };

    class Tileset
    {
    public:
        void drawEditor()
        {
            ImGui::Text("Edit a tileset here!");
        }
    };

    typedef std::vector<std::shared_ptr<Tileset>> TilesetCollection;
    typedef std::vector<std::shared_ptr<SourceTile>> SourceTileCollection;

    class TilesetEditor : public PokeDevWindow
    {
    public:
        TilesetEditor(std::shared_ptr<Tileset> activeTileset)
            : m_tileset{activeTileset}
        {
        }

        const char *getName() override
        {
            return "Tileset Editor";
        }

        void renderWindow() override
        {
            m_tileset->drawEditor();
        }

    private:
        std::shared_ptr<Tileset> m_tileset;
    };

    class SourceTileEditor : public PokeDevWindow
    {
    public:
        SourceTileEditor(std::shared_ptr<SourceTile> activeTile)
            : m_tile{activeTile}
            , m_name {}
        {
            m_name = fmt::format("\"{}\" Source Tile Editor", activeTile->getPath().string());
        }

        const char *getName() override
        {
            return m_name.c_str();
        }

        void renderWindow() override
        {
            m_tile->drawEditor();
        }

    private:
        std::string m_name;
        std::shared_ptr<SourceTile> m_tile;
    };

    class TilesetBrowser : public PokeDevWindow
    {
    public:
        TilesetBrowser(std::shared_ptr<TilesetCollection> tilesetCollection)
            : m_tilesetCollection{tilesetCollection}, m_activeEditors{}
        {
        }

        const char *getName() override
        {
            return "Tileset Browser";
        }

        void renderWindow() override
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

    private:
        std::vector<TilesetEditor> m_activeEditors;
        std::shared_ptr<TilesetCollection> m_tilesetCollection;
    };

    class SourceTileBrowser : public PokeDevWindow
    {
    public:
        SourceTileBrowser(std::shared_ptr<SourceTileCollection> tileCollection)
            : m_tileCollection{tileCollection}
            , m_activeEditors {}
        {
        }

        const char *getName() override
        {
            return "Tile Browser";
        }

        void renderWindow() override
        {
            if (ImGui::Button("New Tile"))
            {
                m_tileCollection->push_back(std::shared_ptr<SourceTile>{new SourceTile{}});
            }

            // what follows is wierd imgui math, look at `imgui_demo.cpp:3015` for the copypaste
            float windowVisiblex2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
            ImGuiStyle& style = ImGui::GetStyle();

            for (int i = 0; i < m_tileCollection->size(); ++i)
            {
                std::shared_ptr<SourceTile> tile = m_tileCollection->at(i);

                if (ImGui::ImageButton(tile->getTexture(), ImVec2(static_cast<float>(tile->getSurface()->w), static_cast<float>(tile->getSurface()->h)))) {
                    SourceTileEditor newEditor {tile};
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

    private:
        std::vector<SourceTileEditor> m_activeEditors;
        std::shared_ptr<SourceTileCollection> m_tileCollection;
    };

    class TilesetTool : public PokeDevWindow
    {
    public:
        TilesetTool()
            : m_tileCollection{new SourceTileCollection{}}, m_tilesetCollection{new TilesetCollection{}}, tilesetBrowser{m_tilesetCollection}, tileCollectionBrowser{m_tileCollection}
        {
            // todo: load source tiles?
            // todo: load tilesets?
        }

        void renderWindow() override
        {
            tilesetBrowser.renderAlwaysOpen();
            tileCollectionBrowser.renderAlwaysOpen();
        }

        const char *getName() override
        {
            return "Tileset Tool";
        }

    private:
        std::shared_ptr<SourceTileCollection> m_tileCollection;
        std::shared_ptr<TilesetCollection> m_tilesetCollection;
        TilesetBrowser tilesetBrowser;
        SourceTileBrowser tileCollectionBrowser;
    };
}

#endif // POKEDEV_TILESET_TOOL_HPP