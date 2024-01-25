#include <vector>
#include <memory>
#include <imgui.h>
#include "imgui_window.hpp"

typedef int CpuImage; // todo: real image rep

class CpuImageFactory {
public:
    virtual CpuImage getImage() = 0;
    virtual void drawEditor() = 0;
    virtual bool isDirty() = 0;
};

class TileSource : public CpuImageFactory {
public:
    CpuImage getImage() override;
    class Id {};
};

class TilesetLayer {
public:
    class Tile : public CpuImageFactory {
    public:
        std::vector<TileSource::Id> sources{};
        int index {};
        CpuImage getImage() override;
    };
    std::vector<Tile> stacks {};
};

class Tileset : public CpuImageFactory {
public:
    int getLayerCount();
    TilesetLayer getLayer(int index);
    CpuImage getImage() override;
};

class CpuImageModifier : public CpuImageFactory {
public:
    void setInput(std::shared_ptr<CpuImage> image) {
        m_inputImage = image;
    }
protected:
    std::shared_ptr<CpuImage> m_inputImage;
};

class CpuImageModifierStack : public CpuImageModifier {
public:
    void addModifier(std::shared_ptr<CpuImageModifier> modifier) {
        m_modifiers.push_back(modifier);
    }
    void removeModifier(std::shared_ptr<CpuImageModifier> modifier) {
    }

    // we are dirty if any of our stack members are dirty.
    bool isDirty() override {
        bool isDirty {false};

        for (int i = 0; i < m_modifiers.size(); ++i) {
            isDirty |= m_modifiers[i]->isDirty();
        }

        return isDirty;
    }

    CpuImage getImage() override {
        for (int i = 0; i < m_modifiers.size(); ++i) {
        }
    }

private:
    std::vector<std::shared_ptr<CpuImageModifier>> m_modifiers;
};

void editFactoryModifiers() {
}

class TilesetEditorTool : ImGuiWindow {
public:
    inline void renderWindow() override {
    }

    inline const char* getName() override {
        return "Tileset Editor";
    };
};
