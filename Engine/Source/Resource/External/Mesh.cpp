#include "Mesh.h"

bool Mesh::Load(FString FileName)
{
    __super::Load(FileName);
    LOG("%s �� �ε� ����", fileName.c_str());

    FString fullPath = path + fileName;

    // Assimp Importer ��ü ����
    Assimp::Importer importer{};

    //TODO �ߺ����� ���� LNK4006
    const aiScene* scene = importer.ReadFile(EngineUtil::String::ConvertString<std::string>(fullPath),
        aiProcess_Triangulate |            // ��� �������� �ﰢ������ ����
        aiProcess_FlipUVs |                // UV ��ǥ ������
        aiProcess_JoinIdenticalVertices |  // �ߺ��� ���� ����
        aiProcess_SortByPType);            // ���� �������� ����

    // �ε� ���� �� ���� �޽��� ���
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        //TODO �����޼��� ���� ����
        LOG("�ε� ���� : %hs", importer.GetErrorString());
        return false;
    }

    LOG("%s �޽� ���� : %d", fileName.c_str(), scene->mNumMeshes);

    //TODO �޽� �ҷ����� ����

    LOG("%s �� �ε� ����", fileName.c_str());
    return true;
}
