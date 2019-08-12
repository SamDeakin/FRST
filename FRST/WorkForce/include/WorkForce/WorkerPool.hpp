#pragma once

#include "WorkForce/JobDependencyTracker.hpp"
#include <map>
#include <string>
#include <vector>

namespace FRST {
	namespace WorkForce {
		class WorkerPool {
		public:
			WorkerPool();
			~WorkerPool();



		private:
			// A map of each consumable string registered to every job that consumes it.
			map<string, vector<JobDependencyTracker*>> m_consumableMap;
			// A list of jobs that consume nothing
			vector<Job*> m_ // TODO: How to best track once the next frame can begin to run?


			vector<Job*> m_jobs;
		};
	}
}