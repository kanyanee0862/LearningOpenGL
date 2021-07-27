#pragma once
#include <string>
class Texture
{
public:
	Texture();
	~Texture();

	bool Load(const std::string& fileName);
	void Unload();

	void setActive();
	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
private:
	//テキスチャーのOpenGL ID
	unsigned int mTextureID;
	int mWidth;
	int mHeight;
};