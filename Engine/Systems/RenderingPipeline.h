#pragma once 
#include "Renderer.h"
#include <list>

class RenderingPipeline {
    public:
    std::list<Renderer> _renderers;
    Renderer& AddRenderer(int index = -1);
    void DeleteRenderer(int index);
    void SetUp();
    void Clean();
    void Capture();
    void Capture(int end);
    void DrawLast();
    void CaptureAndDrawLast();
};

namespace NWPPFX {
    class EffectIO {
        public:
            Camera*  _cam = nullptr;
            Renderer* _rnd= nullptr;
            Renderer* _outp;
            fVec2 _inpSize;
            bool  _autoInput = 0;

            void SetInput(Camera*);
            void SetInput(Renderer*);
            void SetOutput(Renderer*);
            Renderer* GetOutput();
            Texture* GetTex();
            int Cmp(const EffectIO*);
            bool operator == (const EffectIO&);
    };

    class Effect {
        public:
        EffectIO _fxio;
        RenderingPipeline _pline;

        virtual void SetUp() {};
        virtual void Clean();
        virtual void Capture();
        virtual void DrawLast();
    };

    class Bloom : public Effect {
        public:
        std::vector<Renderer*> _cascade;
        float luminanceThreshold = 0.8;
        //SetUp binds everything; any change in the input,
        //Require manual update for now
        void SetUp(const EffectIO* input = 0); 
        void Capture()  override;
        void DrawLast() override;
        void _Refresh();
        void _SetUpCombine();
        Shader* _GetThresholdSh();
    };

    enum class TonemapperType {
        None = 0,
        Reinhard = 1,
        Uncharted = 2,
        ACESNrkz = 3
    };

    struct TonemapperSpec {
        TonemapperType type = TonemapperType::Uncharted;
        float whitePoint = 2.0;
        float exposure = 2.0;
    };

    class Tonemapper : public Effect {
        public:
        void SetUp(const EffectIO* input = 0);
        void Capture() override;
        void DrawLast() override;
        void _Refresh(); 
        TonemapperSpec spec;
    };

};
