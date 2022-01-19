//================================================================================
// Copyright (c) 2013 ~ 2022. HyungKi Jeong(clonextop@gmail.com)
// Freely available under the terms of the 3-Clause BSD License
// (https://opensource.org/licenses/BSD-3-Clause)
// 
// Redistribution and use in source and binary forms,
// with or without modification, are permitted provided
// that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
// GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
// OF SUCH DAMAGE.
// 
// Title : Template design
// Rev.  : 1/19/2022 Wed (clonextop@gmail.com)
//================================================================================
#include "DPI_Sound.h"
#include <math.h>

DPI_Sound		g_Sound;

DPI_Sound::DPI_Sound(void)
{
	m_pDevice	= NULL;
	m_pContext	= NULL;
}

DPI_Sound::~DPI_Sound(void)
{
	ReleaseAll();
}

bool DPI_Sound::Initialize(void)
{
	m_pDevice	= alcOpenDevice(NULL);
	ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");

	if(enumeration == AL_FALSE) {
		return false;
	}

	m_pContext = alcCreateContext(m_pDevice, NULL);

	if(!alcMakeContextCurrent(m_pContext)) {
		return false;
	}

	{
		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
		alListenerfv(AL_ORIENTATION, listenerOri);
		alListener3f(AL_POSITION, 0, 0, 0);
		alListener3f(AL_VELOCITY, 0, 0, 0);
	}

	return true;
}

void DPI_Sound::ReleaseAll(void)
{
	while(DPI_SoundSource::Head()) delete DPI_SoundSource::Head();

	if(m_pDevice) {
		if(m_pContext) {
			alcMakeContextCurrent(NULL);
			alcDestroyContext(m_pContext);
			m_pContext	= NULL;
		}

		alcCloseDevice(m_pDevice);
		m_pDevice	= NULL;
	}
}

DPI_SoundSource* DPI_Sound::CreateSource(SOUND_BUFFER* pBuffer)
{
	DPI_SoundSource*	pSource	= new DPI_SoundSource(pBuffer);
	return pSource;
}

void DPI_SoundSource::Free(void)
{
	if(m_iSource != (ALuint) - 1) {
		alDeleteSources(1, &m_iSource);
		m_iSource	= (ALuint) - 1;
	}

	if(m_iBuffer != (ALuint) - 1) {
		alDeleteBuffers(1, &m_iBuffer);
		m_iBuffer	= (ALuint) - 1;
	}
}

bool DPI_SoundSource::Create(SOUND_BUFFER* pBuffer)
{
	Free();
	alGenBuffers(1, &m_iBuffer);
	alGenSources(1, &m_iSource);
	alBufferData(m_iBuffer, pBuffer->format, pBuffer->pData, pBuffer->byte_size, pBuffer->frequency);
	alSourcef(m_iSource, AL_PITCH, 1);
	alSourcef(m_iSource, AL_GAIN, 1);
	alSourcei(m_iSource, AL_BUFFER, m_iBuffer);
	return true;
}

void DPI_SoundSource::SetLoop(bool bLoop)
{
	if(m_iSource != (ALint) - 1) alSourcei(m_iSource, AL_LOOPING, bLoop);
}

void DPI_SoundSource::SetGain(float fGain)
{
	if(m_iSource != (ALint) - 1) alSourcef(m_iSource, AL_GAIN, fGain);
}

void DPI_SoundSource::SetPitch(float fPitch)
{
	if(m_iSource != (ALint) - 1) alSourcef(m_iSource, AL_PITCH, fPitch);
}

void DPI_SoundSource::Play(void)
{
	if(m_iSource != (ALint) - 1) alSourcePlay(m_iSource);
}

void DPI_SoundSource::Stop(void)
{
	if(IsPlaying()) {
		alSourceStop(m_iSource);
	}
}

bool DPI_SoundSource::IsPlaying(void)
{
	ALint	source_state = AL_STOPPED;

	if(m_iSource != (ALint) - 1) alGetSourcei(m_iSource, AL_SOURCE_STATE, &source_state);

	return (source_state == AL_PLAYING);
}
