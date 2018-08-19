#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include <deque>

#include "WindowSystem.hpp"
#include "IOEvent.hpp"


namespace FRST {
	class Core {
	public:
		// Throws if there is an issue
		Core(vk::Instance* instance, vk::SurfaceKHR* surface, SDL_Window* window);
		
		// dtors are nothrow
		~Core() noexcept;

		// Run the game
		void run();

	private:
		IO::WindowSystem ws;
		std::deque<IO::IOEvent> events;
	};

}
