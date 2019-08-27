# Find and build all external packages

# Some external packages must be installed elsewhere as they don't use git
find_package(SDL2 REQUIRED) # Requires SDL2 from libsdl.org
find_package(Vulkan REQUIRED) # Requires the Vulkan SDK from Lunarg.com

# Requires git submodule --init --recursive
add_subdirectory(glm)