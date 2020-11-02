using System.Collections.Generic;
using System.IO;
using System.Text;
using UnityEditor;
using UnityEngine;

namespace Chronicle
{
    /// <summary>
    /// 合并mesh
    /// </summary>
    public class CombineMeshEditor
    {
        [MenuItem("GameObject/CombineMesh", false, 10)]
        public static void CombineMesh()
        {
            combineMesh(Selection.activeGameObject);
        }

        [MenuItem("GameObject/CombineMeshNoUV", false, 10)]
        public static void CombineMeshNoUV()
        {
            combineMeshNoUV(Selection.activeGameObject);
        }

        /// <summary>
        /// 合并子节点Mesh
        /// </summary>
        /// <param name="gameObj">要合并mesh的对象</param>
        private static void combineMesh(GameObject gameObj)
        {
            StringBuilder vertexBuilder = new StringBuilder();
            StringBuilder uvBuilder = new StringBuilder();
            StringBuilder indexBuilder = new StringBuilder();

            //mesh列表
            int row = Mathf.CeilToInt(Mathf.Sqrt(gameObj.transform.childCount));
            for (int index = 0, lastVertexCount = 1; index < gameObj.transform.childCount; ++index)
            {
                var go = gameObj.transform.GetChild(index);
                var meshFilter = go.GetComponent<MeshFilter>();

				//写入顶点信息
				foreach (var vertex in meshFilter.sharedMesh.vertices)
				{
                    var worldPos = go.transform.localToWorldMatrix.MultiplyPoint(vertex);
                    var realPos = worldPos;//gameObj.transform.worldToLocalMatrix.MultiplyPoint(worldPos);
					vertexBuilder.Append(string.Format("v {0} {1} {2}\n", realPos.x, realPos.y, realPos.z));
				}

                //写入uv信息
                foreach (var uv in meshFilter.sharedMesh.uv)
				{
				    float offsetX = index % row / (float)row;
				    float offsetY = index / row / (float)row;
					uvBuilder.Append(string.Format("vt {0} {1}\n", offsetX + uv.x / (float)row, offsetY + uv.y / (float)row));
				}

                //写入发现信息
                foreach (var normal in meshFilter.sharedMesh.normals)
                {
                    var worldNormal = go.transform.localToWorldMatrix.MultiplyVector(normal);
                    var realNormal = worldNormal;//gameObj.transform.worldToLocalMatrix.MultiplyVector(worldNormal);
                    vertexBuilder.Append(string.Format("vn {0} {1} {2}\n", realNormal.x, realNormal.y, realNormal.z));
                }

                //写入顶点与uv的索引关系
                for (int i = 0; i < meshFilter.sharedMesh.subMeshCount; ++i)
                {
                    var triangles = meshFilter.sharedMesh.GetTriangles(i);
					for (int k = 0; k < triangles.Length; k += 3)
					{
						indexBuilder.Append(string.Format("f {0}/{1}/{2} {3}/{4}/{5} {6}/{7}/{8}\n",
                            triangles[k + 0] + lastVertexCount, triangles[k + 0] + lastVertexCount, triangles[k + 0] + lastVertexCount,
                            triangles[k + 1] + lastVertexCount, triangles[k + 1] + lastVertexCount, triangles[k + 1] + lastVertexCount,
                            triangles[k + 2] + lastVertexCount, triangles[k + 2] + lastVertexCount, triangles[k + 2] + lastVertexCount));
					}
                }

                lastVertexCount += meshFilter.sharedMesh.vertexCount;
			}

            //写入文件
            string path = "art/combinedobj/";
			if (!Directory.Exists(path))
			{
				Directory.CreateDirectory(path);
			}
			var stream = new FileStream(path + gameObj.name + ".obj", FileMode.Create);
			var writer = new StreamWriter(stream);
            writer.Write(vertexBuilder.ToString());
            writer.Write(uvBuilder.ToString());
            writer.Write(indexBuilder.ToString());
			writer.Close();
            writer.Dispose();
            stream.Close();
            stream.Dispose();
        }

        /// <summary>
        /// 合并子节点Mesh
        /// </summary>
        /// <param name="gameObj">要合并mesh的对象</param>
        private static void combineMeshNoUV(GameObject gameObj)
        {
            StringBuilder vertexBuilder = new StringBuilder();
            StringBuilder uvBuilder = new StringBuilder();
            StringBuilder indexBuilder = new StringBuilder();

            //mesh列表
            int row = Mathf.CeilToInt(Mathf.Sqrt(gameObj.transform.childCount));
            for (int index = 0, lastVertexCount = 1; index < gameObj.transform.childCount; ++index)
            {
                var go = gameObj.transform.GetChild(index);
                var meshFilter = go.GetComponent<MeshFilter>();

                //写入顶点信息
                foreach (var vertex in meshFilter.sharedMesh.vertices)
                {
                    var worldPos = go.transform.localToWorldMatrix.MultiplyPoint(vertex);
                    var realPos = gameObj.transform.worldToLocalMatrix.MultiplyPoint(worldPos);
                    vertexBuilder.Append(string.Format("v {0} {1} {2}\n", realPos.x, realPos.y, realPos.z));
                }

                //写入uv信息
                foreach (var uv in meshFilter.sharedMesh.uv)
                {
                    uvBuilder.Append(string.Format("vt {0} {1}\n", uv.x, uv.y));
                }

                //写入发现信息
                foreach (var normal in meshFilter.sharedMesh.normals)
                {
                    var worldNormal = go.transform.localToWorldMatrix.MultiplyVector(normal);
                    var realNormal = gameObj.transform.worldToLocalMatrix.MultiplyVector(worldNormal);
                    vertexBuilder.Append(string.Format("vn {0} {1} {2}\n", realNormal.x, realNormal.y, realNormal.z));
                }

                //写入顶点与uv的索引关系
                for (int i = 0; i < meshFilter.sharedMesh.subMeshCount; ++i)
                {
                    var triangles = meshFilter.sharedMesh.GetTriangles(i);
                    for (int k = 0; k < triangles.Length; k += 3)
                    {
                        indexBuilder.Append(string.Format("f {0}/{1}/{2} {3}/{4}/{5} {6}/{7}/{8}\n",
                            triangles[k + 0] + lastVertexCount, triangles[k + 0] + lastVertexCount, triangles[k + 0] + lastVertexCount,
                            triangles[k + 1] + lastVertexCount, triangles[k + 1] + lastVertexCount, triangles[k + 1] + lastVertexCount,
                            triangles[k + 2] + lastVertexCount, triangles[k + 2] + lastVertexCount, triangles[k + 2] + lastVertexCount));
                    }
                }

                lastVertexCount += meshFilter.sharedMesh.vertexCount;
            }

            //写入文件
            string path = "art/combinedobj/";
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
            var stream = new FileStream(path + gameObj.name + ".obj", FileMode.Create);
            var writer = new StreamWriter(stream);
            writer.Write(vertexBuilder.ToString());
            writer.Write(uvBuilder.ToString());
            writer.Write(indexBuilder.ToString());
            writer.Close();
            writer.Dispose();
            stream.Close();
            stream.Dispose();
        }
    }
}