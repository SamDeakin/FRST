#pragma once

#include <string>
#include <unordered_map>

#include "FRST/Atlas/Asset.hpp"
#include "FRST/Atlas/AssetUUID.hpp"


namespace FRST {
	namespace Atlas {
		class AssetManager {
			/*
			 * The AssetManager is a framework for managing access/loading/processing of backing raw assets
			 * such as meshs, textures, shaders, etc.
			 *
			 * Before loading, an asset should be referred to as a string path within the Data folder, such that
			 * "Data/textures/tree/alpha.png" would be "textures/tree/alpha.png".
			 * After loaded, an asset will be given a UUID to be used as a faster reference method. Generally this
			 * will be a hashed value of the string suitable for use in a hashtable, for quick lookup reasons.
			 */
		public:
			const Asset& getAsset(AssetUUID uuid);
		private:
			std::unordered_map<std::string, AssetUUID> pathUUIDMap;
			std::unordered_map<AssetUUID, Asset*, DontHash> assetMap;
		};
	}
}