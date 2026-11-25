
meta:
	ADDON_NAME = ofxNvApi
	ADDON_DESCRIPTION = Addon for controlling NVIDIA GPU settings and querying GPU information. This addon is a wrapper for the NVIDIA NVAPI library, which provides access to NVIDIA GPU features and settings.
	ADDON_AUTHOR = Sam McElhinney


common:

	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	# ADDON_INCLUDES = libs/nvapi/include

vs:
# Visual Studio only, any additional libraries that should be linked to the project
	# ADDON_LIBS = libs/nvapi/lib/nvapi64.lib
