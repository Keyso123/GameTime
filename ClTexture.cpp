//***************************************************************************
// File:		
// Description:	
// Author:		Desgraff (C) homework (12/09/2005+) (Do not share, ask desgraff)
//***************************************************************************

#include <vector>
#include "Targa.h"


bool Texture::CheckExtension(const char* p_filename, const char* p_ext) {
	unsigned int extLen = (unsigned int)strlen(p_ext);

	if(p_filename[(unsigned int)strlen(p_filename) - extLen -1] != '.')
		return false;

	return (_stricmp(&p_filename[(unsigned int)strlen(p_filename) - extLen],p_ext) == 0);
}

unsigned int Texture::createTextureFromFile(string & filename){

	// Notre texture ID de ogl
	unsigned int Texture = 0;
	unsigned char *imageData;
	GLint Level;

	// On check si c'est un TGA ou un BMP
	if (CheckExtension(filename.c_str(), "tga"))
	{
		// Les variables utilisé pour tenir l'information loadé du Targa
		unsigned char TGAcompare[12];
		unsigned char header[6];
		unsigned int imageSize;
		unsigned char temp;

		// On ouvre le fichier targa
		FILE *file = fopen(filename.c_str(), "rb");

		// Si ça marche pas, oups, on returne 0 comme texture.
		if (file == NULL) 
		{
			return 0;
		}

		// On li le header du fichier (12 premiers byte)
		fread(TGAcompare,1,sizeof(TGAcompare),file);

		// On li la suite du header
		fread(header,1,sizeof(header),file);

		// On prend le width et le height du header
		this->width  = header[1] * 256 + header[0];
		this->height = header[3] * 256 + header[2];

		// On prend le bbp (24bit ou 32bit)
		this->bpp	= header[4];

		// On le divise par 8 pour avoir 3 ou 4 bytes (RGB ou RGBA)
		this->bpp	= this->bpp/8;

		// On calcul la grandeur de l'image en byte
		imageSize		= this->width * this->height * this->bpp;

		// On alou alors autant de bytes qu'il faut pour tenir l'image
		imageData = new unsigned char [imageSize];

		// On li maintenant le gros bloc de données
		fread(imageData, 1, imageSize, file);

		// On défini si c'est RGB ou RGBA
		Level = (this->bpp == 3) ? GL_RGB : GL_RGBA;

		// Ici c'est con, mais faut switcher le rouge avec le bleu
		for(unsigned int i=0; i<imageSize; i+=this->bpp){

			temp=imageData[i];
			imageData[i] = imageData[i + 2];
			imageData[i + 2] = temp;
		}

		// On ferme maintenant le fichier
		fclose (file);

		// On génère une texture
		glGenTextures(1, &Texture);

		// On bind cette texture au context
		glBindTexture(GL_TEXTURE_2D, Texture);

		// On construit les mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, this->bpp, this->width, this->height,
						  Level, GL_UNSIGNED_BYTE, imageData);

	}else if (CheckExtension(filename.c_str(), "bmp")){

		//
		// bon, ici c'est un peu le foutoire là, c'est une fonction que j'ai pris dans
		// l'un de mes vieux engine (J'avais pris la fonction sur internet)
		//
		FILE *file = fopen(filename.c_str(), "rb");

		// Si ça marche pas, oups, on returne 0 comme texture.
		if (file == NULL) 
		{
			return 0;
		}

		// Loader le fichier BMP et la foutre dans la texture
			long imageSize;
			GLubyte TMP;

			//The BITMAPFILEHEADER:
			int i;
			short	bfType[1];
			long	bfSize[1];
			short	bfReserved1[1];
			short	bfReserved2[1];
			long	bfOffBits[1];

			//The BITMAPINFOHEADER:
			 long biSize[1];// 40 specifies the size of the BITMAPINFOHEADER structure, in bytes. 
			 long biWidth[1];//  100 specifies the width of the image, in pixels. 
			 long biHeight[1];//  100 specifies the height of the image, in pixels. 
			 short biPlanes[1];//  1 specifies the number of planes of the target device, must be set to zero. 
			 short biBitCount[1];//  8 specifies the number of bits per pixel. 
			 long biCompression[1];//  0 Specifies the type of compression, usually set to zero (no compression). 
			 long biSizeImage[1];//  0 specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero. 
			 long biXPelsPerMeter[1];//  0 specifies the the horizontal pixels per meter on the designated targer device, usually set to zero. 
			 long biYPelsPerMeter[1];//  0 specifies the the vertical pixels per meter on the designated targer device, usually set to zero. 
			 long biClrUsed[1];//  0 specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member. 
			 long biClrImportant[1];//  0 specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.

			// Load all the shit
				fread(bfType,2,1,file);
				fread(bfSize,4,1,file);
				fread(bfReserved1,2,1,file);
				fread(bfReserved2,2,1,file);
				fread(bfOffBits,4,1,file);

				fread(biSize,4,1,file);
				fread(biWidth,4,1,file);
				fread(biHeight,4,1,file);
				fread(biPlanes,2,1,file);
				fread(biBitCount,2,1,file);
			//	biBitCount[0] = 24;
				biBitCount[0] /= 8;
				fread(biCompression,4,1,file);
				fread(biSizeImage,4,1,file);
				fread(biXPelsPerMeter,4,1,file);
				fread(biYPelsPerMeter,4,1,file);
				fread(biClrUsed,4,1,file);
				fread(biClrImportant,4,1,file);

			// Alloc
			imageSize = biWidth[0]*biHeight[0]*biBitCount[0];
			imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

			fread(imageData, 1, imageSize, file);

			fclose(file);

			// Invert the R and B
				for (i=0;i<imageSize;i+=biBitCount[0])
				{
					TMP = imageData[i];
					imageData[i] = imageData[i+2];
					imageData[i+2] = TMP;
				}

		// On génère une texture
		glGenTextures(1, &Texture);

		// On bind cette texture au context
		glBindTexture(GL_TEXTURE_2D, Texture);

		this->bpp = biBitCount[0];
		this->width = biWidth[0];
		this->height = biHeight[0];

		// On défini si c'est RGB ou RGBA
		Level = (this->bpp == 3) ? GL_RGB : GL_RGBA;

		// On construit les mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, this->bpp, this->width, this->height,
						  Level, GL_UNSIGNED_BYTE, imageData);

	}

	delete [] imageData;

	// On retourne l'index de la texture
	return Texture;
}



void Texture::setFilter(int filter){

	useTexture();

	switch (filter)
	{
	case FILTER_NEAREST: // Nearest
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			break;
		}
	case FILTER_LINEAR: // Linear
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			break;
		}
	case FILTER_BILINEAR: // Bilinear
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			break;
		}
	case FILTER_TRILINEAR: // Trilinear
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
	default: // Nearest default
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
			break;
		}
	}
}


unsigned int Texture::loadTexture(string filename) {

	//TO DO : ajouter validation si la texture a des grandeurs au carré

	// Notre texture ID de ogl
	unsigned char *imageData = 0;
	GLint Level;

	// On check si c'est un TGA ou un BMP
	if (CheckExtension(filename.c_str(), "tga")){
		// Les variables utilisé pour tenir l'information loadé du Targa
		unsigned char TGAcompare[12];
		unsigned char header[6];
		unsigned int imageSize;
		unsigned char temp;

		// On ouvre le fichier targa
		FILE *file = fopen(filename.c_str(), "rb");

		// Si ça marche pas, oups, on returne 0 comme texture.
		if (file == NULL){

			return 0;
		}

		// On li le header du fichier (12 premiers byte)
		fread(TGAcompare,1,sizeof(TGAcompare),file);

		// On li la suite du header
		fread(header,1,sizeof(header),file);

		// On prend le width et le height du header
		this->width  = header[1] * 256 + header[0];
		this->height = header[3] * 256 + header[2];

		// On prend le bbp (24bit ou 32bit)
		this->bpp	= header[4];

		// On le divise par 8 pour avoir 3 ou 4 bytes (RGB ou RGBA)
		this->bpp	= this->bpp/8;

		// On calcul la grandeur de l'image en byte
		imageSize = this->width * this->height * this->bpp;

		// On alou alors autant de bytes qu'il faut pour tenir l'image
		imageData = new unsigned char [imageSize];

		// On li maintenant le gros bloc de données
		fread(imageData, 1, imageSize, file);

		// On défini si c'est RGB ou RGBA
		Level = (this->bpp == 3) ? GL_RGB : GL_RGBA;

		// Ici c'est con, mais faut switcher le rouge avec le bleu
		for(unsigned int i=0; i<imageSize; i+=this->bpp){

			temp=imageData[i];
			imageData[i] = imageData[i + 2];
			imageData[i + 2] = temp;
		}

		// On ferme maintenant le fichier
		fclose (file);

		// On génère une texture
		glGenTextures(1, &textureID);

		// On bind cette texture au context
		glBindTexture(GL_TEXTURE_2D, textureID);

		// On construit les mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, this->bpp, this->width, this->height,
						  Level, GL_UNSIGNED_BYTE, imageData);

	}else if (CheckExtension(filename.c_str(), "bmp")){

		//
		// bon, ici c'est un peu le foutoire là, c'est une fonction que j'ai pris dans
		// l'un de mes vieux engine (J'avais pris la fonction sur internet)
		//
		FILE *file = fopen(filename.c_str(), "rb");

		// Si ça marche pas, oups, on returne 0 comme texture.
		if (file == NULL){
			return 0;
		}

		// Loader le fichier BMP et la foutre dans la texture
			long imageSize;
			GLubyte TMP;

			//The BITMAPFILEHEADER:
			int i;
			short	bfType[1];
			long	bfSize[1];
			short	bfReserved1[1];
			short	bfReserved2[1];
			long	bfOffBits[1];

			//The BITMAPINFOHEADER:
			 long biSize[1];// 40 specifies the size of the BITMAPINFOHEADER structure, in bytes. 
			 long biWidth[1];//  100 specifies the width of the image, in pixels. 
			 long biHeight[1];//  100 specifies the height of the image, in pixels. 
			 short biPlanes[1];//  1 specifies the number of planes of the target device, must be set to zero. 
			 short biBitCount[1];//  8 specifies the number of bits per pixel. 
			 long biCompression[1];//  0 Specifies the type of compression, usually set to zero (no compression). 
			 long biSizeImage[1];//  0 specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero. 
			 long biXPelsPerMeter[1];//  0 specifies the the horizontal pixels per meter on the designated targer device, usually set to zero. 
			 long biYPelsPerMeter[1];//  0 specifies the the vertical pixels per meter on the designated targer device, usually set to zero. 
			 long biClrUsed[1];//  0 specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member. 
			 long biClrImportant[1];//  0 specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.

			// Load all the info
				fread(bfType,2,1,file);
				fread(bfSize,4,1,file);
				fread(bfReserved1,2,1,file);
				fread(bfReserved2,2,1,file);
				fread(bfOffBits,4,1,file);

				fread(biSize,4,1,file);
				fread(biWidth,4,1,file);
				fread(biHeight,4,1,file);
				fread(biPlanes,2,1,file);
				fread(biBitCount,2,1,file);
			//	biBitCount[0] = 24;
				biBitCount[0] /= 8;
				fread(biCompression,4,1,file);
				fread(biSizeImage,4,1,file);
				fread(biXPelsPerMeter,4,1,file);
				fread(biYPelsPerMeter,4,1,file);
				fread(biClrUsed,4,1,file);
				fread(biClrImportant,4,1,file);

			// Alloc
			imageSize = biWidth[0]*biHeight[0]*biBitCount[0];
			imageData=(GLubyte *)malloc(imageSize);			// Reserve Memory To Hold The TGA Data

			fread(imageData, 1, imageSize, file);

			fclose(file);

			// Invert the R and B
				for (i=0;i<imageSize;i+=biBitCount[0]){
					TMP = imageData[i];
					imageData[i] = imageData[i+2];
					imageData[i+2] = TMP;
				}

		// On génère une texture
		glGenTextures(1, &textureID);

		// On bind cette texture au context
		glBindTexture(GL_TEXTURE_2D, textureID);

		this->bpp = biBitCount[0];
		this->width = biWidth[0];
		this->height = biHeight[0];

		// On défini si c'est RGB ou RGBA
		Level = (this->bpp == 3) ? GL_RGB : GL_RGBA;

		// On construit les mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, this->bpp, this->width, this->height, Level, GL_UNSIGNED_BYTE, imageData);
	}

	if(imageData) { delete[] (imageData);   (imageData)=0; }

	// On retourne l'index de la texture
	return textureID;
}
