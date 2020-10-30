#include "ResourceLoader.h"

namespace SaplingEngine
{
	/**
	 * \brief 加载Mesh资源
	 * \param path Mesh资源路径
	 * \return MeshResource指针
	 */
	MeshResource* LoadMeshResource(const std::string& path)
	{
		//打开文件
		std::fstream fs;
		fs.open(path, std::ios::in);

		//检查文件打开是否成功
		if (!fs.is_open())
		{
			fs.close();
			std::cout << path << " open failed!" << std::endl;
			return nullptr;
		}

		//数据容器
		std::vector<Vector3> positions;
		std::vector<Vector3> normals;
		std::vector<Vector2> uvs;

		std::vector<VertexData> vertexDatas;
		std::vector<uint16_t> indices;

		//解析
		std::string line;
		float fValue[3];
		while (!fs.eof())
		{
			//读取一行
			std::getline(fs, line);

			//解析参数
			if (line[0] == 'v' && line[1] == ' ')
			{
				if (sscanf_s(line.c_str(), "v %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
				{
					positions.emplace_back(fValue[0], fValue[1], fValue[2]);
				}
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				if (sscanf_s(line.c_str(), "vn %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
				{
					normals.emplace_back(fValue[0], fValue[1], fValue[2]);
				}
			}
			else if (line[0] == 'v' && line[1] == 't')
			{
				if (sscanf_s(line.c_str(), "vt %f %f %f", fValue, fValue + 1, fValue + 2) == 3)
				{
					uvs.emplace_back(fValue[0], fValue[1]);
				}
			}
			else if (line[0] == 'f' && line[1] == ' ')
			{
				std::vector<int> pIndexes, nIndexes, uIndexes;

				auto lineSize = static_cast<int32_t>(line.size());
				if (line[lineSize - 1] != ' ')
				{
					line.append(" ");
					++lineSize;
				}

				std::string substr;
				for (auto i = 2; i < lineSize;)
				{
					auto pos = static_cast<int32_t>(line.find('/', i));
					if (pos == -1)
					{
						break;
					}
					substr = line.substr(i, pos - i);
					pIndexes.emplace_back(std::atoi(substr.c_str()) - 1);
					i = pos + 1;

					pos = static_cast<int32_t>(line.find('/', i));
					if (i != pos)
					{
						substr = line.substr(i, pos - i);
						uIndexes.emplace_back(std::atoi(substr.c_str()) - 1);
					}
					i = pos + 1;

					pos = static_cast<int32_t>(line.find(' ', i));
					substr = line.substr(i, pos - i);
					nIndexes.emplace_back(std::atoi(substr.c_str()) - 1);
					i = pos + 1;
				}

				const auto vCount = static_cast<int32_t>(vertexDatas.size());
				if (pIndexes.size() == 3)
				{
					VertexData vertexData0
					{
						positions[pIndexes[0]].value,
						normals[nIndexes[0]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					VertexData vertexData1
					{
						positions[pIndexes[1]].value,
						normals[nIndexes[1]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					VertexData vertexData2
					{
						positions[pIndexes[2]].value,
						normals[nIndexes[2]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					vertexDatas.push_back(vertexData0);
					vertexDatas.push_back(vertexData1);
					vertexDatas.push_back(vertexData2);
					indices.push_back(vCount + 0);
					indices.push_back(vCount + 3);
					indices.push_back(vCount + 1);
				}
				else
				{
					VertexData vertexData0
					{
						positions[pIndexes[0]].value,
						normals[nIndexes[0]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					VertexData vertexData1
					{
						positions[pIndexes[1]].value,
						normals[nIndexes[1]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					VertexData vertexData2
					{
						positions[pIndexes[2]].value,
						normals[nIndexes[2]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					VertexData vertexData3
					{
						positions[pIndexes[3]].value,
						normals[nIndexes[3]].value,
						XMFLOAT4(Colors::Green),
						XMFLOAT2(0, 0),
						XMFLOAT2(0, 0),
					};

					vertexDatas.push_back(vertexData0);
					vertexDatas.push_back(vertexData1);
					vertexDatas.push_back(vertexData2);
					vertexDatas.push_back(vertexData3);
					indices.push_back(vCount + 0);
					indices.push_back(vCount + 3);
					indices.push_back(vCount + 1);
					indices.push_back(vCount + 3);
					indices.push_back(vCount + 2);
					indices.push_back(vCount + 1);
				}
			}
		}

		//关闭文件
		fs.close();

		auto* pMeshResource = new MeshResource();
		pMeshResource->SetVertexDatas(std::move(vertexDatas));
		pMeshResource->SetIndices(std::move(indices));
		return pMeshResource;
	}
}
