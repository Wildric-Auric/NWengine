#include "Context.h"
#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"
#include "Shader.h"

namespace ComputeShaderTst {

ComputeShader* compShader;
Sprite*		   spr;

const char* TestShaderPath = "../Sandbox/src/templates/Test.comp.shader";

static void Init() {
	Context::SetTitle("Sandbox");
	Context::EnableVSync();
	Scene& s = Scene::CreateNew("New Scene");
	s.MakeCurrent();
	GameObject& cam	   = s.AddObject();
	GameObject& texObj = s.AddObject();
	Camera*		camC   = cam.AddComponent<Camera>();
	texObj.AddComponent<Transform>();
	spr = texObj.AddComponent<Sprite>();
	spr->SetSize({200, 200});

	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(720, 480);
	s.Start();

	ComputeShaderIdentifier id = TestShaderPath;
	Loader<ComputeShader>	shaderl;
	ComputeShader::parser.AddCnst("LocalSizeX", "16");
	ComputeShader::parser.AddCnst("LocalSizeY", "16");
	ComputeShader::parser.AddCnst("LocalSizeZ", "1");
	compShader = shaderl.LoadFromFileOrGetFromCache((void*)&id, id.c_str(), 0);
	printf("NW_VERSION: %s\n", NWengineGetVersionString());
	ComputeShader::parser.OutputData();
	compShader->Use();
	compShader->SetUniform1f("t", 1.0f);
	spr->texture->BindImageTex(0);
}

static void Render() {
	(*Renderer::defaultRenderer)(true);
	compShader->Use();
	compShader->Dispatch(iVec3(1, 1, 1));
	Context::NWMemoryBarrier(NWMemoryBarrierBit::SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	NWengineShutdown();
}
}; // namespace ComputeShaderTst
