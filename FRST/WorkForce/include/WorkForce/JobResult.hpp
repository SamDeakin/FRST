#pragma once

namespace FRST {
	namespace WorkForce {
		class JobResult {
		public:
			/**
			 * A generic wrapper for data to be passed between jobs.
			 * We use this class as a base so we can ensure things like proper memory management.
			 */
			JobResult() {}
			virtual ~JobResult() {}

			/**
			 * A helper method to downcast into whatever real data is encapsulated.
			 * We prefer casting here instead of accessor methods for each type, because we cannot
			 * possibly think of every kind of data a user may want to store here.
			 */
			template<class T>
			T* GetData() {
				#ifdef _DEBUG
					T* data dynamic_cast<T>(this);
					assert(data);
					return data;
				#else
					return reinterpret_cast<T>(this);
				#endif
			}
		};
	}
}