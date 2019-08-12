# Find and build all external packages

# Some external packages must be installed elsewhere as they don't use git
find_package(SDL2 REQUIRED) # Requires SDL2 from libsdl.org
find_package(Vulkan REQUIRED) # Requires the Vulkan SDK from Lunarg.com

set(EXTERNAL_INCLUDES SDL2_INCLUDE_DIR Vulkan_INCLUDE_DIRS)
set(EXTERNAL_LIBRARIES SDL2_LIBRARY Vulkan_LIBRARIES)