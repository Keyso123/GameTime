//***************************************************************************
// File:		
// Description:	
// Author:	Desgraff (C) homework (12/09/2005+) (Do not share, ask desgraff)
//***************************************************************************
#pragma once

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>
#include <string>

using namespace std;
const int FILTER_NEAREST = 0;
const int FILTER_LINEAR = 1;
const int FILTER_BILINEAR = 2;
const int FILTER_TRILINEAR = 3;





class Texture{
public:
	// Le filename
	string filename;

	// Le ID de la texture
	unsigned int textureID;

	// Son width et son height et son BBP
	unsigned int width;
	unsigned int height;
	unsigned int bpp;

public:
	// Constructor
	Texture::Texture() {
		textureID = 0;
	}


	Texture(string & _filename){

		filename = _filename;

		// On cré notre texture
		textureID = createTextureFromFile(filename);

		setFilter(FILTER_BILINEAR);
	}

	Texture(int size){

		width = size;
		height = size;
		bpp = 3;

		// On va automatiquement créer une texture vide
		unsigned char *imageData = new unsigned char[width*height*bpp];

		int totalSize = width*height*bpp;
		for (int i=0;i<totalSize;i++){

			imageData[i] = rand()%255;
		}

		// On génère une texture
		glGenTextures(1, &textureID);

		// On bind cette texture au context
		glBindTexture(GL_TEXTURE_2D, textureID);

		// On construit les mipmaps
		gluBuild2DMipmaps(GL_TEXTURE_2D, bpp, width, height,
						  GL_RGB, GL_UNSIGNED_BYTE, imageData);

		delete [] imageData;
	}

	// Destructeur
	virtual ~Texture(){
		glDeleteTextures(1, &textureID);
	}

	bool CheckExtension(const char* p_filename, const char* p_ext);

	unsigned int Width() { return width; }
	unsigned int Height() { return height; }

	// Pour loader un fichier
	unsigned int createTextureFromFile(string & filename);
	unsigned int Texture::loadTexture(string filename);

	// Pour l'utiliser (la binder)
	void useTexture() {glBindTexture(GL_TEXTURE_2D, textureID);}

	// Pour changer le filter
	void setFilter(int filter);

	unsigned int Texture::getID() {
		return textureID;
	}
};
