#pragma once

#include <stdexcept>
#include <string>

namespace XYZEngine
{
	class EngineException : public std::runtime_error
	{
	public:
		explicit EngineException(const std::string& message)
			: std::runtime_error(message)
		{
		}
	};

	class ResourceLoadException : public EngineException
	{
	public:
		explicit ResourceLoadException(const std::string& resourceName, const std::string& path)
			: EngineException("Failed to load resource '" + resourceName + "' from '" + path + "'")
		{
		}
	};
}
