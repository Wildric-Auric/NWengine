#pragma once
#include<map>
#include<vector>
#include"GameObject.h"
#include"FrameBuffer.h"


#define POST_PROCESSING_CELLS 0
#define POST_PROCESSING_BLOOM 1

class PostProcessing : public GameComponent {
public:
	static std::string GetType() { return "PostProcessing"; };
	PostProcessing() {};
	PostProcessing(GameObject* attachedObj);
	GameObject* attachedObj = nullptr;

	FrameBuffer fbo;

	void SetUp(iVec2 size);
	void AddFlag(uint16);
	GameObject renderQuad = GameObject();
	Shader shader = Shader(SHADER_POST_PROCESSING);
	static std::map<GameObject*, PostProcessing> componentList;
	std::vector<uint16> flags;
};
