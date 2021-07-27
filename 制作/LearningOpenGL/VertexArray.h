#pragma once
class VertexArray
{
public:
	VertexArray(const float* verts,unsigned int numVerts,const unsigned int* indices,unsigned int numIndices);
	~VertexArray();
	//�̒��_�z����A�N�e�B�u�ɂ���
	void SetActive();
	unsigned int GetNumIndices() const { return mNumIndices; }
	unsigned int GetNumVerts() const { return mNumVerts; }
private:
	//���_�o�b�t�@�ɂ��钸�_
	unsigned int mNumVerts;
	//�C���f�b�N�X�o�b�t�@�ɂ���C���f�b�N�X�̐�
	unsigned int mNumIndices;
	//���_�o�b�t�@OpenGL ID
	unsigned int mVertexBuffer;
	//�C���f�b�N�X�o�b�t�@OpenGL ID
	unsigned int mIndexBuffer;
	//���_�z��o�b�t�@OpenGL ID
	unsigned int mVertexArray;
};