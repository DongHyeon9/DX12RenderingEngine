#pragma once
#include "Header\EngineCore.h"
#include "Resource\External\Resource.h"

class ResourceManager
{
	DECLARE_SINGLETON(ResourceManager);
public:

protected:

private:
	std::unordered_map<FString, std::shared_ptr<Resource>> resources{};
public:
	bool Init();
	template<class _ResourceType>
	std::shared_ptr<_ResourceType> GetResource(FString FileName)
	{
		auto mIter = resources.find(FileName);
		if (mIter != resources.end())
			return std::static_pointer_cast<_ResourceType>(mIter->second);
		else
		{
			std::shared_ptr<_ResourceType> resource = std::make_shared<_ResourceType>();
			resource->Load(FileName);
			resources.emplace(FileName, resource);
			return std::static_pointer_cast<_ResourceType>(resources[FileName]);
		}
	}

protected:

private:

};

