
#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "Texture.h"
#include "image.h"




#include "Primitives.h"

enum BatchType {
	UNBATCHED,
	STATIC_BATCH  = 1,
	DYNAMIC_BATCH = 2
};


class Sprite : public GameComponent {
private:
public:
	static std::string GetType() { return "Sprite"; };
	TextureIdentifier _texId{};
	Texture* texture  = nullptr;

	BatchType _isBatched        = BatchType::UNBATCHED;
	uint32    _lastSortingLayer = 0;
	bool      _shouldDraw		= 1;
	bool      _isRendered	    = 1;

	fVec3 vertexAttributes; //TODO::Find better place for these variabels
	Shader* shader         = nullptr;
	Quad container; //Readonly
	GameObject* attachedObj = nullptr;
	uint32 sortingLayer		= 0; //ReadOnly
	double zbuffer			= 1.0; //ReadOnly

	Sprite()                = default;
	Sprite(GameObject* go);
	~Sprite();

	void SetTexture(std::string path, bool alpha = 1);
	void SetTexture(const Image*, TextureIdentifierPtr);
	void SetTexture(Texture* tex);
	void SetShader(std::string path);
	void SetShader(const ShaderText& st, ShaderIdentifier* id);
	void SetShader(Shader* shader);
	void SetSortingLayer(uint32 order);

	void Render();
	void StopRendering();

	void Batch(BatchType type = BatchType::DYNAMIC_BATCH);
	void UnBatch();

	void Update() override;
	static std::map<GameObject*, Sprite> componentList;

	int Serialize(std::fstream* data, int offset) override;
	int Deserialize(std::fstream* data, int offset) override;

	void  SetGameObject(void* go)  override;
	void* GetGameObject()		   override;

	static unsigned int DefaultSpriteDrawCallback(void*);
};