#include "DefaultAssets.h"
#include "Image.h"
#include "NWengine.h"
#include "Scene.h"
#include "Wave.h"

namespace RandomTst {

RandomMatrix<500, 256, 1> mat;

static Texture randTex;
static void	   Init() {
	   Context::SetTitle("Sandbox");
	   Context::EnableVSync();
	   Scene& s = Scene::CreateNew("New Scene");
	   s.MakeCurrent();
	   GameObject& cam	= s.AddObject();
	   Camera*	   camC = cam.AddComponent<Camera>();
	   camC->Use();
	   camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	   camC->ChangeOrtho(720, 480);
	   Image im;
	   im.width	   = 1000;
	   im.height   = 1000;
	   im.channels = 3;
	   im.alpha	   = 0;
	   im.Alloc();
	   ValueNoise valueNoise = ValueNoise(10.0f, 255.0f, 0.0);

	   float		f	 = 200.0;
	   float		ampl = 150.0;
	   WaveComposer wc	 = WaveComposer();
	   for(int i = 0; i < 10; ++i) {
		   ValueNoise2* vn = new ValueNoise2(f, ampl, 0.0);
		   ampl /= 2.0;
		   f /= 2.0;
		   wc.Add(vn);
	   }

	   //    ValueNoise r;
	   //    for (int i = 0; i < 100; ++i) {
	   //        printf("%lf\n", Normalize<float>(r._rand.Get(i), NW_UI32_MAX, 1.0f));
	   //    }

	   for(int i = 0; i < im.height; ++i) {
		   for(int j = 0; j < im.width; ++j) {
			   // im.Write(fVec2(i,j), ImageColor(valueNoise.Evaluate(j), 0.0,0.0,1.0));
			   im.Write(fVec2(i, j), ImageColor(Min<float>(wc.Evaluate(i, j), 255.0), 0.0, 0.0, 1.0));
		   }
	   }

	   randTex._size = {im.width, im.height};
	   randTex._GPUGen(im.pixelBuffer, TexChannelInfo::NW_RGB);
	   GameObject& obj = s.AddObject();
	   obj.AddComponent<Transform>();
	   Sprite* sp = obj.AddComponent<Sprite>();
	   sp->SetTexture(&randTex);
	   sp->SetShader(NW_DEFAULT_SHADER_COLORED);
	   sp->GetShader()->Use();
	   sp->GetShader()->SetUniform3f("uCol", 1.1, 1.0, 1.0);

	   for(Wave* w : wc._data) {
		   delete(ValueNoise*)w;
	   }
	   // im.SaveToFile("..\\noise.png");
	   im.Clean();
	   s.Start();
	   printf("NW_VERSION: %s\n", NWengineGetVersionString());
}

static void Render() { (*Renderer::defaultRenderer)(true); }

void Run() {
	Context::_glInfo.maxVersion = 4;
	Context::_glInfo.minVersion = 6;
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::InitEnd, Init);
	NWenginePushFunction(ON_MAIN_CALL_LOCATION::FrameIntermediate, Render);
	NWengineInit();
	NWengineLoop();
	randTex.Destroy();
	NWengineShutdown();
}
}; // namespace RandomTst
