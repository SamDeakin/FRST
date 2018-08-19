#pragma once

#include <unordered_map>
#include <vector>

#include "FRST/IO/IOEvent.hpp"


namespace FRST {
	namespace IO {
		class IOState {
			/*
			 * A class that represents the state of the user's input devices during one frame of execution.
			 * Contains both the current state of IO and changes this frame.
			 *
			 * It holds changes in itself but it queries SDL for the current state, should it be asked to get state.
			 *
			 * The suggested usage is to:
			 *		Create one each frame. This must be fast
			 *		Accumulate state changes in the frame
			 *		Pass to render function at the end of the frame
			 *
			 * A user must be able to:
			 *		Query state with Control (and possibly an extra int for control method)
			 *		Loop through events from this frame
			 */
		public:
			// Construct a new default state with no changes
			IOState();

			// Construct a new state by performing changes to other.
			// The memory created for changes be taken and managed, so it should
			// not be manually freed after calling.
			IOState(const IOState& other, const std::vector<IOEvent*>& changes);

			~IOState();

			// Receive a State object representing the most up to date state of the Control
			const IOEvent* getState(IOEvent::Control ctrl);

			// The proper interface to look at changes this frame is to iterate over them
			// They will appear in the order that they occurred
			typedef std::vector<IOEvent*>::const_iterator StateChangeIterator;
			StateChangeIterator changesBegin();
			StateChangeIterator changesEnd();
		private:
			// The changes accumulated over this frame
			std::vector<IOEvent*> m_changes;

			// The current keyboard state as of this frame
			// Optimized by only containing states that differ from the default
			std::unordered_map<IOEvent::Control, IOEvent*> m_currentState;
		};
	}
}
