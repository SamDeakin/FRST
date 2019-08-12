#pragma once

#include <string>

#include "FRST/Atlas/AssetUUID.hpp"


namespace FRST {
	namespace Atlas {
		class Asset {
		public:
		private:
			std::string m_path;
			AssetUUID m_uuid;
		};
	}
}