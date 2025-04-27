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
        aiProcess_SortByPType|             // ���� �������� ����
        aiProcess_CalcTangentSpace);       // ź��Ʈ ������ �߰�

    // �ε� ���� �� ���� �޽��� ���
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        LOG("�ε� ���� : %hs", importer.GetErrorString());
        return false;
    }

    LOG("%s �޽� ���� : %d", fileName.c_str(), scene->mNumMeshes);

    ProcessNode(scene->mRootNode, scene, {});

    LOG("%s �� �ε� ����", fileName.c_str());
    return true;
}

void Mesh::SetMeshData(const std::vector<MeshData>& Meshes)
{
    meshes.assign(Meshes.begin(), Meshes.end());
}

void Mesh::AddMeshData(const MeshData& Mesh)
{
    meshes.emplace_back(Mesh);
}

void Mesh::ProcessNode(aiNode* Node, const aiScene* Scene, Matrix Transform)
{
    Matrix mat{};
    ai_real* temp = &Node->mTransformation.a1;
    float* matTemp = &mat._11;
    for (size_t i = 0; i < 16; ++i)
    {
        matTemp[i] = float(temp[i]);
    }
    mat = mat.Transpose() * Transform;

    for (uint32 i = 0; i < Node->mNumMeshes; ++i)
    {
        aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
        auto newMesh = ProcessMesh(mesh, Scene);

        for (auto& v : newMesh.vertices)
            v.position = Vector3::Transform(v.position, mat);

        meshes.emplace_back(newMesh);
    }

    for (uint32 i = 0; i < Node->mNumChildren; ++i)
        ProcessNode(Node->mChildren[i], Scene, mat);
}

MeshData Mesh::ProcessMesh(aiMesh* Mesh, const aiScene* Scene)
{
    MeshData meshData{};

    for (uint32 i = 0; i < Mesh->mNumVertices; ++i)
    {
        Vertex vertex{};

        // ��ġ ����
        vertex.position = {
            Mesh->mVertices[i].x,
            Mesh->mVertices[i].y,
            Mesh->mVertices[i].z
        };

        // �븻 ����
        vertex.normal = Mesh->HasNormals() ?
            Vector3{
                Mesh->mNormals[i].x,
                Mesh->mNormals[i].y,
                Mesh->mNormals[i].z
        } :
            Vector3{};

        // ź��Ʈ ����
        vertex.tangent = Mesh->HasTangentsAndBitangents() ?
            Vector3{
                Mesh->mTangents[i].x,
                Mesh->mTangents[i].y,
                Mesh->mTangents[i].z
        } :
            Vector3{};

        // �ؽ�ó ��ǥ (ù ��° ä�θ� ���)
        if (Mesh->mTextureCoords[0])
        {
            vertex.texcoord = {
                Mesh->mTextureCoords[0][i].x,
                Mesh->mTextureCoords[0][i].y
            };
        }
        else
        {
            vertex.texcoord = Vector2{};
        }

        meshData.vertices.emplace_back(vertex);
    }

    // �ε��� ����
    for (uint32 i = 0; i < Mesh->mNumFaces; ++i)
    {
        aiFace face = Mesh->mFaces[i];
        for (uint32 j = 0; j < face.mNumIndices; ++j)
        {
            meshData.indices.emplace_back(static_cast<uint32>(face.mIndices[j]));
        }
    }

    return meshData;
}
