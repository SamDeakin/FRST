#pragma once

#include <WorkForce/Job.hpp>
#include "Atlas/AssetUpdateJob.hpp"


namespace FRST {
	namespace Atlas {
		class AssetLoadJob : public AssetUpdateJob {
			/*
			 * A task to load an asset.
			 */
		public:
			AssetLoadJob(AssetManager& assetManager);
			virtual ~AssetLoadJob();
		};
	}
}