#include "VertexArray.h"
#include <glad/glad.h>
VertexArray::VertexArray(const float* verts, unsigned int numVerts,
	const unsigned int* indices, unsigned int numIndices)
	:mNumVerts(numVerts),
	mNumIndices(numIndices)
{
	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexArray);
	glBufferData(GL_ARRAY_BUFFER, numVerts * 36 * sizeof(float), verts, GL_STATIC_DRAW);
	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices*sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//Position attribute
	glVertexAttribPointer(0,3, GL_FLOAT,GL_FALSE, 8*sizeof(float), 0);
	glEnableVertexAttribArray(0);
	//Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
VertexArray::~VertexArray()
{
	glDeleteBuffers(1, &mVertexBuffer);
	glDeleteBuffers(1, &mIndexBuffer);
	glDeleteVertexArrays(1,&mVertexArray);
}
void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}