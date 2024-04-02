#pragma once

#define NW_DECL_RES_LIST(key,AssetClass) static std::unordered_map<key, AssetClass> resList;
#define NW_IMPL_RES_LIST(key,AssetClass) std::unordered_map<key, AssetClass> AssetClass::resList;

class Asset {
public:
	virtual Asset*  GetFromCache(void* identifier);
	virtual Asset*  LoadFromFileOrGetFromCache(void* identifier, const char* path, void* data);
	virtual Asset*  LoadFromFile(const char* path, void* data = nullptr);
	virtual Asset*  LoadFromBuffer(void* buffer, void* data);
	virtual void    Clean();
};

template<typename T, typename T_Identifier> 
T_Identifier GetIDWithAsset(T assetChildInstancePtr) {
	auto list = assetChildInstancePtr->resList;
	auto iter = list.begin();
	for (iter; iter != list.end(); ++iter) {
		if (&iter->second == assetChildInstancePtr) {
			return iter->first;
		}
	}
	return {"",0};
};


template <typename T>
inline void hashCombine(size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
};