#pragma once

namespace ofxNvApi{

	enum class Scope { Global, AppSpecific };

	bool initialize();
	bool isNvidiaAvailable();
	bool setThreadedOptimizationOff(Scope scope = Scope::Global);

}
