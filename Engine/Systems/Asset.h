#pragma once

#define NW_DECL_RES_LIST(key,AssetClass) static std::unordered_map<key, AssetClass> resList;
#define NW_IMPL_RES_LIST(key,AssetClass) std::unordered_map<key, AssetClass> AssetClass::resList;

/**
 * @brief The base class for all assets.
 */
class Asset {
public:
    int _usageCounter = 0;

    /**
     * @brief Get the asset from the cache based on the identifier.
     * @param identifier The identifier of the asset.
     * @return The asset from the cache.
     */
    virtual Asset* GetFromCache(void* identifier);

    /**
     * @brief Load the asset from file or get it from the cache based on the identifier and file path.
     * @param identifier The identifier of the asset.
     * @param path The file path of the asset.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    virtual Asset* LoadFromFileOrGetFromCache(void* identifier, const char* path, void* data);

    /**
     * @brief Load the asset from file based on the file path.
     * @param path The file path of the asset.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    virtual Asset* LoadFromFile(const char* path, void* data = nullptr);

    /**
     * @brief Load the asset from a buffer.
     * @param buffer The buffer containing the asset data.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    virtual Asset* LoadFromBuffer(void* buffer, void* data);

    /**
     * @brief Load the asset from a buffer or get it from the cache based on the identifier.
     * @param identifier The identifier of the asset.
     * @param buffer The buffer containing the asset data.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    virtual Asset* LoadFromBufferOrGetFromCache(void* identifier, void* buffer, void* data);

    /**
     * @brief Clean up the asset.
     */
    virtual void Clean();
};

template<typename T>
class Loader {
private:
    T _asset;
public:
    /**
     * @brief Get the asset from the cache based on the identifier.
     * @param identifier The identifier of the asset.
     * @return The asset from the cache.
     */
    T* GetFromCache(void* identifier) {
        return (T*)_asset.GetFromCache();
    }

    /**
     * @brief Load the asset from file or get it from the cache based on the identifier and file path.
     * @param identifier The identifier of the asset.
     * @param path The file path of the asset.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    T* LoadFromFileOrGetFromCache(void* identifier, const char* path, void* data) {
        return (T*)_asset.LoadFromFileOrGetFromCache(identifier, path, data);
    }

    /**
     * @brief Load the asset from file based on the file path.
     * @param path The file path of the asset.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    T* LoadFromFile(const char* path, void* data = nullptr) {
        return (T*)_asset.LoadFromFile(path, data);
    }

    /**
     * @brief Load the asset from a buffer.
     * @param buffer The buffer containing the asset data.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    T* LoadFromBuffer(void* buffer, void* data) {
        return (T*)_asset.LoadFromBuffer(buffer, data);
    }

    /**
     * @brief Load the asset from a buffer or get it from the cache based on the identifier.
     * @param identifier The identifier of the asset.
     * @param buffer The buffer containing the asset data.
     * @param data Additional data for loading the asset.
     * @return The loaded asset.
     */
    T* LoadFromBufferOrGetFromCache(void* identifier, void* buffer, void* data) {
        return (T*)_asset.LoadFromBufferOrGetFromCache(identifier, buffer, data);
    }
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
    return {"", 0};
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
void hashCombine(size_t& s, const T& v) {
    std::hash<T> h;
    s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
};
