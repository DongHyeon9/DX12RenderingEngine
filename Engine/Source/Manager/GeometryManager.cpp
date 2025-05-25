#include "GeometryManager.h"

bool GeometryManager::Init()
{
	LOG("������Ʈ�� �Ŵ��� �ʱ�ȭ ����");

	LOG("������Ʈ�� �Ŵ��� �ʱ�ȭ ����");
	return true;
}

MeshData GeometryManager::CreatePlane(float Width, float Height, uint32 RowSlice, uint32 ColumnSlice) const
{
	LOG("Plane ���� ����");
	MeshData result{};

	//Vertex �� Index �迭 ����
	std::vector<Vertex>& vertices = result.vertices;
	std::vector<uint32>& indices = result.indices;

	uint32 rowCount = RowSlice + 1;
	uint32 colCount = ColumnSlice + 1;

	vertices.reserve((rowCount + 1) * (colCount + 1));
	indices.reserve(rowCount * colCount * 6);

	//���������� ����� Width�� Height�� GLOBAL::UNIT�� ���� ��
	float unitWidth = Width * GLOBAL::PARSE_UNIT;
	float unitHeight = Height * GLOBAL::PARSE_UNIT;

	//RowSlice�� ColumnSlice�� ���� ���� ���� ���
	float rowInterval = unitWidth / rowCount;	// �� ���� ����
	float colInterval = unitHeight / colCount;	// �� ���� ����

	//Vertex ����
	for (uint32 i = 0; i <= colCount; ++i)
	{
		for (uint32 j = 0; j <= rowCount; ++j)
		{
			Vertex v{};

			//x, z ��ġ ��� (y�� ����, z �������� �������� ���� ����)
			float x = -unitWidth / 2.0f + j * rowInterval;
			float z = -unitHeight / 2.0f + i * colInterval;
			v.position = Vector3(x, 0.0f, z);

			//�븻�� ����� ���ϵ��� ����
			v.normal = GLOBAL::UP;

			//�ؽ�ó ��ǥ (u, v) ���
			v.texcoord = Vector2(static_cast<float>(j) / rowCount, 1.0f - static_cast<float>(i) / colCount);

			v.tangent = GLOBAL::RIGHT;

			//���ؽ� �迭�� �߰�
			vertices.emplace_back(v);
		}
	}
	LOG("���ؽ� ����");

	// �ε��� ���� (�ﰢ������ ����� ����)
	for (uint32 i = 0; i < colCount; ++i)
	{
		for (uint32 j = 0; j < rowCount; ++j)
		{
			// �ε��� ���
			uint32 index0 = i * (rowCount + 1) + j;
			uint32 index1 = index0 + 1;
			uint32 index2 = index0 + (rowCount + 1);
			uint32 index3 = index2 + 1;

			// ù ��° �ﰢ��
			indices.emplace_back(index0);
			indices.emplace_back(index2);
			indices.emplace_back(index1);

			// �� ��° �ﰢ��
			indices.emplace_back(index1);
			indices.emplace_back(index2);
			indices.emplace_back(index3);
		}
	}
	LOG("�ε��� ����");

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	LOG("Plane ���� �Ϸ�");
	return result;
}

MeshData GeometryManager::CreateBox(const Vector3& BoxExtent) const
{
	LOG("Box ���� ����");
	MeshData result{};

	std::vector<Vertex>& vertices = result.vertices;
	std::vector<uint32>& indices = result.indices;

	vertices.reserve(24);
	indices.reserve(36);

	const float xUnit = BoxExtent.x * GLOBAL::PARSE_UNIT * 0.5f;
	const float yUnit = BoxExtent.y * GLOBAL::PARSE_UNIT * 0.5f;
	const float zUnit = BoxExtent.z * GLOBAL::PARSE_UNIT * 0.5f;

	std::vector<Vector3> positions{};
	std::vector<Vector3> normals{};
	std::vector<Vector2> texcoords{};
	std::vector<Vector3> tangents{};

	//����
	positions.emplace_back(Vector3(-xUnit, yUnit, -zUnit));
	positions.emplace_back(Vector3(xUnit, yUnit, -zUnit));
	positions.emplace_back(Vector3(xUnit, -yUnit, -zUnit));
	positions.emplace_back(Vector3(-xUnit, -yUnit, -zUnit));
	texcoords.emplace_back(Vector2(0.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 1.0f));
	for (size_t i = 0; i < 4; ++i)
	{
		normals.emplace_back(GLOBAL::BACKWARD);
		tangents.emplace_back(GLOBAL::LEFT);
	}

	//������
	positions.emplace_back(Vector3(xUnit, yUnit, -zUnit));
	positions.emplace_back(Vector3(xUnit, yUnit, zUnit));
	positions.emplace_back(Vector3(xUnit, -yUnit, zUnit));
	positions.emplace_back(Vector3(xUnit, -yUnit, -zUnit));
	texcoords.emplace_back(Vector2(0.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 1.0f));
	for (size_t i = 0; i < 4; ++i)
	{
		normals.emplace_back(GLOBAL::RIGHT);
		tangents.emplace_back(GLOBAL::BACKWARD);
	}

	//����
	positions.emplace_back(Vector3(-xUnit, yUnit, zUnit));
	positions.emplace_back(Vector3(-xUnit, yUnit, -zUnit));
	positions.emplace_back(Vector3(-xUnit, -yUnit, -zUnit));
	positions.emplace_back(Vector3(-xUnit, -yUnit, zUnit));
	texcoords.emplace_back(Vector2(0.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 1.0f));
	for (size_t i = 0; i < 4; ++i)
	{
		normals.emplace_back(GLOBAL::LEFT);
		tangents.emplace_back(GLOBAL::FORWARD);
	}

	//���
	positions.emplace_back(Vector3(-xUnit, yUnit, zUnit));
	positions.emplace_back(Vector3(xUnit, yUnit, zUnit));
	positions.emplace_back(Vector3(xUnit, yUnit, -zUnit));
	positions.emplace_back(Vector3(-xUnit, yUnit, -zUnit));
	texcoords.emplace_back(Vector2(1.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 0.0f));
	for (size_t i = 0; i < 4; ++i)
	{
		normals.emplace_back(GLOBAL::UP);
		tangents.emplace_back(GLOBAL::LEFT);
	}

	//�ϴ�
	positions.emplace_back(Vector3(-xUnit, -yUnit, -zUnit));
	positions.emplace_back(Vector3(xUnit, -yUnit, -zUnit));
	positions.emplace_back(Vector3(xUnit, -yUnit, zUnit));
	positions.emplace_back(Vector3(-xUnit, -yUnit, zUnit));
	texcoords.emplace_back(Vector2(0.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 1.0f));
	for (size_t i = 0; i < 4; ++i)
	{
		normals.emplace_back(GLOBAL::DOWN);
		tangents.emplace_back(GLOBAL::LEFT);
	}

	//�ĸ�
	positions.emplace_back(Vector3(xUnit, yUnit, zUnit));
	positions.emplace_back(Vector3(-xUnit, yUnit, zUnit));
	positions.emplace_back(Vector3(-xUnit, -yUnit, zUnit));
	positions.emplace_back(Vector3(xUnit, -yUnit, zUnit));
	texcoords.emplace_back(Vector2(0.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 0.0f));
	texcoords.emplace_back(Vector2(1.0f, 1.0f));
	texcoords.emplace_back(Vector2(0.0f, 1.0f));
	for (size_t i = 0; i < 4; ++i)
	{
		normals.emplace_back(GLOBAL::FORWARD);
		tangents.emplace_back(GLOBAL::LEFT);
	}

	LOG("���ؽ� ����");

	indices = {
		0,1,3,1,2,3,		//����
		4,5,7,5,6,7,		//������
		8,9,11,9,10,11,		//����
		12,13,15,13,14,15,	//���
		16,17,19,17,18,19,	//�ϴ�
		20,21,23,21,22,23	//�ĸ�
	};
	LOG("�ε��� ����");

	for (size_t i = 0; i < positions.size(); i++) {
		Vertex v{};
		v.position = positions[i];
		v.normal = normals[i];
		v.texcoord = texcoords[i];
		v.tangent = tangents[i];
		vertices.emplace_back(v);
	}

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	LOG("Box ���� �Ϸ�");
	return result;
}

MeshData GeometryManager::CreateCapsule(float Radius, float Height, uint32 NumSlices, uint32 HemiSphereStacks) const
{
	LOG("Capsule ���� ����");
	MeshData result{};

	// �Է°��� ����ȭ
	const float unitRadius = Radius * GLOBAL::PARSE_UNIT;
	const float unitHeight = Height * GLOBAL::PARSE_UNIT;
	const float halfHeight = unitHeight * 0.5f;

	//�ּڰ� ����
	NumSlices += 3;
	HemiSphereStacks += 1;
	HemiSphereStacks = HemiSphereStacks & 1 ? HemiSphereStacks + 1 : HemiSphereStacks;
	uint32 sphereStack = HemiSphereStacks * 2;

	//�� Slice�� Stack�� ���� ���� ���
	const float sliceAngle = -DirectX::XM_2PI / NumSlices;
	const float stackAngle = -DirectX::XM_PI / sphereStack;

	std::vector<Vertex>& vertices = result.vertices;
	std::vector<uint32>& indices = result.indices;

	const uint32 sliceCount = NumSlices + 1;
	const uint32 stackCount = sphereStack + 1;

	vertices.reserve((HemiSphereStacks * 2 + 1) * (NumSlices + 1));
	indices.reserve((HemiSphereStacks * 2) * NumSlices * 6);

	//���ؽ� ����
	for (uint32 i = 0; i <= sphereStack; ++i) {
		const float phi = stackAngle * i;
		Vector3 stackStartPoint = Vector3::Transform(Vector3(0.0f, -unitRadius, 0.0f), Matrix::CreateRotationZ(phi));

		for (uint32 j = 0; j <= NumSlices; ++j) {
			const float theta = sliceAngle * j;

			Vertex v{};
			v.position = Vector3::Transform(stackStartPoint, Matrix::CreateRotationY(sliceAngle * static_cast<float>(j)));
			v.normal = v.position;
			v.normal.Normalize();
			v.texcoord = Vector2(static_cast<float>(j) / NumSlices, 0.0f);

			v.tangent.x = -Radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = Radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vertices.emplace_back(v);
		}
	}
	LOG("���ؽ� ����");

	//�ؽ�ó ��ǥ ������ ���� �� ���
	const float totalHeight = Radius * 2 + Height;
	const float radiusRatio = Radius / totalHeight;
	const float texcoordUnit = radiusRatio / (HemiSphereStacks - 1);
	size_t halfIndex = static_cast<size_t>((sphereStack * 0.5f) * (NumSlices + 1));

	//��ܹݱ�
	for (size_t i = halfIndex; i < vertices.size(); ++i)
	{
		//ĸ�� �߾ӿ� �� ���ؽ���
		if (i < halfIndex + NumSlices + 1)
		{
			vertices[i].position.y = 0.0f;
			vertices[i].texcoord.y = 0.5f;
		}
		//������ ���ؽ��� ����
		else
		{
			vertices[i].position = Vector3::Transform(vertices[i].position, Matrix::CreateTranslation(0.0f, halfHeight, 0.0f));
			vertices[i].texcoord.y = radiusRatio - texcoordUnit * ((i - halfIndex - NumSlices - 1) / (NumSlices + 1));
		}
	}
	LOG("��� �ݱ� �̵�, �ؽ�ó ��ǥ ����");

	//�ϴܹݱ�
	for (size_t i = 0; i < halfIndex; ++i)
	{
		//�ϴ� �ݱ� �̵�
		vertices[i].position = Vector3::Transform(vertices[i].position, Matrix::CreateTranslation(0.0f, -halfHeight, 0.0f));
		vertices[i].texcoord.y = 1.0f - texcoordUnit * (i / (NumSlices + 1));
	}
	LOG("�ϴ� �ݱ� �̵�, �ؽ�ó ��ǥ ����");

	//�ε��� ����
	for (uint32 i = 0; i < sphereStack; ++i)
	{
		const uint32 offset = (NumSlices + 1) * i;
		for (uint32 j = 0; j < NumSlices; ++j)
		{

			indices.emplace_back(offset + j);
			indices.emplace_back(offset + j + NumSlices + 1);
			indices.emplace_back(offset + j + 1 + NumSlices + 1);

			indices.emplace_back(offset + j);
			indices.emplace_back(offset + j + 1 + NumSlices + 1);
			indices.emplace_back(offset + j + 1);
		}
	}
	LOG("�ε��� ����");

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	LOG("Capsule ���� �Ϸ�");
	return result;
}

MeshData GeometryManager::CreateCylinder(float BottomRadius, float TopRadius, float Height, uint32 NumSlices) const
{
	LOG("Cylinder ���� ����");
	MeshData result{};

	NumSlices += 3;

	//���� ����ȭ, ���� ���
	const float unitAngle = -DirectX::XM_2PI / NumSlices;
	const float unitBottomRadius = BottomRadius * GLOBAL::PARSE_UNIT;
	const float unitTopRadius = TopRadius * GLOBAL::PARSE_UNIT;
	const float unitHeight = Height * GLOBAL::PARSE_UNIT;
	const float halfHeight = unitHeight * 0.5f;

	std::vector<Vertex>& vertices = result.vertices;
	std::vector<uint32>& indices = result.indices;

	vertices.reserve((NumSlices + 1) * 2 + 2);
	indices.reserve(NumSlices * 12);

	//�ϴ� ���ؽ� ����
	for (uint32 i = 0; i <= NumSlices; i++) {
		Vertex v{};
		v.position = Vector3::Transform(Vector3(unitBottomRadius, -halfHeight, 0.0f), Matrix::CreateRotationY(unitAngle * i));
		v.normal = v.position;
		v.normal.Normalize();
		v.texcoord = Vector2(static_cast<float>(i) / NumSlices, 1.0f);
		v.tangent = v.normal.Cross(GLOBAL::UP);
		
		vertices.emplace_back(v);
	}
	const uint32 topStart{ static_cast<uint32>(vertices.size()) };
	LOG("�ϴ� ���ؽ� ����");

	//��� ���ؽ� ����
	for (uint32 i = 0; i <= NumSlices; i++) {
		Vertex v{};
		v.position = Vector3::Transform(Vector3(unitTopRadius, halfHeight, 0.0f), Matrix::CreateRotationY(unitAngle * i));
		v.normal = v.position;
		v.normal.Normalize();
		v.texcoord = Vector2(static_cast<float>(i) / NumSlices, 0.0f);
		v.tangent = v.normal.Cross(GLOBAL::UP);;

		vertices.emplace_back(v);
	}
	const uint32 bottomCenterIdx{ static_cast<uint32>(vertices.size()) };
	LOG("��� ���ؽ� ����");

	//�ϴ� �߽� ���ؽ� ����
	{
		Vertex v{};
		v.position = Vector3(0.0f, -halfHeight, 0.0f);
		v.normal = GLOBAL::DOWN;
		v.texcoord = Vector2(0.0f, 0.0f);
		v.tangent = GLOBAL::RIGHT;

		vertices.emplace_back(v);
	}
	const uint32 topCenterIdx{ static_cast<uint32>(vertices.size()) };

	//��� �߽� ���ؽ� ����
	{
		Vertex v{};
		v.position = Vector3(0.0f, halfHeight, 0.0f);
		v.normal = GLOBAL::UP;
		v.texcoord = Vector2(0.0f, 1.0f);
		v.tangent = GLOBAL::LEFT;

		vertices.emplace_back(v);
	}

	//���� �ε��� ����
	for (uint32 i = 0; i < NumSlices; i++) {
		//���� �ﰢ�� ����
		indices.emplace_back(i);
		indices.emplace_back(i + 1 + topStart);
		indices.emplace_back(i + 1);

		//���� ���ﰢ�� ����
		indices.emplace_back(i);
		indices.emplace_back(i + topStart);
		indices.emplace_back(i + 1 + topStart);
	}

	//�ϴ� �ε��� ����
	for (uint32 i = 0; i < NumSlices; ++i) {
		indices.emplace_back(bottomCenterIdx);
		indices.emplace_back(i);
		indices.emplace_back(i + 1);
	}

	//��� �ε��� ����
	for (uint32 i = 0; i < NumSlices; ++i) {
		indices.emplace_back(topCenterIdx);
		indices.emplace_back(topStart + i + 1);
		indices.emplace_back(topStart + i);
	}

	LOG("�ε��� ����");

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	LOG("Cylinder ���� �Ϸ�");
	return result;
}

MeshData GeometryManager::CreateSphere(float Radius, uint32 NumSlices, uint32 NumStacks) const
{
	LOG("Sphere ���� ����");
	MeshData result{};

	// �Է°��� ����ȭ
	const float unitRadius = Radius * GLOBAL::PARSE_UNIT;

	NumSlices += 3;
	NumStacks += 2;

	//�� Stack�� Slice�� ���� ���� ���
	const float sliceAngle = -DirectX::XM_2PI / NumSlices;
	const float stackAngle = -DirectX::XM_PI / NumStacks;

	std::vector<Vertex>& vertices = result.vertices;
	std::vector<uint32>& indices = result.indices;

	vertices.reserve((NumStacks + 1) * (NumSlices + 1));
	indices.reserve(NumStacks * NumSlices * 6);

	//���ؽ� ����
	for (uint32 i = 0; i <= NumStacks; ++i) {
		const float phi = stackAngle * i;
		Vector3 stackStartPoint = Vector3::Transform(Vector3(0.0f, -unitRadius, 0.0f), Matrix::CreateRotationZ(stackAngle * i));

		for (uint32 j = 0; j <= NumSlices; ++j) {
			Vertex v{};
			const float theta = sliceAngle * j;
			v.position = Vector3::Transform(stackStartPoint, Matrix::CreateRotationY(sliceAngle * static_cast<float>(j)));
			v.normal = v.position;
			v.normal.Normalize();
			v.texcoord = Vector2(static_cast<float>(j) / NumSlices, 1.0f - static_cast<float>(i) / NumStacks);

			v.tangent.x = -Radius * sinf(phi) * sinf(theta);
			v.tangent.y = 0.0f;
			v.tangent.z = Radius * sinf(phi) * cosf(theta);
			v.tangent.Normalize();

			vertices.emplace_back(v);
		}
	}
	LOG("���ؽ� ����");

	//�ε��� ����
	for (uint32 i = 0; i < NumStacks; ++i)
	{
		const uint32 offset = (NumSlices + 1) * i;
		for (uint32 j = 0; j < NumSlices; ++j)
		{
			indices.emplace_back(offset + j);
			indices.emplace_back(offset + j + NumSlices + 1);
			indices.emplace_back(offset + j + 1 + NumSlices + 1);

			indices.emplace_back(offset + j);
			indices.emplace_back(offset + j + 1 + NumSlices + 1);
			indices.emplace_back(offset + j + 1);
		}
	}
	LOG("�ε��� ����");

	vertices.shrink_to_fit();
	indices.shrink_to_fit();

	LOG("Sphere ���� �Ϸ�");
	return result;
}