
//*****************************************************************************
//
// Image.cpp : Defines the class operations on images
//
// Author - Parag Havaldar
// Code used by students as starter code to display and modify images
//
//*****************************************************************************

#include "Image.h"
#include <math.h>
// Constructor and Desctructors
MyImage::MyImage() 
{
	Data = NULL;
	Width = -1;
	Height = -1;
	ImagePath[0] = 0;
}

MyImage::~MyImage()
{
	if ( Data )
		delete Data;
}


// Copy constructor
MyImage::MyImage( MyImage *otherImage)
{
	Height = otherImage->Height;
	Width  = otherImage->Width;
	Data   = new char[Width*Height*3];
	strcpy(otherImage->ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage->Data[i];
	}


}



// = operator overload
MyImage & MyImage::operator= (const MyImage &otherImage)
{
	Height = otherImage.Height;
	Width  = otherImage.Width;
	Data   = new char[Width*Height*3];
	strcpy( (char *)otherImage.ImagePath, ImagePath );

	for ( int i=0; i<(Height*Width*3); i++ )
	{
		Data[i]	= otherImage.Data[i];
	}
	
	return *this;

}


// MyImage::ReadImage
// Function to read the image given a path
bool MyImage::ReadImage()
{

	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		fprintf(stderr, "Usage is `Image.exe Imagefile w h`");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *IN_FILE;
	IN_FILE = fopen(ImagePath, "rb");
	if ( IN_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Reading");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Width*Height; i ++)
	{
		Rbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Gbuf[i] = fgetc(IN_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		Bbuf[i] = fgetc(IN_FILE);
	}
	
	// Allocate Data structure and copy
	Data = new char[Width*Height*3];
	for (i = 0; i < Height*Width; i++)
	{
		Data[3*i]	= Bbuf[i];
		Data[3*i+1]	= Gbuf[i];
		Data[3*i+2]	= Rbuf[i];
	}

	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(IN_FILE);

	return true;

}



// MyImage functions defined here
bool MyImage::WriteImage()
{
	// Verify ImagePath
	// Verify ImagePath
	if (ImagePath[0] == 0 || Width < 0 || Height < 0 )
	{
		fprintf(stderr, "Image or Image properties not defined");
		return false;
	}
	
	// Create a valid output file pointer
	FILE *OUT_FILE;
	OUT_FILE = fopen(ImagePath, "wb");
	if ( OUT_FILE == NULL ) 
	{
		fprintf(stderr, "Error Opening File for Writing");
		return false;
	}

	// Create and populate RGB buffers
	int i;
	char *Rbuf = new char[Height*Width]; 
	char *Gbuf = new char[Height*Width]; 
	char *Bbuf = new char[Height*Width]; 

	for (i = 0; i < Height*Width; i++)
	{
		Bbuf[i] = Data[3*i];
		Gbuf[i] = Data[3*i+1];
		Rbuf[i] = Data[3*i+2];
	}

	
	// Write data to file
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Rbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Gbuf[i], OUT_FILE);
	}
	for (i = 0; i < Width*Height; i ++)
	{
		fputc(Bbuf[i], OUT_FILE);
	}
	
	// Clean up and return
	delete Rbuf;
	delete Gbuf;
	delete Bbuf;
	fclose(OUT_FILE);

	return true;

}

unsigned int ui(char x) {
	unsigned char y = x;
	return (unsigned int)(y);
}

void filter(char* Data, int W, int H, int h1, int h2) {
	float r, g, b, h, v, xmax, xmin, c, s, f, p, q, t;
	int j;
	for (int i = 0; i < W*H*3; i+=3) {
		b = float(ui(Data[i]))/255;
		g = float(ui(Data[i + 1]))/255;
		r = float(ui(Data[i + 2]))/255;
		xmin = min(r, min(g, b));
		xmax = max(r, max(g, b));
		v = xmax;
		c = xmax - xmin;
		if(xmax==0){
			s = 0; h = -1;
			h += (h < 0) ? 360 : 0;
		}
		else {
			s = c / xmax;
			if (r == xmax) h = (g - b) / c;
			else if (g == xmax) h = 2 + (b - r) / c;
			else h = 4 + (r - g) / c;
			h *= 60;
			h += (h < 0) ? 360 : 0;
		}

		if (h>=h1 && h<=h2) {
			Data[i] = (unsigned int)(b * 255);
			Data[i + 1] = (unsigned int)(g * 255);
			Data[i + 2] = (unsigned int)(r * 255);
			continue;
		}
		if (s == 0) r = g = b = v;
		else {
			s = 0; h /= 60;
			j = floor(h);
			f = h - j;
			t = v * (1 - s * (1 - f));
			q = v * (1 - s * f);
			p = v * (1 - s);
			switch (j) {
				case 0: r = v; g = t; b = p; break;
				case 1:r = q; g = v; b = p; break;
				case 2:r = p; g = v; b = t; break;
				case 3:r = p; g = q; b = v; break;
				case 4:r = t; g = p; b = v; break;
				default:r = v; g = p; b = q; break;
			}
		}
		Data[i]   = (unsigned int)(b*255);
		Data[i+1] = (unsigned int)(g*255);
		Data[i+2] = (unsigned int)(r*255);
	}
}

bool MyImage::Modify(int h1, int h2)
{
	filter(Data, Width, Height, h1, h2);
	return true;
}