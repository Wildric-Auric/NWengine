#include "Context.h"
#include "DefaultAssets.h"
#include "NWengine.h"
#include "Scene.h"
#include "Shader.h"
#include "StorageBuffer.h"

namespace ComputeShaderTst {

ComputeShader* compShader;
Sprite*		   spr;
// We will create ssbo, the compute shader write to it, and we read and output
// to the console the result in the cpu
StorageBuffer ssbo;

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
	ssbo.SetUp(256 * sizeof(float));
	ssbo.Bind(1);
}

static void Render() {
	(*Renderer::defaultRenderer)(true);
	compShader->Use();
	compShader->Dispatch(iVec3(1, 1, 1));
	Context::NWMemoryBarrier(NWMemoryBarrierBit::SHADER_IMAGE_ACCESS_BARRIER_BIT);
	static float data[256] = {-1.0};
	if(data[0] == -1.0) {
		ssbo.Read(data, 256 * sizeof(float));
		for(int i = 0; i < 256; ++i) {
			data[0] = 0;
			if(i % 16 == 0)
				printf("\n");
			printf("%d  ", (int)data[i]);
		}
	}
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
