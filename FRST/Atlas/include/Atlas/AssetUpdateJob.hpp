#pragma once

#include <FRST/WorkForce/Task.hpp>
#include "FRST/Atlast/AssetManager.hpp"


namespace FRST {
	namespace Atlas {
		class AssetUpdateJob : public WorkForce::Task {
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