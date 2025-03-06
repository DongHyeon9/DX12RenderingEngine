#pragma once
#include "Header\EngineCore.h"

class GeometryManager
{
	DECLARE_SINGLETON(GeometryManager);
public:

protected:

private:

public:
	bool Init();
	/*
	* ����� �����Ѵ�
	* @param[in] Width ����� ���� ����
	* @param[in] Height ����� ���� ����
	* @param[in] RowSlice ����� �� ����
	* @param[in] ColumnSlice ����� �� ����
	* @return ����� MeshData
	*/
	MeshData CreatePlane(float Width, float Height, uint32 RowSlice, uint32 ColumnSlice) const;
	/*
	* �ڽ��� �����Ѵ�
	* @param[in] X �ڽ��� ���� ����
	* @param[in] Y �ڽ��� ����
	* @param[in] Z �ڽ��� ���� ����
	* @return �ڽ��� MeshData
	*/
	MeshData CreateBox(const Vector3& BoxExtent) const;
	/*
	* ĸ���� �����Ѵ�
	* ���� ���̴� Radius * 2 + Height�� �ȴ�
	* @param[in] Radius ��,�ϴ� �ݱ��� ������
	* @param[in] Height ������� ����
	* @param[in] NumSlices ĸ���� Slice ����, 0�� ��� 3���� �ȴ�
	* @param[in] HemiSphereStacks ��,�ϴ� �ݱ��� Stack�� ��, 0�� ��� 1���� �ȴ�
	* @return ĸ���� MeshData
	*/
	MeshData CreateCapsule(float Radius, float Height, uint32 NumSlices, uint32 HemiSphereStacks) const;
	/*
	* ������� �����Ѵ�
	* @param[in] BottomRadius �ϴ��� ����
	* @param[in] TopRadius ����� ����
	* @param[in] Height ����
	* @param[in] NumSlices Slice ����, 0�� ��� 3���� �ȴ�
	* @return ������� MeshData
	*/
	MeshData CreateCylinder(float BottomRadius, float TopRadius, float Height, uint32 NumSlices) const;
	/*
	* ��ü�� �����Ѵ�
	* @param[in] Radius ������
	* @param[in] NumSlices Slice ����, 0�� ��� 3���� �ȴ�
	* @param[in] NumStacks Stack�� ����, 0�� ��� 1���� �ȴ�
	* @return ��ü�� MeshData
	*/
	MeshData CreateSphere(float Radius, uint32 NumSlices, uint32 NumStacks) const;

protected:

private:

};

