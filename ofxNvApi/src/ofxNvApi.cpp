#include "ofxNvApi.h"
#include "ofxNvApi.h"
#include <iostream>

#if defined(TARGET_WIN32) || defined(_WIN32) || defined(_WIN64)
#include "nvapi.h"
#include "nvapi_lite_common.h"
#include "NvApiDriverSettings.h"
#include <windows.h>
#endif

#if defined(TARGET_WIN32) || defined(_WIN32) || defined(_WIN64)

bool ofxNvApi::initialize(){

    NvAPI_Status status = NvAPI_Initialize();
    if(status != NVAPI_OK){
        NvAPI_ShortString msg = {0};
        NvAPI_GetErrorMessage(status, msg);
		std::cout << "ofxNvApi::initialize() failed: " << (int)status << " - " << msg << std::endl;
        return false;
    }
	return true;
}

#else

bool ofxNvApi::initialize(){
	std::cout << "ofxNvApi::initialize() failed: Not running on Windows" << std::endl;
	return false;
}

#endif

bool ofxNvApi::isNvidiaAvailable(){
    return initialize();
}

bool ofxNvApi::setThreadedOptimizationOff(Scope scope) {
	NvAPI_Status status;

	status = NvAPI_Initialize();

	if (status != NVAPI_OK) return false;

	NvDRSSessionHandle session = 0;

	status = NvAPI_DRS_CreateSession(&session);

	if (status != NVAPI_OK) return false;

	status = NvAPI_DRS_LoadSettings(session);

	if (status != NVAPI_OK) {
		NvAPI_DRS_DestroySession(session);
		return false;
	}

	// modify the global (base) profile
	NvDRSProfileHandle profile = 0;

	if (scope == Scope::Global) {
		status = NvAPI_DRS_GetBaseProfile(session, &profile);
	} else {
		wchar_t path[MAX_PATH] = {};
		GetModuleFileNameW(NULL, path, MAX_PATH);
		const wchar_t* exe = wcsrchr(path, L'\\');
		exe = exe ? exe + 1 : path;

		NvAPI_UnicodeString appName = {};
		for (size_t i = 0; i < NVAPI_UNICODE_STRING_MAX - 1 && exe[i]; i++) appName[i] = (NvU16)exe[i];

		NVDRS_APPLICATION app = {};
		app.version = NVDRS_APPLICATION_VER;
		status = NvAPI_DRS_FindApplicationByName(session, appName, &profile, &app);

		if (status != NVAPI_OK) {
			NVDRS_PROFILE profileInfo = {};
			profileInfo.version = NVDRS_PROFILE_VER;
			memcpy(profileInfo.profileName, appName, sizeof(appName));
			status = NvAPI_DRS_CreateProfile(session, &profileInfo, &profile);

			if (status == NVAPI_OK) {
				NVDRS_APPLICATION newApp = {};
				newApp.version = NVDRS_APPLICATION_VER;
				memcpy(newApp.appName, appName, sizeof(appName));
				status = NvAPI_DRS_CreateApplication(session, profile, &newApp);
			}
		}
	}

	if (status != NVAPI_OK) {
		NvAPI_DRS_DestroySession(session);
		return false;
	}

	NVDRS_SETTING setting = {};

	setting.version = NVDRS_SETTING_VER;

	setting.settingId = OGL_THREAD_CONTROL_ID;

	setting.settingType = NVDRS_DWORD_TYPE;

	setting.u32CurrentValue = OGL_THREAD_CONTROL_DISABLE;

	status = NvAPI_DRS_SetSetting(
		session,
		profile,
		&setting);

	if (status != NVAPI_OK) {
		NvAPI_DRS_DestroySession(session);
		return false;
	}

	status = NvAPI_DRS_SaveSettings(session);

	NvAPI_DRS_DestroySession(session);

	return status == NVAPI_OK;
}
