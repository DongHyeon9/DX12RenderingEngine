#include "DepthStencilObject.h"
#include "Engine\Engine.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

bool DepthStencilObject::Init()
{
    LOG("뎁스 스텐실 오브젝트 초기화 시작");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	HRESULT hr{};

    dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	LOG("뎁스 스텐실 뷰 생성 시작");

	D3D12_DESCRIPTOR_HEAP_DESC depthStencilDesc{};
	depthStencilDesc.NumDescriptors = 1;						//뎁스 버퍼는 하나만 사용
	depthStencilDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		//DSV 타입
	depthStencilDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	//CPU에서만 접근하므로 플래그 없음
	depthStencilDesc.NodeMask = 0;								//싱글 GPU 환경이므로 0

	hr = device->CreateDescriptorHeap(&depthStencilDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(hr))
	{
		hr = device->GetDeviceRemovedReason();
	}
	CHECK(SUCCEEDED(hr), "뎁스 스텐실 뷰 생성 실패", false);

	LOG("뎁스 스텐실 뷰 생성 성공");

	CreateDepthStencilState();

    LOG("뎁스 스텐실 오브젝트 초기화 성공");
    return true;
}

void DepthStencilObject::ResetDepthStencilBuffer()
{
	depthStencilBuffer.Reset();
}

bool DepthStencilObject::CreateDepthStencilBuffer()
{
	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	HRESULT hr{};

	D3D12_RESOURCE_DESC depthStencilDesc{};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//리소스의 차원 (1D, 2D, 3D)
	depthStencilDesc.Alignment = 0;			//메모리 정렬 방식 (0은 기본 정렬)
	depthStencilDesc.Width = static_cast<UINT64>(engineSetting.resolution.x);
	depthStencilDesc.Height = static_cast<UINT64>(engineSetting.resolution.y);
	depthStencilDesc.DepthOrArraySize = 1;	//깊이 값(3D 텍스처의 경우) 또는 배열 크기
	depthStencilDesc.MipLevels = 1;			//사용될 Mipmap 레벨 수 (1은 Mipmap 없음)
	depthStencilDesc.Format = LITERAL::DEPTH_STENCIL_FORMAT;	//픽셀 포맷
	depthStencilDesc.SampleDesc.Count = engineSetting.state4XMSAA ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = engineSetting.state4XMSAA ? (engineSetting.quality4XMSAA - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;		//텍스처 레이아웃 (D3D12_TEXTURE_LAYOUT_UNKNOWN은 기본)
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//리소스 사용 목적 (D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL은 뎁스 스텐실 버퍼로 사용)

	//버퍼의 초기값을 정하는 구조체, 뎁스 스텐실 값도 지정가능
	D3D12_CLEAR_VALUE clearValue{ CD3DX12_CLEAR_VALUE{LITERAL::DEPTH_STENCIL_FORMAT, 1.0f, 0} };

	CD3DX12_HEAP_PROPERTIES heapProperty{ D3D12_HEAP_TYPE_DEFAULT };	//GPU에 의해 관리되는 기본 힙 사용
	//뎁스 스텐실 버퍼 생성
	hr = device->CreateCommittedResource(
		&heapProperty,					//힙 속성
		D3D12_HEAP_FLAG_NONE,			//힙 플래그(특별한 힙 옵션 없음)
		&depthStencilDesc,				//리소스 설명(뎁스 스텐실 버퍼 설명)
		D3D12_RESOURCE_STATE_COMMON,	//초기 리소스 상태
		&clearValue,					//초기화 시 사용될 깊이값(1.0)과 스텐실 값(0)
		IID_PPV_ARGS(&depthStencilBuffer));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;	//추가적인 플래그 설정 (ex. 읽기 전용)
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	//텍스처의 차원
	dsvDesc.Format = LITERAL::DEPTH_STENCIL_FORMAT;	//뎁스 스텐실 포맷
	dsvDesc.Texture2D.MipSlice = 0;

	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	//뎁스 스텐실 뷰 생성
	device->CreateDepthStencilView(
		depthStencilBuffer.Get(),
		&dsvDesc,
		dsvHandle);	//DSV가 저장될 디스크립터 힙의 위치

	return true;
}

void DepthStencilObject::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC desc{};
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.StencilEnable = false;
	desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	SetDepthStencilState(E_DEPTH_STENCIL_STATE_FLAG::DRAW, desc);

	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	desc.StencilEnable = true;
	desc.StencilReadMask = 0xFF;
	desc.StencilWriteMask = 0xFF;
	desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;
	desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	SetDepthStencilState(E_DEPTH_STENCIL_STATE_FLAG::MASK, desc);

	desc.DepthEnable = true;
	desc.StencilEnable = true;
	desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	desc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
	SetDepthStencilState(E_DEPTH_STENCIL_STATE_FLAG::DRAW_MASKED, desc);
}

void DepthStencilObject::SetDepthStencilState(E_DEPTH_STENCIL_STATE_FLAG Flag, D3D12_DEPTH_STENCIL_DESC Desc)
{
	depthStencilStates[static_cast<uint8>(Flag)] = std::move(Desc);
}
