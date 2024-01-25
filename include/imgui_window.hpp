#ifndef POKEDEV_IMGUI_WINDOW_HPP
#define POKEDEV_IMGUI_WINDOW_HPP

class ImGuiWindow
{
public:
    virtual ~ImGuiWindow() {};
    virtual const char* getName() = 0;
    void render();
    bool isActive;

protected:
    virtual void renderWindow() = 0;
};

#endif // POKEDEV_IMGUI_WINDOW_HPP