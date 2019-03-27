// pch.cpp: source file corresponding to pre-compiled header; necessary for compilation to succeed

#include "SoundSystem.h"

#include "inc/fmod_studio.hpp"
#include "inc/fmod.h"
	// Pointer to the FMOD instance

FMOD::System *m_pSystem;

SoundSystemClass::SoundSystemClass() {
	if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
	{
		// Report Error
		cout << "esto no funco";
		return;
	}

	int driverCount = 0;
	m_pSystem->getNumDrivers(&driverCount);

	if (driverCount == 0)
	{
		// Report Error
		cout << "esto no funco2";
		return;
	}

	// Initialize our Instance with 36 Channels
	m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);
}

void SoundSystemClass::createSound(SoundClass *pSound, const char* pFile)
{
	cout << pFile << "\n";
	m_pSystem->createSound(pFile, NULL, 0, pSound);
}

void SoundSystemClass::playSound(SoundClass pSound, bool bLoop)
{
	if (!bLoop)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	m_pSystem->playSound(pSound, NULL, false, 0);
}

void SoundSystemClass::releaseSound(SoundClass pSound)
{
	pSound->release();
}
