#include "ResourceLoader.h"

#include <chrono>


#include "ResourceManager.h"

namespace SaplingEngine
{
	/**
	 * \brief ����Mesh��Դ
	 * \param meshName Mesh����
	 * \return Resourceָ��
	 */
	IResource* LoadMeshResource(const std::string& meshName)
	{
		//��ȡMesh����
		const auto* pMeshConfig = ResourceManager::GetMeshConfig(meshName);
		
		//���ļ�
		std::fstream fs;
		fs.open(std::get<0>(*pMeshConfig), std::ios::in);

		//����ļ����Ƿ�ɹ�
		if (!fs.is_open())
		{
			fs.close();
			std::cout << meshName << " open failed!" << std::endl;
			return nullptr;
		}

		//�������ݺ���������
		std::vector<VertexData> vertexDatas(std::get<1>(*pMeshConfig));
		std::vector<uint16_t> indices(std::get<2>(*pMeshConfig));

		//����
		std::string line;
		float fValue[3];
		uint32_t uValue[9];
		std::vector<VertexData>::size_type vIndex = 0, vnIndex = 0, vtIndex = 0, iIndex = 0;
		if (vertexDatas.empty())
		{
			while (!fs.eof())
			{
				//��ȡһ��
				std::getline(fs, line);

				//��������
				if (line[0] == 'v' && line[1] == ' ')
				{
					if (sscanf_s(line.c_str(), "v %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
					{
						if (vertexDatas.size() <= vIndex)
						{
							vertexDatas.emplace_back();
						}
						auto& position = vertexDatas[vIndex++].PositionOS;
						position.x = fValue[0];
						position.y = fValue[1];
						position.z = fValue[2];
					}
				}
				else if (line[0] == 'v' && line[1] == 'n')
				{
					if (sscanf_s(line.c_str(), "vn %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
					{
						if (vertexDatas.size() <= vnIndex)
						{
							vertexDatas.emplace_back();
						}
						auto& normal = vertexDatas[vnIndex++].NormalOS;
						normal.x = fValue[0];
						normal.y = fValue[1];
						normal.z = fValue[2];
					}
				}
				else if (line[0] == 'v' && line[1] == 't')
				{
					if (sscanf_s(line.c_str(), "vt %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
					{
						if (vertexDatas.size() <= vtIndex)
						{
							vertexDatas.emplace_back();
						}
						auto& uv0 = vertexDatas[vtIndex++].UV0;
						uv0.x = fValue[0];
						uv0.y = fValue[1];
					}
				}
				else if (line[0] == 'f' && line[1] == ' ')
				{
					if (sscanf_s(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", uValue, uValue + 1, uValue + 2, uValue + 3, uValue + 4, uValue + 5, uValue + 6, uValue + 7, uValue + 8) == 9)
					{
						indices.push_back(static_cast<uint16_t>(uValue[0] - 1));
						indices.push_back(static_cast<uint16_t>(uValue[3] - 1));
						indices.push_back(static_cast<uint16_t>(uValue[6] - 1));
					}
				}
			}
		}
		else
		{
			while (!fs.eof())
			{
				//��ȡһ��
				std::getline(fs, line);

				//��������
				if (line[0] == 'v' && line[1] == ' ')
				{
					if (sscanf_s(line.c_str(), "v %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
					{
						auto& position = vertexDatas[vIndex++].PositionOS;
						position.x = fValue[0];
						position.y = fValue[1];
						position.z = fValue[2];
					}
				}
				else if (line[0] == 'v' && line[1] == 'n')
				{
					if (sscanf_s(line.c_str(), "vn %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
					{
						auto& normal = vertexDatas[vnIndex++].NormalOS;
						normal.x = fValue[0];
						normal.y = fValue[1];
						normal.z = fValue[2];
					}
				}
				else if (line[0] == 'v' && line[1] == 't')
				{
					if (sscanf_s(line.c_str(), "vt %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
					{
						auto& uv0 = vertexDatas[vtIndex++].UV0;
						uv0.x = fValue[0];
						uv0.y = fValue[1];
					}
				}
				else if (line[0] == 'f' && line[1] == ' ')
				{
					if (sscanf_s(line.c_str(), "f %u/%u/%u %u/%u/%u %u/%u/%u", uValue, uValue + 1, uValue + 2, uValue + 3, uValue + 4, uValue + 5, uValue + 6, uValue + 7, uValue + 8) == 9)
					{
						indices[iIndex] = static_cast<uint16_t>(uValue[0] - 1);
						indices[iIndex + 1] = static_cast<uint16_t>(uValue[3] - 1);
						indices[iIndex + 2] = static_cast<uint16_t>(uValue[6] - 1);
						iIndex += 3;
					}
				}
			}
		}

		//�ر��ļ�
		fs.close();
		
		auto* pMeshResource = new MeshResource();
		pMeshResource->SetVertexDatas(std::move(vertexDatas));
		pMeshResource->SetIndices(std::move(indices));
		return pMeshResource;
	}
}
