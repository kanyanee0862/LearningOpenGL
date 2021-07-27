#pragma once
class VertexArray
{
public:
	VertexArray(const float* verts,unsigned int numVerts,const unsigned int* indices,unsigned int numIndices);
	~VertexArray();
	//個の頂点配列をアクティブにする
	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	//頂点バッファにある頂点
	unsigned int mNumVerts;
	//インデックスバッファにあるインデックスの数
	unsigned int mNumIndices;
	//頂点バッファOpenGL ID
	unsigned int mVertexBuffer;
	//インデックスバッファOpenGL ID
	unsigned int mIndexBuffer;
	//頂点配列バッファOpenGL ID
	unsigned int mVertexArray;
};