#include "PerfectEngine/PerfectModelLoader.h"
#include "PerfectEngine/PerfectObjModelLoader.h"
#include "PerfectEngine/PerfectMesh.h"

namespace Perfect
{
    ModelLoaderPtr ModelLoader::CreateLoader(ModelType type)
    {
        switch (type)
        {
        case ModelType::EObj:
            return std::make_shared<ObjModelLoader>();
        default:
            return nullptr;
        }
    }

    /**
    * \brief 提取顶点数据
    * \param mesh Mesh
    */
    void ModelLoader::ExtractVertexData(Mesh& mesh)
    {
        mesh.m_Vertices = std::move(m_Vertices);
        mesh.m_Normals = std::move(m_Normals);
        mesh.m_Uvs = std::move(m_Uvs);
        mesh.m_Tangents = std::move(m_Tangents);
        mesh.m_Triangles = std::move(m_Triangles);
    }
}
