//================================================================================
// Copyright (c) 2013 ~ 2024. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Virtual FPGA Starter Kit Template design
// Rev.  : 6/27/2024 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __DPI_SOUND_H___
#define __DPI_SOUND_H___
#include "dpi_common.h"
#include <AL/al.h>
#include <AL/alc.h>

typedef enum {
	xSOUND_FORMAT_MONO8	   = AL_FORMAT_MONO8,
	xSOUND_FORMAT_MONO16   = AL_FORMAT_MONO16,
	xSOUND_FORMAT_STEREO8  = AL_FORMAT_STEREO8,
	xSOUND_FORMAT_STEREO16 = AL_FORMAT_STEREO16,
} xSOUND_FORMAT;

typedef struct {
	xSOUND_FORMAT format;
	int			  byte_size;
	int			  frequency;
	uint8_t		 *pData;
} SOUND_BUFFER;

class DPI_SoundSource;

class DPI_Sound
{
public:
	DPI_Sound(void);
	virtual ~DPI_Sound(void);

	bool			 Initialize(void);
	void			 ReleaseAll(void);

	DPI_SoundSource *CreateSource(SOUND_BUFFER *pBuffer);

private:
	ALCdevice  *m_pDevice;
	ALCcontext *m_pContext;
};

class DPI_SoundSource : public ChainList<DPI_SoundSource>
{
public:
	DPI_SoundSource(SOUND_BUFFER *pBuffer) : ChainList<DPI_SoundSource>(this)
	{
		m_iBuffer = (ALuint)-1;
		m_iSource = (ALuint)-1;
		Create(pBuffer);
	}
	virtual ~DPI_SoundSource(void)
	{
		Free();
	}

	bool Create(SOUND_BUFFER *pBuffer);
	void Play(void);
	void Stop(void);
	bool IsPlaying(void);
	void SetLoop(bool bLoop = true);
	void SetGain(float fGain);	 // 0 ~ 1.0
	void SetPitch(float fPitch); // 0.5 ~ 2.0
private:
	void   Free(void);

	ALuint m_iBuffer;
	ALuint m_iSource;
};

extern DPI_Sound g_Sound;

#endif //__X_SOUND_H___
