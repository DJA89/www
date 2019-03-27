#ifndef _SOUND_SYSTEM_H_
#define _SOUND_SYSTEM_H_

#include "inc/fmod_studio.hpp"
#include "inc/fmod.h"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

typedef FMOD::Sound* SoundClass;
using namespace std;

class SoundSystemClass {
public:
	SoundSystemClass();
	void createSound(SoundClass *pSound, const char* pFile);
	void playSound(SoundClass pSound, bool bLoop = false);
	void releaseSound(SoundClass pSound);
};

#endif
