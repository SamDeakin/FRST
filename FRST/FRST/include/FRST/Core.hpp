#pragma once

#include "SDL.h"
#include <vulkan/vulkan.hpp>

#include "Interactions/ControllerManager.hpp"
#include "Interactions/InputEvent.hpp"
#include "Interactions/WindowSystem.hpp"


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
		Interactions::WindowSystem m_ws;
		Interactions::ControllerManager m_controllerManager;

		// Whether the game is currently running
		bool m_running;
	};
}
