#pragma once

#include "WorkForce/JobDependencyTracker.hpp"
#include "WorkForce/Job.hpp"
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
			std::map<std::string, std::vector<JobDependencyTracker*>> m_consumableMap;
			// A list of jobs that consume nothing
			std::vector<Job*> m_ // TODO: How to best track once the next frame can begin to run?


			std::vector<Job*> m_jobs;
		};
	}
}