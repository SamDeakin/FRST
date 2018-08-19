#include <SDL2/SDL.h>
#include <tuple>

#include "FRST/IO/IOState.hpp"


namespace FRST {
	namespace IO {
		IOState::IOState() : m_changes(), m_currentState() {
		}

		IOState::IOState(const IOState& other, const std::vector<IOEvent*>& changes)
			: m_changes(changes)
			, m_currentState() {

			// Perform changes on the previous IOState
			for (auto it = other.m_currentState.cbegin(); it != other.m_currentState.cend(); it++) {
				if (!it->second->shouldMoveToNextFrame()) {
					// Copy it over
					// We can't steal the pointer because another thread may still be using it
					IOEvent* event = new IOEvent(*it->second);
					m_currentState[event->control] = event;
				}
			}

			// Loop over the changes to edit the current state to match the changes
			for (auto it = m_changes.begin(); it != m_changes.end(); it++) {
				IOEvent& change = **it;
				auto search = m_currentState.find(change.control);
				if (search == m_currentState.end()) {
					std::tie(search, std::ignore) = m_currentState.insert(
						std::make_pair(change.control, new IOEvent(change)));

					change.dx = change.x;
					change.dy = change.y;
					search->second->dx = change.dx;
					search->second->dy = change.dy;
				} else {
					IOEvent& event = *search->second;
					change.dx = event.x - change.x;
					change.dy = event.y - change.y;
					event.dx += change.dx;
					event.dy += change.dy;
					event.x = change.x;
					event.y = change.y;
				}
			}
		}

		IOState::~IOState() {
			for (auto it = m_currentState.begin(); it != m_currentState.end(); it++) {
				delete it->second;
			}
			
			for (auto it = m_changes.begin(); it != m_changes.end(); it++) {
				delete *it;
			}
		}

		const IOEvent* IOState::getState(IOEvent::Control ctrl) {
			auto it = m_currentState.find(ctrl);
			if (it != m_currentState.end()) {
				return it->second;
			}

			// Create a default state and add it to the dictionary
			// We do this for memory management reasons
			IOEvent* event = new IOEvent(ctrl);
			m_currentState[ctrl] = event;
			return event;
		}

		IOState::StateChangeIterator IOState::changesBegin() {
			return m_changes.cbegin();
		}

		IOState::StateChangeIterator IOState::changesEnd() {
			return m_changes.cend();
		}
	}
}