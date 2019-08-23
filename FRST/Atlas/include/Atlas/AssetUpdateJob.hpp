#pragma once

#include <WorkForce/Job.hpp>
#include "Atlas/AssetManager.hpp"


namespace FRST {
	namespace Atlas {
		class AssetUpdateJob : public WorkForce::Job {
			/*
			 * A task to update an asset in some way.
			 */
		public:
			AssetUpdateJob(AssetManager& assetManager)
				: m_assetManager(assetManager) {}
			virtual ~AssetUpdateJob() {}

		protected:
			AssetManager& m_assetManager;
		};
	}
}