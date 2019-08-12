#pragma once

#include "WorkForce/JobResult.hpp"
#include <map>
#include <memory>
#include <string>
#include <vector>


namespace FRST {
	namespace WorkForce {
		class JobDependencyTracker {
		public:
			/**
			 * A wrapper to track the dependencies of a job and trigger the job to run when they are satisfied.
			 * A job will not be triggered in a frame if all of its dependencies are not satisfied.
			 * Dependencies will be held by many trackers at a time, and as such the trackers are not responsible
			 * for their memory management.
			 */
			JobDependencyTracker();
			~JobDependencyTracker();

			/**
			 * Resets the gathered job results for the next frame.
			 * Pointes to held results will not be deleted.
			 * The returned vector should be used when running the associated job.
			 *
			 * reset() may be called anytime after addResult returns true, and will contain the most up-to-date
			 * version of each result.
			 */
			std::unique_ptr<const std::vector<const JobResult*>> reset();

			/**
			 * Add a new result for this job.
			 * Returns true when the job should be queueud to run.
			 * Overwrites the old result for resultName if one was already present.
			 */
			bool addResult(const std::string& resultName, const JobResult* result);
		private:
			// A map of job results to their indices in m_jobResults.
			const std::map<std::string, size_t> m_jobResultIndexMap;
			// The saved job results for this job.
			// Size will not change. Unsatisfied indices will be nullptr until they are satisfied.
			std::vector<const JobResult*>* m_jobResults;

			// To ensure a job is only run once per frame.
			bool m_hasRunThisFrame;
		};
	}
}
