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
        uniform float uThresh;
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
            FragColor = vec4(c,1.0);
        }
    
)V0G0N";

std::string tonemappingShaderSrc = 
R"V0G0N(        
        #pragma fragment
        #version 330 core
        #pragma def MAX_TEX MaxTexNum 

        uniform sampler2D uTex0;

        uniform int uSpec = 0;
        uniform float uWhitePt;
        uniform float uExposure = 2.0;
 
        in vec2 uv; 
        out vec4 FragColor;
        
        float luminance(inout vec3 col) {
            return dot(col, vec3(0.2125, 0.7154, 0.0721));
        }

        void reinhard(inout vec3 c, inout vec4 oCol) {
            float inl = luminance(c);
            float tmp = inl / (uWhitePt*uWhitePt) + 1.0;
            float outl = inl * tmp / (1.0 + inl);
            oCol = vec4(c * outl / inl, 1.0);
        }

        void uncharted(inout vec3 c, inout vec4 oCol) {
            float A = 0.15;
            float B = 0.50;
            float C = 0.10;
            float D = 0.20;
            float E = 0.02;
            float F = 0.30;
            float W = 11.2;
            oCol.xyz = c * uExposure;
            oCol.xyz = ((oCol.xyz * (A * oCol.xyz + C * B) + D * E) / (oCol.xyz * (A * oCol.xyz + B) + D * F)) - E / F;
            float white = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
            oCol /= white;
            oCol.w = 1.0;
        }

        void filmicNrkzACES(inout vec3 x, inout vec4 oCol) {
            float a = 2.51f;
            float b = 0.03f;
            float c = 2.43f;
            float d = 0.59f;
            float e = 0.14f;
            oCol.xyz = clamp((x*(a*x+b))/(x*(c*x+d)+e),0.0,1.0);
            oCol.w = 1.0;
        }

        void main() {
            vec3 c = texture(uTex0,uv).xyz;
            if (uSpec == 0) {
                FragColor = vec4(c,1.0);
                return;
            }
            if (uSpec == 1) {
                reinhard(c, FragColor);
                return;
            }
            if (uSpec == 2) {
                uncharted(c, FragColor);
                return;
            }
            if (uSpec == 3) {
                filmicNrkzACES(c, FragColor);
                return;
            }
        } 
)V0G0N";

std::string colorCorrectionShaderSrc = 
R"V0G0N(        
        #pragma fragment
        #version 330 core

        in vec2 uv; 
        out vec4 FragColor;
        uniform sampler2D uTex0;

        uniform float uTemperature = 6500.0;
        uniform float uTint = 0.0;
        uniform float uContrast = 1.0;
        uniform float uBrightness = 0.0;
        uniform float uHiLights = 0.0;
        uniform float uShadows  = 0.0;
        uniform float uHiThresh = 0.6;
        uniform float uShadowThresh = 0.4;
        uniform float uVibrance = 0.0;
        uniform float uSaturation = 1.0;


        float luminance(inout vec3 col) {
            return dot(col, vec3(0.2125, 0.7154, 0.0721));
        }

        void main() {
            vec3 c = texture(uTex0,uv).xyz;
            //------White balance-------
            float tpr = (uTemperature - 6500.0) / 1000.0;
            vec3 tempFilter = vec3(1.0 + 0.1 * tpr, 1.0, 1.0 - 0.1 * tpr);
            vec3 tintFilter = vec3(1.0, 1.0 + uTint, 1.0);
            c *= tempFilter * tintFilter;
            //------Contrast-------
            c = (c - 0.5) * uContrast + 0.5;
            c += uBrightness;
            //------Shadows and Highlights------- 
            float lum = luminance(c);
            float hFactor = smoothstep(uHiThresh, 1.0 - uHiThresh, lum);
            float sFactor = 1.0 - smoothstep(0.0, uShadowThresh, lum);
            c += uHiLights * hFactor;
            c += uShadows  * sFactor;
            //------Vibrance------- 
            float m = max(c.r, max(c.g, c.b));
            float intensity = 1.0 - m;
            c = mix(c, c * (1.0 + uVibrance), intensity);
            //------Saturation--------
            c = mix(vec3(lum), c, uSaturation);

            //------Return---------
            c = vec3(max(c.x,0.0),max(c.y,0.0),max(c.z,0.0));
            FragColor = vec4(c,1.0);
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
    for (auto iter = _renderers.begin(); iter != _renderers.end();) {
        iter->Clean();
        iter = _renderers.erase(iter);
    }
}

void RenderingPipeline::DeleteRenderer(int index) {
    auto iter = _renderers.begin();
    std::advance(iter,index);
    _renderers.erase(iter);
}

void RenderingPipeline::SetUp() {

}

void RenderingPipeline::Capture(int end) {
    Renderer* r = (*_renderers.begin())(false);
    auto iter = ++_renderers.begin();
    for (int i = 1; i < end; ++i) {
        r = (*iter)(r);
        ++iter;
    }
}
void RenderingPipeline::Capture() {
    Capture(_renderers.size());
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


    inline EffectIO Effect::GetIO() {
        return _fxio;
    }

    Renderer* Effect::GetOutput() {
        return _fxio.GetOutput(); 
    }

    void Effect::Clean() {_pline.Clean();}
    void Effect::Capture() {_pline.Capture();}
    void Effect::DrawLast() {_pline.DrawLast();}
    bool EffectIO::operator== (const EffectIO& s) {
       return this->_cam == s._cam && this->_inpSize == s._inpSize &&
           this->_rnd == s._rnd;
    }

    int  EffectIO::Cmp(const EffectIO* other) {
        if (*this == *other) return 0;
        return 1;
    }

    void Bloom::SetUp(const EffectIO* input) {
        if (input) {
            _fxio = *input;  
        }
        else {
            _fxio.SetInput(Camera::GetActiveCamera());
            _fxio._inpSize = Camera::GetActiveCamera()->size;
            _fxio._autoInput = 1;
        }
        
        int csNum = (int)Min<float>(std::log2(_fxio._inpSize.x) ,std::log2(_fxio._inpSize.y)) - 3;
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
    
    Shader* Bloom::_GetThresholdSh() {
        return _pline._renderers.begin()->componentContainer.GetComponent<Sprite>()->shader;
    }

    void Bloom::_Refresh() {
        EffectIO  tmp = _fxio;
        int flag = 0;
        if (_fxio._cam != nullptr) {
            tmp.SetInput(Camera::GetActiveCamera());
            tmp._inpSize = Camera::GetActiveCamera()->size; 
            flag = _fxio.Cmp(&tmp);
        }
        else if (_fxio._rnd != nullptr) {
            tmp.SetInput(_fxio._rnd);
            tmp._inpSize = _fxio.GetTex()->_size; 
            flag = _fxio.Cmp(&tmp);
        }
        if (!flag) return;
        _fxio = tmp;
        Clean();
        SetUp();
    }

    void Bloom::_SetUpCombine() {
        std::vector<int> v(_cascade.size() + 1);
        for (int i = 0; i < _cascade.size();++i ) {
            v[i] = i+1;
            Camera* cam = _cascade[i]->GetCamera();
            cam->fbo.GetAtt(0).tex.Bind(i+1);
        }
        Shader* sh = _fxio.GetOutput()->componentContainer.GetComponent<Sprite>()->shader;
        sh->Use();
        sh->SetUniform1i("uCascadeNum", _cascade.size());
        sh->SetUniformArrayi("uTex",v.data(), _cascade.size());
        sh = _pline._renderers.begin()->componentContainer.GetComponent<Sprite>()->shader;
        sh->Use();
        sh->SetUniform1f("uThresh", luminanceThreshold);
    }

    void Bloom::Capture() {
        if (_fxio._autoInput) 
            _Refresh();
        Renderer* r;
        Camera* temp = Camera::GetActiveCamera();
        if (_fxio._cam) {
            _fxio._cam->Use();
            r = (*_pline._renderers.begin())(false);
        }
        else 
            r = (*_pline._renderers.begin())(_fxio._rnd, false);
        auto iter = ++_pline._renderers.begin();
        for (int i = 1; i < _pline._renderers.size() - 1; ++i) {
            r = (*iter)(r);
            ++iter;
        }
        _SetUpCombine();
        (*_fxio.GetOutput())(false); 
        temp->Use();
    }

    void Bloom::DrawLast() {
        Camera* temp = Camera::GetActiveCamera();
        if (_fxio._cam) {
            _fxio._cam->Use();
        }
        _SetUpCombine();
        (*_fxio.GetOutput())(true); 
        temp->Use();
    }

    void Tonemapper::SetUp(const EffectIO* input) {
        if (input) {
            _fxio = *input;  
        }
        else {
            _fxio.SetInput(Camera::GetActiveCamera());
            _fxio._inpSize = Camera::GetActiveCamera()->size;
            _fxio._autoInput = 1;
        }
        Renderer& r = _pline.AddRenderer();  
        r.SetUp();

        ShaderIdentifier sidTm= "NWPPFXTonemapping";
        ShaderText stTm;
        stTm.vertex = vertexShaderSrc.c_str();
        stTm.fragment = tonemappingShaderSrc.c_str(); 
        _fxio.SetOutput(&r);
        r.SetShader(stTm, &sidTm);
    }

    void Tonemapper::Capture() {
        Camera* temp = Camera::GetActiveCamera();
        Renderer* r;
        if (_fxio._cam) {
            _fxio._cam->Use();
        }
        if (_fxio._cam) {
            _fxio._cam->Use();
            r = (*_pline._renderers.begin())(false);
        }
        else 
            r = (*_pline._renderers.begin())(_fxio._rnd, false);
        
        Shader* sh = _pline._renderers.begin()->componentContainer.GetComponent<Sprite>()->shader;
        sh->Use();
        sh->SetUniform1f("uWhitePt", spec.whitePoint);
        sh->SetUniform1i("uSpec", (int)spec.type);
        sh->SetUniform1f("uExposure", spec.exposure);
        temp->Use();
    }

    void Tonemapper::DrawLast() {
        Camera* temp = Camera::GetActiveCamera();
        Renderer* r;
        if (_fxio._cam) {
            _fxio._cam->Use();
        }
        if (_fxio._cam) {
            _fxio._cam->Use();
            r = (*_pline._renderers.begin())(1);
        }
        else 
            r = (*_pline._renderers.begin())(_fxio._rnd, 1);
        temp->Use();
    }

    void Tonemapper::_Refresh() {

    }
    void ColorCorrection::SetUp(const EffectIO* input) {
        if (input) {
            _fxio = *input;
        }
        else {
            _fxio.SetInput(Camera::GetActiveCamera());
            _fxio._inpSize  = Camera::GetActiveCamera()->size;
            _fxio._autoInput = 1;
        }
        _fxio._outp = &this->_pline.AddRenderer();
        Renderer& r = *_fxio._outp;
        ShaderIdentifier sidColorCor= "NWPPFXColorCorrection";
        ShaderText stColorCor;
        stColorCor.vertex = vertexShaderSrc.c_str();
        stColorCor.fragment = colorCorrectionShaderSrc.c_str();
        r.SetUp();
        r.SetOffScreenSizeMultiplier(fVec2(1.0,1.0));
        r.SetTexParams(TexMinFilter::NW_MIN_LINEAR, TexMaxFilter::NW_LINEAR);
        r.SetShader(stColorCor, &sidColorCor);
    }

    //TODO::Refresh
    void ColorCorrection::Capture() {
        Renderer* r;
        Camera* temp = Camera::GetActiveCamera();
        _SetParams();
        if (_fxio._cam) {
            _fxio._cam->Use();
            r = (*_pline._renderers.begin())(false);
        }
        else 
            r = (*_pline._renderers.begin())(_fxio._rnd, false);
        temp->ActiveCamera->Use();
    }

    void ColorCorrection::_SetParams() {
        Shader* s = _pline._renderers.begin()->componentContainer.GetComponent<Sprite>()->shader; 
        s->Use();
        s->SetUniform1f("uTemperature", spec.temperature);
        s->SetUniform1f("uTint", spec.tint);
        s->SetUniform1f("uContrast", spec.contrast);
        s->SetUniform1f("uBrightness", spec.brightness);
        s->SetUniform1f("uHiLights", spec.hiLights);
        s->SetUniform1f("uShadows", spec.shadows);
        s->SetUniform1f("uHiThresh", spec.hiThresh);
        s->SetUniform1f("uShadowThresh", spec.shadowThresh);
        s->SetUniform1f("uVibrance", spec.vibrance);
        s->SetUniform1f("uSaturation", spec.saturation);;
    }

    void ColorCorrection::DrawLast() {
        Camera* temp = Camera::GetActiveCamera();
        _SetParams();
        if (_fxio._cam) {
            _fxio._cam->Use();
        }
        (*_fxio.GetOutput())(true); 
        temp->Use();

    }
};


