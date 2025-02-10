#include "RenderingPipeline.h"
#include "Sprite.h"
#include "Batch.h"

std::string vertexShaderSrc = 
R"V0G0N( 
        #pragma vertex
        #version 330 core
        
        layout(location = 0) in vec3 attribPos;
        layout(location = 1) in vec2 texCoord;
        
        uniform mat4 uMvp = mat4(1.0);
        out vec2 uv;
        
        void main() {
            gl_Position = uMvp * vec4(attribPos, 1.0);
            uv = texCoord;
        };
)V0G0N";

std::string gaussianBlurShaderSrc = 
R"V0G0N(        
        #pragma fragment
        #version 330 core
        
        uniform sampler2D uTex0;
        in vec2 uv;
        
        out vec4 FragColor;
        
        #define s(x,y) texelFetch(uTex0, ivec2(gl_FragCoord.xy) + ivec2(x,y),0).xyz

        void main() {
            vec3 c = vec3(0.0);
            c += s(-1,0)  *2.0/16.0;
            c += s(0,0)   *4.0/16.0;
            c += s(1,0)   *2.0/16.0;

            c += s(-1,-1) *1.0/16.0;
            c += s(0,-1)  *2.0/16.0;
            c += s(1,-1)  *1.0/16.0;

            c += s(-1,1) *1.0/16.0;
            c += s(0,1)  *2.0/16.0;
            c += s(1,1)  *1.0/16.0;
            
            FragColor = vec4(c,1.0);
        };
    
)V0G0N";

std::string thresholdShaderSrc = 
R"V0G0N(        
        #pragma fragment
        #version 330 core
        
        uniform sampler2D uTex0;
        uniform float uThresh = 0.8;
        in vec2 uv;
        
        out vec4 FragColor;
        
        float luminance(inout vec3 col) {
            return dot(col, vec3(0.2125, 0.7154, 0.0721));
        }

        void main() {
            vec3 c = texture(uTex0,uv).xyz ;
            c = (luminance(c) < uThresh) ? vec3(0.0) : c;
            FragColor = vec4(c,1.0);
        }
    
)V0G0N";

std::string combineShaderSrc = 
R"V0G0N(        
        #pragma fragment
        #version 330 core
        #pragma def MAX_TEX MaxTexNum 

        uniform sampler2D uTex[MAX_TEX];
        uniform sampler2D uTex0;
        uniform int uCascadeNum;

        in vec2 uv;
        
        out vec4 FragColor;
        
        void main() {
            float contrib = 0.5;
            vec3 c = texture(uTex0,uv).xyz ;
            for (int i = 1; i < uCascadeNum; ++i) {
                c += (1.0 -0.5 * contrib) * texture(uTex[i], uv).xyz;
                contrib -= 0.5*contrib;
            }
            vec3 cw = vec3(2.0,2.0,2.0); 
            //TODO::Remove this, tonemapping should be separate and correct
            vec3 f = c / (cw*cw) + vec3(1.0);
            FragColor = vec4(c*f/(vec3(1.0)+c),1.0);
        }
    
)V0G0N";

Renderer& RenderingPipeline::AddRenderer(int index) {
    if (index < 0)  {
        _renderers.push_back({});
        _renderers.back().SetUp();
        return _renderers.back();
    }
    auto iter = _renderers.begin();
    std::advance(iter,index);
    Renderer& r = *_renderers.insert(iter, {});
    r.SetUp();
    return r;
}

void RenderingPipeline::Clean() {
    for (auto iter = _renderers.begin(); iter != _renderers.end(); ++iter) {
        iter->Clean();
    }
}

void RenderingPipeline::DeleteRenderer(int index) {
    auto iter = _renderers.begin();
    std::advance(iter,index);
    _renderers.erase(iter);
}

void RenderingPipeline::SetUp() {

}

void RenderingPipeline::Capture() {
    Renderer* r = (*_renderers.begin())(false);
    auto iter = ++_renderers.begin();
    for (int i = 1; i < _renderers.size(); ++i) {
        r = (*iter)(r);
        ++iter;
    }
}

void RenderingPipeline::DrawLast() {
    if (_renderers.size() == 1) {
        (*--_renderers.end())(true);
        return;
    }
    Renderer* r = &*(--(--_renderers.end()));
    (*--_renderers.end())(r,true);
}

void RenderingPipeline::CaptureAndDrawLast() {
    Renderer* r = (*_renderers.begin())(_renderers.size() == 1);
    for (int i = 1; i < _renderers.size(); ++i) {
        r = (*r)(i == _renderers.size()-1);
    }
}

namespace NWPPFX {
    void EffectIO::SetInput(Camera* c) {
       _cam = c;
       _rnd = 0;
    }
    void EffectIO::SetInput(Renderer* r) {
       _rnd = r;
       _cam = 0;
    }  
    void EffectIO::SetOutput(Renderer* r) {
        _outp = r;
    }
    Renderer* EffectIO::GetOutput() {
        return _outp;
    }
    Texture* EffectIO::GetTex() {
        if (_cam) {
            return &_cam->fbo.GetAtt(0).tex;
        }
        if (_rnd) {
            FrameBuffer* fbo = &_rnd->GetCamera()->fbo;
            if (!fbo) return 0;
            return &fbo->GetAtt(0).tex;
        }
        return 0;
    }

    void Bloom::SetUp(const EffectIO* input) {
        if (input) 
            _fxio = *input;  
        else 
            _fxio.SetInput(Camera::ActiveCamera);
        
        fVec2 curSize = _fxio.GetTex()->_size; 
        int csNum = (int)Min<float>(std::log2(curSize.x) ,std::log2(curSize.y)) - 3;
        Renderer* rnd;

        ShaderIdentifier sidBlur = "NWPPFXBlur";
        ShaderText stBlur;
        ShaderIdentifier sidThreshold = "NWPPFXThreshold";
        ShaderText stThreshold;
        ShaderIdentifier sidCombine= "NWPPFXCombine";
        ShaderText stCombine;
        stBlur.vertex = vertexShaderSrc.c_str();
        stBlur.fragment = gaussianBlurShaderSrc.c_str();

        stCombine.vertex = vertexShaderSrc.c_str();
        stCombine.fragment = combineShaderSrc.c_str();

        stThreshold.vertex = vertexShaderSrc.c_str();
        stThreshold.fragment = thresholdShaderSrc.c_str();
        //First we get the threshold
        rnd = &_pline.AddRenderer();
        rnd->SetUp();
        rnd->SetOffScreenSizeMultiplier(fVec2(1.0,1.0));
        rnd->SetTexParams(TexMinFilter::NW_MIN_LINEAR, TexMaxFilter::NW_LINEAR);
        rnd->SetShader(stThreshold, &sidThreshold);

        //Downscale and Blur
        _cascade.clear();
        for (int i = 0; i < csNum; ++i) {
            rnd = &_pline.AddRenderer();
            rnd->SetUp();
            rnd->SetOffScreenSizeMultiplier(fVec2(0.5,0.5));
            rnd->SetTexParams(TexMinFilter::NW_MIN_LINEAR, TexMaxFilter::NW_LINEAR);

            rnd = &_pline.AddRenderer();
            rnd->SetUp();
            rnd->SetOffScreenSizeMultiplier(fVec2(1.0,1.0));
            rnd->SetTexParams(TexMinFilter::NW_MIN_LINEAR, TexMaxFilter::NW_LINEAR);
            rnd->SetShader(stBlur, &sidBlur);
            _cascade.push_back(rnd);
        }
        //Combine
        rnd = &_pline.AddRenderer();
        rnd->SetUp();
        rnd->SetOffScreenSizeMultiplier(fVec2(1.0,1.0));
        rnd->SetTexParams(TexMinFilter::NW_MIN_LINEAR, TexMaxFilter::NW_LINEAR);
        rnd->SetShader(stCombine, &sidCombine);
        _fxio.SetOutput(rnd);
    } 
    
    void Bloom::Clean() {
        _pline.Clean();
    }

    void Bloom::_SetUpCombine() {
        Shader* sh = _fxio.GetOutput()->componentContainer.GetComponent<Sprite>()->shader;
        sh->Use();
        sh->SetUniform1i("uCascadeNum", _cascade.size());
        std::vector<int> v(_cascade.size() + 1);

        for (int i = 0; i < _cascade.size();++i ) {
            v[i] = i+1;
            Camera* cam = _cascade[i]->GetCamera();
            cam->fbo.GetAtt(0).tex.Bind(i+1);
        }
        sh->SetUniformArrayi("uTex",v.data(), _cascade.size());
    }

    void Bloom::Capture() {
        Renderer* r = (*_pline._renderers.begin())(false);
        auto iter = ++_pline._renderers.begin();
        for (int i = 1; i < _pline._renderers.size() - 1; ++i) {
            r = (*iter)(r);
            ++iter;
        }
        _SetUpCombine();
        (*_fxio.GetOutput())(false); 
    }

    void Bloom::DrawLast() {
        _SetUpCombine();
        (*_fxio.GetOutput())(true); 
    }
};
