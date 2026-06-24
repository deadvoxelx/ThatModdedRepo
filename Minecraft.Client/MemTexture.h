#pragma once
class BufferedImage;
class MemTextureProcessor;
using namespace std;

class MemTexture {
public:
	BufferedImage *loadedImage;
    int count;
    int id;
    bool isLoaded;
	int ticksSinceLastUse;
	static const int UNUSED_TICKS_TO_FREE = 200;

	MemTexture() = default;

    MemTexture(const wstring& _name, PBYTE pbData, DWORD dwBytes, MemTextureProcessor *processor);
	~MemTexture();
};
