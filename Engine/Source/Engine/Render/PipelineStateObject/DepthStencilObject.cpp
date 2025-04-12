#include "DepthStencilObject.h"
#include "Engine\Engine.h"

#include "Manager\RenderManager.h"
#include "Engine\Render\DeviceObject.h"

bool DepthStencilObject::Init()
{
    LOG("���� ���ٽ� ������Ʈ �ʱ�ȭ ����");

	Microsoft::WRL::ComPtr<ID3D12Device> device = DEVICE_OBJ->GetDevice();

	HRESULT hr{};

    dsvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	LOG("���� ���ٽ� �� ���� ����");

	D3D12_DESCRIPTOR_HEAP_DESC depthStencilDesc{};
	depthStencilDesc.NumDescriptors = 1;						//���� ���۴� �ϳ��� ���
	depthStencilDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		//DSV Ÿ��
	depthStencilDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	//CPU������ �����ϹǷ� �÷��� ����
	depthStencilDesc.NodeMask = 0;								//�̱� GPU ȯ���̹Ƿ� 0

	hr = device->CreateDescriptorHeap(&depthStencilDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(hr))
	{
		hr = device->GetDeviceRemovedReason();
	}
	CHECK(SUCCEEDED(hr), "���� ���ٽ� �� ���� ����", false);

	LOG("���� ���ٽ� �� ���� ����");

	CreateDepthStencilState();

    LOG("���� ���ٽ� ������Ʈ �ʱ�ȭ ����");
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
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//���ҽ��� ���� (1D, 2D, 3D)
	depthStencilDesc.Alignment = 0;			//�޸� ���� ��� (0�� �⺻ ����)
	depthStencilDesc.Width = static_cast<UINT64>(engineSetting.resolution.x);
	depthStencilDesc.Height = static_cast<UINT64>(engineSetting.resolution.y);
	depthStencilDesc.DepthOrArraySize = 1;	//���� ��(3D �ؽ�ó�� ���) �Ǵ� �迭 ũ��
	depthStencilDesc.MipLevels = 1;			//���� Mipmap ���� �� (1�� Mipmap ����)
	depthStencilDesc.Format = LITERAL::DEPTH_STENCIL_FORMAT;	//�ȼ� ����
	depthStencilDesc.SampleDesc.Count = engineSetting.state4XMSAA ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = engineSetting.state4XMSAA ? (engineSetting.quality4XMSAA - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;		//�ؽ�ó ���̾ƿ� (D3D12_TEXTURE_LAYOUT_UNKNOWN�� �⺻)
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//���ҽ� ��� ���� (D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL�� ���� ���ٽ� ���۷� ���)

	//������ �ʱⰪ�� ���ϴ� ����ü, ���� ���ٽ� ���� ��������
	D3D12_CLEAR_VALUE clearValue{ CD3DX12_CLEAR_VALUE{LITERAL::DEPTH_STENCIL_FORMAT, 1.0f, 0} };

	CD3DX12_HEAP_PROPERTIES heapProperty{ D3D12_HEAP_TYPE_DEFAULT };	//GPU�� ���� �����Ǵ� �⺻ �� ���
	//���� ���ٽ� ���� ����
	hr = device->CreateCommittedResource(
		&heapProperty,					//�� �Ӽ�
		D3D12_HEAP_FLAG_NONE,			//�� �÷���(Ư���� �� �ɼ� ����)
		&depthStencilDesc,				//���ҽ� ����(���� ���ٽ� ���� ����)
		D3D12_RESOURCE_STATE_COMMON,	//�ʱ� ���ҽ� ����
		&clearValue,					//�ʱ�ȭ �� ���� ���̰�(1.0)�� ���ٽ� ��(0)
		IID_PPV_ARGS(&depthStencilBuffer));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;	//�߰����� �÷��� ���� (ex. �б� ����)
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	//�ؽ�ó�� ����
	dsvDesc.Format = LITERAL::DEPTH_STENCIL_FORMAT;	//���� ���ٽ� ����
	dsvDesc.Texture2D.MipSlice = 0;

	dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

	//���� ���ٽ� �� ����
	device->CreateDepthStencilView(
		depthStencilBuffer.Get(),
		&dsvDesc,
		dsvHandle);	//DSV�� ����� ��ũ���� ���� ��ġ

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
