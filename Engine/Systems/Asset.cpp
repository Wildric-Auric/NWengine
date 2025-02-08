#include "Asset.h"
#include "AssetList.h"

Asset* Asset::GetFromCache(void* identifier)			   { return nullptr;}
Asset* Asset::LoadFromFileOrGetFromCache(void* identifier, 
								 const char* path = nullptr, 
								 void* data = nullptr)     
{
	Asset* ret = GetFromCache(identifier);
	if (ret != nullptr)
		return ret;
	return LoadFromFile(path, identifier);

}

Asset* Asset::LoadFromBufferOrGetFromCache(void* identifier, void* buffer, void* data) {
	Asset* ret = GetFromCache(identifier);
	if (ret != nullptr)
		return ret;
	return LoadFromBuffer(buffer, data);
}

Asset*   Asset::LoadFromFile(const char* path, void* output) { return nullptr;}
Asset*   Asset::LoadFromBuffer(void* buffer, void* data)	 { return nullptr;}
void	 Asset::Clean()									     {}

void Asset::Destroy() {
#define CLN_RES(type)\
    while (type::resList.size() != 0)\
        type::resList.begin()->second.Clean();
    CLN_RES(Texture);
    CLN_RES(Sound);
    CLN_RES(DllScript);
    CLN_RES(Font);
}


