#include "Texture.h"
#include <iostream>
#include <glad/glad.h>


Texture::Texture()
	:mTextureID(0),
	mHeight(0),
	mWidth(0)
{
}

Texture::~Texture()
{
}

bool Texture::Load(const std::string & fileName)
{
	//load and generate texture
	int channels;
	//unsigned char* image = stbi_load(fileName.c_str(), &mWidth, &mHeight, &channels, 0);
	//if (!image)
	{
		std::cout << "Failed to load texture" << std::endl;
		//stbi_image_free(image);
		return false;
		
	}
	GLenum format;
	if (channels == 1)
		format = GL_RED;
	else if (channels == 3)
		format = GL_RGB;
	else if (channels == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, mTextureID);
	//glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	//set the texture wrapping/filtering options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//stbi_image_free(image);
	return true;
	
}

void Texture::Unload()
{
	glDeleteTextures(1,&mTextureID);
}

void Texture::setActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}
