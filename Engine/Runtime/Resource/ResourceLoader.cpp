#include <chrono>

#include "Render/Graphics/Mesh.h"
#include "Render/Graphics/Texture2D.h"
#include "Resource/ResourceLoader.h"

namespace SaplingEngine
{
	/**
	 * \brief	����Mesh��Դ
	 * \param	path		Mesh��Դ·��
	 * \return	MeshAssetָ��
	 */
	MeshAsset* LoadMeshAsset(const std::string& path)
	{
		//���ļ�
		std::fstream fs;
		fs.open(path, std::ios::in);

		//����ļ����Ƿ�ɹ�
		if (!fs.is_open())
		{
			fs.close();
			return nullptr;
		}

		//�������ݺ���������
		std::vector<VertexData> vertexDatas;
		std::vector<uint16_t> indices;

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
					if (sscanf_s(line.c_str(), "vt %f %f", fValue, fValue + 1) == 2)
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
					if (sscanf_s(line.c_str(), "vt %f %f", fValue, fValue + 1) == 2)
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
		
		auto* pMeshAsset = new MeshAsset();
		pMeshAsset->vertexDatas = std::move(vertexDatas);
		pMeshAsset->indices = std::move(indices);
		return pMeshAsset;
	}
	
	/**
	 * \brief	����Texture2D
	 * \param	path		Texture2D��Դ·��
	 * \return	Texture2D����ָ��
	 */
	Texture2DSptr LoadTexture2D(const std::string& path)
	{
		auto texture2D = MakeShared<Texture2D>(path);
		return texture2D;
	}
}
