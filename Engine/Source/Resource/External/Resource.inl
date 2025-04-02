#include "Resource.h"

template<class _ResourceType>
inline bool Resource<_ResourceType>::Load(FString FileName)
{
	fileName = std::move(FileName);
	return true;
}