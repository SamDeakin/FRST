#pragma once

#include <string>


namespace FRST {
	namespace Atlas {
		// We want to expose this type for other files, but without forcing them to import AssetManager.hpp.
		struct AssetUUID {
			std::size_t uuid;
		};

		inline AssetUUID CreateAssetUUID(const std::string& path) {
			return AssetUUID(std::hash<std::string>()(path));
		}
	}
}


namespace std {
	template<>
	struct hash<FRST::Atlas::AssetUUID> {
		size_t operator()(const FRST::Atlas::AssetUUID& uuid) const {
			// AssetUUID's are already hashed
			return uuid.uuid;
		}
	};
}