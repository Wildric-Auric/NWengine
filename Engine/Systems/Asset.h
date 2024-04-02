#pragma once

#define NW_DECL_RES_LIST(key,AssetClass) static std::unordered_map<key, AssetClass> resList;
#define NW_IMPL_RES_LIST(key,AssetClass) std::unordered_map<key, AssetClass> AssetClass::resList;

class Asset {
public:
	int _usageCounter = 0;

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

template<typename ResListType, typename T>
void EraseRes(const T& identifier) {
	ResListType::resList.erase(identifier);
}

template<typename T, typename IDType> 
T* InsertRes(IDType id) {
	T* temp = (T*)T::GetResFromCache(id);
	if (temp != nullptr) {
		temp->Clean();
	}
	T::resList.emplace(id, T());
}

template<typename T>
T LoadAssetFromFile(const char* path, void* data = nullptr) {
	T loader l;
	return = (T*)l.LoadFromFile(path, data);
}

template <typename T>
void hashCombine(size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
};