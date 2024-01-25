#ifndef POKEDEV_IMGUI_WINDOW_HPP
#define POKEDEV_IMGUI_WINDOW_HPP

class PokeDevWindow
{
public:
    virtual ~PokeDevWindow() {};
    virtual const char* getName() = 0;
    void render();
    void renderAlwaysOpen();
    bool isActive;

protected:
    virtual void renderWindow() = 0;
};

#endif // POKEDEV_IMGUI_WINDOW_HPP