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
	* 평면을 생성한다
	* @param[in] Width 평면의 가로 길이
	* @param[in] Height 평면의 세로 길이
	* @param[in] RowSlice 평면의 열 갯수
	* @param[in] ColumnSlice 평면의 행 갯수
	* @return 평면의 MeshData
	*/
	MeshData CreatePlane(float Width, float Height, uint32 RowSlice, uint32 ColumnSlice) const;
	/*
	* 박스를 생성한다
	* @param[in] X 박스의 가로 길이
	* @param[in] Y 박스의 높이
	* @param[in] Z 박스의 세로 길이
	* @return 박스의 MeshData
	*/
	MeshData CreateBox(const Vector3& BoxExtent) const;
	/*
	* 캡슐을 생성한다
	* 최종 높이는 Radius * 2 + Height가 된다
	* @param[in] Radius 상,하단 반구의 반지름
	* @param[in] Height 원기둥의 높이
	* @param[in] NumSlices 캡슐의 Slice 갯수, 0일 경우 3개가 된다
	* @param[in] HemiSphereStacks 상,하단 반구의 Stack의 수, 0일 경우 1개가 된다
	* @return 캡슐의 MeshData
	*/
	MeshData CreateCapsule(float Radius, float Height, uint32 NumSlices, uint32 HemiSphereStacks) const;
	/*
	* 원기둥을 생성한다
	* @param[in] BottomRadius 하단의 지름
	* @param[in] TopRadius 상단의 지름
	* @param[in] Height 높이
	* @param[in] NumSlices Slice 갯수, 0일 경우 3개가 된다
	* @return 원기둥의 MeshData
	*/
	MeshData CreateCylinder(float BottomRadius, float TopRadius, float Height, uint32 NumSlices) const;
	/*
	* 구체를 생성한다
	* @param[in] Radius 반지름
	* @param[in] NumSlices Slice 갯수, 0일 경우 3개가 된다
	* @param[in] NumStacks Stack의 갯수, 0일 경우 1개가 된다
	* @return 구체의 MeshData
	*/
	MeshData CreateSphere(float Radius, uint32 NumSlices, uint32 NumStacks) const;

protected:

private:

};

