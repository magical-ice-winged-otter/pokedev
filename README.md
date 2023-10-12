# pokedev
A collection of tools for creating pokemon decomp projects.

## Tools

### Porytiles Command Generator
Generates commands for Porytiles, a community-made tool for decompiling and compiling
tilesets for decomp projects.

## Compiling
Currently a work-in-progress, try it and let me know issues so this can be updated.

- The build toolchain is cmake, you can run `cmake -Bbuild -S.` in the root directory to 
  generate project files.
- To create a binary with cmake, run `cmake --build build` to compile the project.
- You'll also need vcpkg installed, and the VCPKG_ROOT environment variable set to the install location
  for package dependencies to be installed correctly.

### Dependencies
- **SDL** for primary platform abstraction layer.
- **SDL** for cross-platform rendering.
- **SDL_image** for loading images.
- **ImGui** for rendering the GUI.
- Uses C++ 20 features, such as **std::filesystem** and **std::format**
- **Cereal** for serialization (header only)

## TODO
- [ ] proper custom logging
- [ ] testing on more platforms besides windows
- [ ] ux overhaul, to support clean readability between multiple tools
- [ ] cmake bin folder, for easier packaging
- [ ] adding tooltips to porytiles gui
- [ ] more centralized settings view, separate from tool controls
- [ ] more tools...
