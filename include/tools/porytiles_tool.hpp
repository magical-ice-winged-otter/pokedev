#ifndef POKEDEV_PORYTILES_TOOL_HPP
#define POKEDEV_PORYTILES_TOOL_HPP

#include "pokedev_tool.hpp"
#include "porytiles/porytiles_gui.hpp"

class PorytilesTool : public PokeDevTool
{
public:
    PorytilesTool(SDL_Renderer* renderer);
    void renderWindow() override;
private:
    PorytilesGui gui {};
    bool compilePrimaryActive {};
    bool compileSecondaryActive {};
    bool decompilePrimaryActive {};
    bool decompileSecondaryActive {};
    bool settingsActive {};
};

#endif // POKEDEV_PORYTILES_TOOL_HPP