#include "Mesh.h"

bool Mesh::Load(FString FileName)
{
    __super::Load(FileName);
    LOG("%s 모델 로드 시작", fileName.c_str());

    FString fullPath = path + fileName;

    // Assimp Importer 객체 생성
    Assimp::Importer importer{};

    //TODO 중복정의 해제 LNK4006
    const aiScene* scene = importer.ReadFile(EngineUtil::String::ConvertString<std::string>(fullPath),
        aiProcess_Triangulate |            // 모든 폴리곤을 삼각형으로 분할
        aiProcess_FlipUVs |                // UV 좌표 뒤집기
        aiProcess_JoinIdenticalVertices |  // 중복된 정점 병합
        aiProcess_SortByPType);            // 원시 유형별로 정렬

    // 로드 실패 시 오류 메시지 출력
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        //TODO 에레메세지 형식 변경
        LOG("로드 실패 : %hs", importer.GetErrorString());
        return false;
    }

    LOG("%s 메쉬 갯수 : %d", fileName.c_str(), scene->mNumMeshes);

    //TODO 메쉬 불러오는 로직

    LOG("%s 모델 로드 성공", fileName.c_str());
    return true;
}
