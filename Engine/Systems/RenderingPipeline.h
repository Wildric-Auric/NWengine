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
    void DrawLast();
    void CaptureAndDrawLast();
};

namespace NWPPFX {
    class EffectIO {
        public:
            Camera*  _cam = nullptr;
            Renderer* _rnd= nullptr;
            Renderer* _outp;

            void SetInput(Camera*);
            void SetInput(Renderer*);
            void SetOutput(Renderer*);
            Renderer* GetOutput();
            Texture* GetTex();
    };

    class Effect {
        virtual void Capture() {};
        virtual void DrawLast() {};
    };

    class Bloom : public Effect {
        public:
        EffectIO _fxio;
        std::vector<Renderer*> _cascade;
        RenderingPipeline _pline;
        fVec2 _lastSize;

        float luminanceThreshold;

        //SetUp binds everything; any change in the input,
        //Require manual update for now
        void SetUp(const EffectIO* input = 0); 
        void Clean();
        void Capture() override;
        void DrawLast() override;
        void _SetUpCombine();
    };
};
