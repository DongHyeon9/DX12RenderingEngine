#include "DepthStencilObject.h"
#include "Engine\Engine.h"

bool DepthStencilObject::Init(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
    LOG("���� ���ٽ� ������Ʈ �ʱ�ȭ ����");

	HRESULT hr{};

    dsvDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	LOG("���� ���ٽ� �� ���� ����");

	D3D12_DESCRIPTOR_HEAP_DESC depthStencilDesc{};
	depthStencilDesc.NumDescriptors = 1;						//���� ���۴� �ϳ��� ���
	depthStencilDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;		//DSV Ÿ��
	depthStencilDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;	//CPU������ �����ϹǷ� �÷��� ����
	depthStencilDesc.NodeMask = 0;								//�̱� GPU ȯ���̹Ƿ� 0

	hr = Device->CreateDescriptorHeap(&depthStencilDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(hr))
	{
		hr = Device->GetDeviceRemovedReason();
	}
	CHECK(SUCCEEDED(hr), "���� ���ٽ� �� ���� ����", false);

	LOG("���� ���ٽ� �� ���� ����");

    LOG("���� ���ٽ� ������Ʈ �ʱ�ȭ ����");
    return true;
}

void DepthStencilObject::ResetDepthStencilBuffer()
{
	depthStencilBuffer.Reset();
}

bool DepthStencilObject::CreateDepthStencilBuffer(Microsoft::WRL::ComPtr<ID3D12Device> Device)
{
	EngineSetting engineSetting = GEngine->GetEngineSetting();
	HRESULT hr{};

	D3D12_RESOURCE_DESC depthStencilDesc{};
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	//���ҽ��� ���� (1D, 2D, 3D)
	depthStencilDesc.Alignment = 0;			//�޸� ���� ��� (0�� �⺻ ����)
	depthStencilDesc.Width = static_cast<UINT64>(engineSetting.resolution.x);
	depthStencilDesc.Height = static_cast<UINT64>(engineSetting.resolution.y);
	depthStencilDesc.DepthOrArraySize = 1;	//���� ��(3D �ؽ�ó�� ���) �Ǵ� �迭 ũ��
	depthStencilDesc.MipLevels = 1;			//���� Mipmap ���� �� (1�� Mipmap ����)
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;	//�ȼ� ����
	depthStencilDesc.SampleDesc.Count = engineSetting.state4XMSAA ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = engineSetting.state4XMSAA ? (engineSetting.quality4XMSAA - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;		//�ؽ�ó ���̾ƿ� (D3D12_TEXTURE_LAYOUT_UNKNOWN�� �⺻)
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;	//���ҽ� ��� ���� (D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL�� ���� ���ٽ� ���۷� ���)

	//������ �ʱⰪ�� ���ϴ� ����ü, ���� ���ٽ� ���� ��������
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Format = LITERAL::DEPTH_STENCIL_FORMAT;
	clearValue.DepthStencil.Depth = 1.0f;
	clearValue.DepthStencil.Stencil = 0;

	CD3DX12_HEAP_PROPERTIES heapProperty{ D3D12_HEAP_TYPE_DEFAULT };	//GPU�� ���� �����Ǵ� �⺻ �� ���
	//���� ���ٽ� ���� ����
	hr = Device->CreateCommittedResource(
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
	Device->CreateDepthStencilView(
		depthStencilBuffer.Get(),
		&dsvDesc,
		dsvHandle);	//DSV�� ����� ��ũ���� ���� ��ġ

	return true;
}
