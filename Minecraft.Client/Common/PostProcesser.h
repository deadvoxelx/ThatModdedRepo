#pragma once

#include <d3d11.h>

class PostProcesser
{
public:
    static PostProcesser& GetInstance()
    {
        static PostProcesser instance;
        return instance;
    }

    void Init();
    void Apply() const;
    void SetViewport(const D3D11_VIEWPORT& viewport);
    void ResetViewport();
    void CopyBackbuffer(); // Copy backbuffer once before multi-pass gamma
    void ApplyFromCopied() const; // Apply gamma using already-copied offscreen texture
    void Cleanup();
    void SetGamma(float gamma);
    float GetGamma() const { return m_gamma; }
    void SetColorPulse(float r, float g, float b, float strength);
    void ToggleColorPulse() { m_colorPulseEnabled = !m_colorPulseEnabled; }
    bool IsColorPulseEnabled() const { return m_colorPulseEnabled; }
    bool HasColorPulse() const { return m_tintStrength > 0.0f; }
    PostProcesser(const PostProcesser&) = delete;
    PostProcesser& operator=(const PostProcesser&) = delete;

private:
    PostProcesser();
    ~PostProcesser();

    static bool IsRunningUnderWine();

    ID3D11Texture2D* m_pGammaOffscreenTex = nullptr;
    ID3D11ShaderResourceView* m_pGammaOffscreenSRV = nullptr;
    ID3D11RenderTargetView* m_pGammaOffscreenRTV = nullptr;
    ID3D11VertexShader* m_pGammaVS = nullptr;
    ID3D11PixelShader* m_pGammaPS = nullptr;
    ID3D11Buffer* m_pGammaCB = nullptr;
    ID3D11SamplerState* m_pGammaSampler = nullptr;
    ID3D11RasterizerState* m_pGammaRastState = nullptr;
    ID3D11DepthStencilState* m_pGammaDepthState = nullptr;
    ID3D11BlendState* m_pGammaBlendState = nullptr;

    bool m_initialized = false;
    float m_gamma = 1.0f;
    bool m_colorPulseEnabled = false;
    float m_tintR = 1.0f;
    float m_tintG = 1.0f;
    float m_tintB = 1.0f;
    float m_tintStrength = 0.0f;
    bool m_wineMode = false;
    D3D11_VIEWPORT m_customViewport;
    bool m_useCustomViewport = false;
    UINT m_gammaTexWidth = 0;
    UINT m_gammaTexHeight = 0;

    struct GammaCBData
    {
        float gamma;
        float uvOffsetX, uvOffsetY;
        float uvScaleX, uvScaleY;
        float tintR, tintG, tintB;
        float strength;
        float pad[3];
    };

    static const char* g_gammaVSCode;
    static const char* g_gammaPSCode;
};