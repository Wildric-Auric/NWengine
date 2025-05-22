#include "SceneSet.h"
#include "Scene.h"
#include "PrimManager.h"
#include "Utilities.h"
#include "DefaultAssets.h"
#include "CircleRenderer.h"
#include "UIWindow.h"

struct GuiItems {
	UIWindow*	 win;
	UIItemLabel* gridSizeLabel;
	UIItem*		 gridSize;
	UIItemLabel* lwidthLabel;
	UIItem*		 lineWidth;
	UIItemLabel* pointRadLabel;
	UIItem*		 pointRad;
	UIItem*		 update;
	UIItem*		 disableGrid;
	UIItem*		 blackBg;
	UIItem*		 whiteBg;
};

GuiItems guiItems = {};

UIColorScheme customColScheme = {
	{0, 0, 0, 1},		  // bg;
	{1, 1, 1, 1},		  // fg;
	{0.5, 0.5, 0.5, 0.5}, // win;
	{0.5, 0.5, 0.5, 0.5}, // winRest;
	{0.5, 0.5, 0.5, 0.5}, // winHover;
	{0.5, 0.5, 0.5, 0.5}, // winSelect;
	{1, 0, 0, 1},		  // winBar;
	{0, 0, 0, 1},		  // winBrdr;
	{0, 0, 1, 1},		  // winBrdrResize;
	{1, 1, 1, 1},		  // text;
	{1, 1, 1, 1},		  // titleText;
};

void SetWin() {
	currentUIColorScheme = customColScheme;
	GameObject* obj		 = &Scene::currentScene->AddObject("uiwin");
	Camera*		cam		 = Camera::ActiveCamera;
	Transform*	tr		 = obj->Get<Transform>();
	UIWindow*	uwin	 = &obj->Add<UIWindow>();
	uwin->cursor.strat	 = CurAdvanceStrat::BreakOnHorizontalEnd;
	v2f s;
	s = cam->GetSize();
	s.x *= 0.15;
	uwin->SetTitle("Docked Win");
	uwin->SetSize(s);
	uwin->SetPosition({-cam->GetSize().x * 0.5f + s.x * 0.5f, 0.0});
	uwin->prop &= ~Window_Prop_ResizableXL;
	uwin->prop &= ~Window_Prop_ResizableYU;
	uwin->prop &= ~Window_Prop_Movable;
	guiItems.gridSizeLabel = uwin->AddItem(UIItemType_Label, -1, 1);
	UISetLabel(guiItems.gridSizeLabel, "Grid Size: ");
	guiItems.gridSize = uwin->AddItem(UIItemType_Slider, -1, 1);

	guiItems.lwidthLabel = uwin->AddItem(UIItemType_Label, -1, 1);
	UISetLabel(guiItems.lwidthLabel, "Line Width: ");
	guiItems.lineWidth = uwin->AddItem(UIItemType_Slider, -1, 1);

	guiItems.pointRadLabel = uwin->AddItem(UIItemType_Label, -1, 1);
	UISetLabel(guiItems.pointRadLabel, "Line Width: ");
	guiItems.pointRad = uwin->AddItem(UIItemType_Slider, -1, 1);

	UIItemLabel* label = uwin->AddItem(UIItemType_Label, -1, 1);
	UISetLabel(label, "Refresh: ");
	guiItems.update = uwin->AddItem(UIItemType_Checkbox, -1, 1);
	guiItems.win	= uwin;
	label			= uwin->AddItem(UIItemType_Label, -1, 0);
	UISetLabel(label, "Disable Background Lines ");
	guiItems.disableGrid = uwin->AddItem(UIItemType_Checkbox, -1, 1);
	label				 = uwin->AddItem(UIItemType_Label, -1, 0);
	UISetLabel(label, "Black Background ");
	guiItems.blackBg = uwin->AddItem(UIItemType_Checkbox, -1, 1);
	label			 = uwin->AddItem(UIItemType_Label, -1, 0);
	UISetLabel(label, "White Background ");
	guiItems.whiteBg = uwin->AddItem(UIItemType_Checkbox, -1);

	UIGetSliderData(guiItems.pointRad)->minn  = 0;
	UIGetSliderData(guiItems.pointRad)->maxx  = 8;
	UIGetSliderData(guiItems.lineWidth)->minn = 0;
	UIGetSliderData(guiItems.lineWidth)->maxx = 5;
	UIGetSliderData(guiItems.gridSize)->minn  = 3;
	UIGetSliderData(guiItems.gridSize)->maxx  = 100;

	UISetSliderValue(guiItems.gridSize, 50);
	UIGetSliderData(guiItems.pointRad)->curPercent	= 0.5;
	UIGetSliderData(guiItems.lineWidth)->curPercent = 0.5;
}

static void SetBg() {
	Scene&		s	= *Scene::GetCurrent();
	Camera&		cam = *Camera::GetActiveCamera();
	GameObject& bg	= s.AddObject("blueprint");
	bg.AddComponent<Transform>();

	Sprite* spr = bg.AddComponent<Sprite>();
	spr->SetShader("../Sandbox/src/templates/Blueprint.shader");
	spr->SetSize(cam.GetSize());
	spr->GetShader()->Use();
	spr->GetShader()->SetVector2("uResolution", spr->container.width, spr->container.height);
	spr->GetShader()->SetVector2("uCell", 50.0f, 50.0f);
	spr->SetSortingLayer(NW_I32_MAX);
}

static void SetCam() {
	Scene&		s	 = *Scene::GetCurrent();
	GameObject& cam	 = s.AddObject();
	Camera*		camC = cam.AddComponent<Camera>();
	camC->Use();
	camC->SetClearColor(fVec4(0.2, 0.0, 1.0, 1.0));
	camC->ChangeOrtho(1080, 720);
	camC->fbo.GenDepthStencilBuffer();
}

static void SetText() {
	Scene&		s	= *Scene::GetCurrent();
	GameObject& obj = s.AddObject<Sprite, Transform, Text>();
	Text*		te	= obj.GetComponent<Text>();
	std::string fdir;
	GetSystemFontDir(&fdir);
	fdir += "Arial.ttf";
	te->isBatched  = 1;
	te->layerOrder = 0;
	te->SetShader(NW_DEFAULT_SHADER_TEXT_BATCHED);
	te->colors = v4f(1.0, 0.0, 0.0, 1.0);
	te->SetFont({fdir.c_str(), 15});
	te->SetContent("'T': Ear Clipping Triangulation\n"
				   "'S': Delaunay Triangulation\n"
				   "'R': reset\n"
				   "'A': circumcircle of a triangle.");
	te->UpdateGlyphs();
	v2f camS = Camera::ActiveCamera->GetSize();
	v2f tep	 = te->GetPosition();
	v2f ts	 = te->GetSize();
	te->SetPosition(tep + camS * 0.5 - ts * 0.5);
	te->UpdateGlyphs();
	obj.GetComponent<Sprite>()->SetSize(ts);
	obj.GetComponent<Transform>()->SetPosition(te->_bb.center);

	GameObject& obj2 = s.AddObject<CircleRenderer>("disc");
	Sprite*		spr	 = obj2.GetComponent<Sprite>();
	spr->SetShader("../Sandbox/src/triangulation/disc.shader");
	spr->StopRendering();
}

void SceneSet::Update() {
	UISetLabel(guiItems.gridSizeLabel,
			   (std::string("Grid Size: ") + std::to_string((int)UIGetSliderValue(guiItems.gridSize))).c_str());

	UISetLabel(guiItems.lwidthLabel,
			   (std::string("Line Width: ") + std::to_string((int)UIGetSliderValue(guiItems.lineWidth))).c_str());

	UISetLabel(guiItems.pointRadLabel,
			   (std::string("Point Radius: ") + std::to_string((int)UIGetSliderValue(guiItems.pointRad))).c_str());
}

void SceneSet::Start() {
	Scene& s = *Scene::GetCurrent();
	SetCam();
	SetBg();
	SetWin();
	SetText();
	s.AddObject().AddComponent<Script>()->SetScript<PrimManager>();
}
