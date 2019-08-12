#pragma once

#include "WorkForce/JobResult.hpp"

namespace FRST {
	namespace WorkForce {
		class Job {
		public:
			/**
			 * A Job is an action that is called once per-frame.
			 */
			Job();
			~Job();

			/**
			 * The api used for job ordering.
			 * It will be ensured that every result asked for by consumes() will be passed into
			 * execute(), and every result declared by produces() MUST be returned by execute.
			 *
			 * This will only be called once, at job queue time. They should not be dependent upon previous frame data.
			 */
			virtual const vector<const string>* produces() const = 0;
			virtual const vector<const string>* consumes() const = 0;

			/**
			 * The work that will be performed per-frame.
			 * inputs: An array of inputs of the same length and order as returned from consumes()
			 */
			virtual const JobResult* execute(const JobResult* inputs) = 0;
		};
	}
}