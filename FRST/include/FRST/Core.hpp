#pragma once

#include <SDL2/SDL.h>
#include <vulkan/vulkan.hpp>

#include "FRST/IO/ControllerManager.hpp"
#include "FRST/IO/IOEvent.hpp"
#include "FRST/IO/WindowSystem.hpp"


namespace FRST {
	class Core {
	public:
		// Throws if there is an issue
		Core(vk::Instance* instance, vk::SurfaceKHR* surface, SDL_Window* window);
		
		// dtors are nothrow
		~Core() noexcept;

		// Run the game
		void run();

		// Quit the game
		// This tells the gameloop to quit. It may actually take some time (up to a full frame) to actually quit the game.
		// The implementation will preferrably be decoupled from the job scheduling system
		// to still allow the game to quit if it deadlocks.
		void quit();

	private:
		IO::WindowSystem m_ws;
		IO::ControllerManager m_controllerManager;

		// Whether the game is currently running
		bool m_running;
	};

}
