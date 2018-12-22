#include "stdafx.h"
#include "pngLoader.h"
#include <png.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#define PNGSIGSIZE  8

#define RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
		(unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
		((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
		((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
		((unsigned)(unsigned char)(va) << 24))

using tImageSource =
struct
{
	unsigned char* data;
	png_size_t size;
	png_size_t offset;
};

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
	tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

	if ((int)(isource->offset + length) <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}

bool loadPng(const char* path, PngTexData& result)
{
	FILE* file;
	fopen_s(&file, path, "rb+");

	int size = 0;
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fseek(file, 0, SEEK_SET);
	}

	std::vector< unsigned char > pngBuf;
	pngBuf.resize(size);

	unsigned char* dataPointer = &pngBuf[0];
	size_t totalBytesRead = 0;
	while (!feof(file))
	{
		size_t bytesRead = fread(dataPointer, sizeof(unsigned char), 1024, file);
		totalBytesRead += bytesRead;
		dataPointer += bytesRead;
	}

	fclose(file);
	
	
	//std::string pngBuf;
	//std::ifstream ifsV(path);
	//pngBuf.assign((std::istreambuf_iterator<char>(ifsV)),
	//	(std::istreambuf_iterator<char>()));

	bool bRet = false;
	png_byte        header[PNGSIGSIZE] = { 0 };
	png_structp     png_ptr = 0;
	png_infop       info_ptr = 0;

	do
	{
		// png header len is 8 bytes
		if (pngBuf.size() < PNGSIGSIZE)
		{
			std::cout << "Corrupted PNG!" << '\n';
			break;
		}

		// check the data is png or not
		memcpy(header, &pngBuf[0], PNGSIGSIZE);
		if (png_sig_cmp(header, 0, PNGSIGSIZE))
		{
			std::cout << "This texture is not actually PNG!" << '\n';
			break;
		}

		// init png_struct
		png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
		if (!png_ptr) break;

		// init png_info
		info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr) break;

		if (setjmp(png_jmpbuf(png_ptr))) break;

		// set the read call back function
		tImageSource imageSource;
		imageSource.data = &pngBuf[0];
		imageSource.size = pngBuf.size();
		imageSource.offset = 0;
		png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

		// read png header info

		// read png file info
		png_read_info(png_ptr, info_ptr);

		unsigned int width = png_get_image_width(png_ptr, info_ptr);
		unsigned int height = png_get_image_height(png_ptr, info_ptr);
		unsigned int bpp = png_get_bit_depth(png_ptr, info_ptr);
		png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);
		bool hasAlpha = false;
		//bool preMulti = false;

		//CCLOG("color type %u", color_type);

		// force palette images to be expanded to 24-bit RGB
		// it may include alpha channel
		if (color_type == PNG_COLOR_TYPE_PALETTE)
		{
			png_set_palette_to_rgb(png_ptr);
		}
		// low-bit-depth grayscale images are to be expanded to 8 bits
		if (color_type == PNG_COLOR_TYPE_GRAY && bpp < 8)
		{
			png_set_expand_gray_1_2_4_to_8(png_ptr);
		}
		// expand any tRNS chunk data into a full alpha channel
		if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		{
			png_set_tRNS_to_alpha(png_ptr);
		}
		// reduce images with 16-bit samples to 8 bits
		if (bpp == 16)
		{
			png_set_strip_16(png_ptr);
		}
		// expand grayscale images to RGB
		if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		{
			png_set_gray_to_rgb(png_ptr);
		}

		// read png data
		// m_nBitsPerComponent will always be 8
		//bpp = 8;
		png_size_t rowbytes;
		png_bytep* row_pointers = (png_bytep*)malloc(sizeof(png_bytep) *height);

		png_read_update_info(png_ptr, info_ptr);

		rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		result.width = width;
		result.height = height;
		result.data.reset(new char[rowbytes * height]);
			
		for (unsigned short i = 0; i < height; ++i)
		{
			row_pointers[i] = (unsigned char*)result.data.get() + i * rowbytes;
		}
		png_read_image(png_ptr, row_pointers);

		png_read_end(png_ptr, NULL);

		png_size_t channel = rowbytes / width;
		if (channel == 4)
		{

			hasAlpha = true;
			/*
			unsigned int *tmp = (unsigned int *)data;
			for(unsigned short i = 0; i <height; i++)
			{
			for(unsigned int j = 0; j < rowbytes; j += 4)
			{
			*tmp++ = RGB_PREMULTIPLY_ALPHA( row_pointers[i][j], row_pointers[i][j + 1],
			row_pointers[i][j + 2], row_pointers[i][j + 3] );
			}
			}

			preMulti = true;
			*/
		}

		if (row_pointers)free(row_pointers);

		bRet = true;

		result.alphaChannel = false;
		switch (channel)
		{
		case 4: result.alphaChannel = true; break;
		}

	} while (0);

	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}

	return true;
}
