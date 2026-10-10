#include "Config.h"

namespace Engine {

	AppConfigData Config::s_configData = {};

	void Config::Initialize(const AppConfigData& initialData) {
		s_configData = initialData;
	}

	const AppConfigData& Config::Get() {
		return s_configData;
	}

	void Config::Set(const AppConfigData& data) {
		s_configData = data;
	}

}
