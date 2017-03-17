#include "ClGameTime.h"

LARGE_INTEGER ClGameTime::frequence;
LARGE_INTEGER ClGameTime::temps; //Le tick actuel du CPU
float ClGameTime::elapsedTime = 0; //Le temps �coul� pour afficher le dernier Frame
float ClGameTime::currentFrame = 0; //Le temps que le Frame actuel a d�but�
float ClGameTime::lastFrame = 0; //Le temps du Frame pr�c�dent a d�but�


ClGameTime::ClGameTime()
{
}


ClGameTime::~ClGameTime() {
}

//Retourne le FPS
float ClGameTime::getFPS() {
	return m_FPS;
}

//Retourne le DeltaTime
float ClGameTime::getDeltaTime() {
	return elapsedTime;
}

//Initialise le compteur
void ClGameTime::initTime() {
	QueryPerformanceFrequency(&frequence); //Retourne le nombre de "tick" pour seconde
	QueryPerformanceCounter(&temps); //Prend le "tick" actuel
	lastFrame = temps.QuadPart; //On dit que lastFrame est �gal au "tick" actuel
}

//Update le compteur
void ClGameTime::updateTimer() {
	QueryPerformanceCounter(&temps); //Prend le "tick" actuel
	currentFrame = temps.QuadPart; //On garde le temps actuel
	elapsedTime = (currentFrame - lastFrame) / frequence.QuadPart; //Calcul diff�rence du temps entre nos 2 frames
	lastFrame = currentFrame; //On pr�pare notre variable lastFrame pour le prochain Frame.
}
