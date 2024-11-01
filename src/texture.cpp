#include "texture.hpp"

#include <wincodec.h>
#include "glext.hpp"

static wchar_t* toWideString(const char *string)
{
	if (!string)
		return nullptr;

	int bufferSize = MultiByteToWideChar(CP_UTF8, 0, string, -1, nullptr, 0);
	if (bufferSize == 0)
		return nullptr;

	wchar_t *wideString = new wchar_t[bufferSize];
	MultiByteToWideChar(CP_UTF8, 0, string, -1, wideString, bufferSize);
	return wideString;
}

Texture::Texture(const char *fileName)
{
	IWICImagingFactory *wicFactory = nullptr;
	CoCreateInstance
	(
		CLSID_WICImagingFactory,
		nullptr,
		CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory,
		(LPVOID*)&wicFactory
	);

	wchar_t *w_fileName = toWideString(fileName);
	IWICBitmapDecoder *wicDecoder = nullptr;
	wicFactory->CreateDecoderFromFilename
	(
		w_fileName,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&wicDecoder
	);

	IWICBitmapFrameDecode *wicFrame = nullptr;
	wicDecoder->GetFrame(0, &wicFrame);

	IWICFormatConverter *wicConverter = nullptr;
	wicFactory->CreateFormatConverter(&wicConverter);

	wicConverter->Initialize
	(
		wicFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	wicConverter->GetSize(&width, &height);

	unsigned int bufferSize = width * height * 4;
	unsigned char *buffer = new unsigned char[bufferSize];

	wicConverter->CopyPixels(
		nullptr,
		width * 4,
		bufferSize,
		buffer
	);

	glCreateTextures(GL_TEXTURE_2D, 1, &id);
	glTextureStorage2D(id, 1, GL_RGBA8, width, height);
	glTextureSubImage2D(id, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

	glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	delete[] w_fileName;
	delete[] buffer;
	wicConverter->Release();
	wicFrame->Release();
	wicDecoder->Release();
	wicFactory->Release();
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}
