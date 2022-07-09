#include "PostProcessing.h"
#include "Sprite.h"
#include "Camera.h"
#include "SceneEditorGui.h"

std::map<GameObject*, PostProcessing> PostProcessing::componentList;

static std::map<uint16, std::string> uniformMap = {
	{POST_PROCESSING_CELLS, "uCells"},
	{POST_PROCESSING_BLOOM, "uBloom"}
};

PostProcessing::PostProcessing(GameObject* attachedObj) {
	this->attachedObj = attachedObj;
};

void PostProcessing::SetUp(iVec2 size) {
	Sprite* sprite = renderQuad.AddComponent<Sprite>();
	sprite->container = Quad(iVec2(0, 0), size.x, size.y);
	sprite->shader = &shader;
	sprite->texture = SceneEditorGui::cam.fbo.RenderedImage; //TODO::Make two buffers in tex array for a camera //attachedObj->GetComponent<Camera>()->fbo.RenderedImage;
}

void PostProcessing::AddFlag(uint16 flag) {
	this->flags.push_back(flag);
	Sprite* sprite = renderQuad.AddComponent<Sprite>();
	sprite->shader->Use();
	sprite->shader->SetUniform1i(uniformMap[flag].c_str(), 1); //TODO::Error handling
}