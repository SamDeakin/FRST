#pragma once

#include <FRST/WorkForce/Task.hpp>
#include "FRST/Atlast/AssetLoadJob.hpp"


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