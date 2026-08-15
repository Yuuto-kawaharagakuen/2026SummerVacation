#include "k2EngineLowPreCompile.h"
#include "FontEngine.h"

using namespace std;
using namespace DirectX;

namespace nsK2EngineLow {
	FontEngine::~FontEngine()
	{
		if (m_srvDescriptorHeap != nullptr) {
			m_srvDescriptorHeap->Release();
		}
	}
    void FontEngine::Init()
    {
        auto d3dDevice = g_graphicsEngine->GetD3DDevice();

        // ディスクリプタヒープをフォント種類数ぶん作成
        D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
        srvHeapDesc.NumDescriptors = (int)enFontType::enNum;  // ← 1から変更
        srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
        auto hr = d3dDevice->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvDescriptorHeap));

        ResourceUploadBatch re(d3dDevice);
        re.Begin();

        RenderTargetState renderTargetState;
        renderTargetState.rtvFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
        renderTargetState.numRenderTargets = 1;
        renderTargetState.dsvFormat = DXGI_FORMAT_D32_FLOAT;
        renderTargetState.sampleMask = UINT_MAX;
        renderTargetState.sampleDesc.Count = 1;

        SpriteBatchPipelineStateDescription sprBatchDesc(renderTargetState);

        D3D12_VIEWPORT viewport;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<FLOAT>(1920);
        viewport.Height = static_cast<FLOAT>(1080);

        m_spriteBatch = make_unique<SpriteBatch>(d3dDevice, re, sprBatchDesc, &viewport);

        // ディスクリプタのサイズを取得(スロットを進めるのに必要)
        UINT descriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        // 読み込むフォントファイルのパス一覧
        const wchar_t* fontPaths[(int)enFontType::enNum] = {
            L"Assets/font/myfile.spritefont",         // Gothic
            L"Assets/font/myfile_bold.spritefont",     // Bold
            L"Assets/font/myfile_rounded.spritefont",  // Rounded
        };

        D3D12_CPU_DESCRIPTOR_HANDLE cpuHandleStart = m_srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_GPU_DESCRIPTOR_HANDLE gpuHandleStart = m_srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

        for (int i = 0; i < (int)enFontType::enNum; i++)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = cpuHandleStart;
            D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = gpuHandleStart;
            cpuHandle.ptr += descriptorSize * i;
            gpuHandle.ptr += descriptorSize * i;

            m_spriteFonts[i] = make_unique<SpriteFont>(
                d3dDevice,
                re,
                fontPaths[i],
                cpuHandle,
                gpuHandle);
        }

        re.End(g_graphicsEngine->GetCommandQueue());
    }

	void FontEngine::BeginDraw(RenderContext& rc)
	{
		auto commandList = g_graphicsEngine->GetCommandList();
		auto d3dDevice = g_graphicsEngine->GetD3DDevice();
		m_spriteBatch->Begin(
			commandList,
			SpriteSortMode_Deferred,
			g_matIdentity
		);
		commandList->SetDescriptorHeaps(1, &m_srvDescriptorHeap);
	}
	void FontEngine::EndDraw(RenderContext& rc)
	{
		m_spriteBatch->End();
	}

    void FontEngine::Draw(
        const wchar_t* text,
        const Vector2& position,
        const Vector4& color,
        float rotation,
        float scale,
        Vector2 pivot,
        enFontType fontType
    )
    {
        m_spriteFonts[(int)fontType]->DrawString(
            m_spriteBatch.get(),
            text,
            position.vec,
            color,
            rotation,
            DirectX::XMFLOAT2(pivot.x, pivot.y),
            scale
        );
    }
}