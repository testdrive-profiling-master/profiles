//================================================================================
// Copyright (c) 2013 ~ 2019. HyungKi Jeong(clonextop@gmail.com)
// All rights reserved.
// 
// The 3-Clause BSD License (https://opensource.org/licenses/BSD-3-Clause)
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
// Title : Testbench
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#include <dshow.h>
#include <initguid.h>
#include <strmif.h>
#include <uuids.h>
#include <iostream>
#include "comptr.h"
#include "VideoSource.h"

using namespace std;

#ifndef STDMETHODIMP	// for cppcheck
#define STDMETHODIMP HRESULT WINAPI
#define STDMETHODIMP_(type) type WINAPI
#endif

DEFINE_GUID(CLSID_SampleGrabber, 0xc1f400a0, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);
DEFINE_GUID(CLSID_NullRenderer, 0xc1f400a4, 0x3f08, 0x11d3, 0x9f, 0x0b, 0x00, 0x60, 0x08, 0x03, 0x9e, 0x37);
DEFINE_GUID(IID_ISampleGrabber, 0x6b652fff, 0x11fe, 0x4fce, 0x92, 0xad, 0x02, 0x66, 0xb5, 0xd7, 0xc7, 0x8f);
DEFINE_GUID(IID_ISampleGrabberCB, 0x0579154a, 0x2b53, 0x4994, 0xb0, 0xd0, 0xe7, 0x73, 0x14, 0x8e, 0xff, 0x85);
DEFINE_GUID(IID_IMediaEvent, 0x56a868b6, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_IMediaControl, 0x56a868b1, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_IVideoProcAmp, 0x4050560e, 0x42a7, 0x413a, 0x85, 0xc2, 0x09, 0x26, 0x9a, 0x2d, 0x0f, 0x44);
DEFINE_GUID(IID_IAMStreamConfig, 0xc6e13340, 0x30ac, 0x11d0, 0xa1, 0x8c, 0x00, 0xa0, 0xc9, 0x11, 0x89, 0x56);
DEFINE_GUID(IID_IBaseFilter, 0x56a86895, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);
DEFINE_GUID(IID_ICreateDevEnum, 0x29840822, 0x5b84, 0x11d0, 0xbd, 0x3b, 0x00, 0xa0, 0xc9, 0x11, 0xce, 0x86);
DEFINE_GUID(IID_IGraphBuilder, 0x56a868a9, 0x0ad4, 0x11ce, 0xb0, 0x3a, 0x00, 0x20, 0xaf, 0x0b, 0xa7, 0x70);

#define INTERFACE IVideoProcAmp
DECLARE_INTERFACE_(IVideoProcAmp, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;
};
#undef INTERFACE

#define INTERFACE ISampleGrabberCB
DECLARE_INTERFACE_(ISampleGrabberCB, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;
	STDMETHOD(SampleCB)(THIS_ double, IMediaSample*) PURE;
	STDMETHOD(BufferCB)(THIS_ double, BYTE*, long) PURE;
};
#undef INTERFACE

#define INTERFACE ISampleGrabber
DECLARE_INTERFACE_(ISampleGrabber, IUnknown)
{
	STDMETHOD(QueryInterface)(THIS_ REFIID, PVOID*) PURE;
	STDMETHOD_(ULONG, AddRef)(THIS) PURE;
	STDMETHOD_(ULONG, Release)(THIS) PURE;
	STDMETHOD(SetOneShot)(THIS_ BOOL) PURE;
	STDMETHOD(SetMediaType)(THIS_ const AM_MEDIA_TYPE*) PURE;
	STDMETHOD(GetConnectedMediaType)(THIS_ AM_MEDIA_TYPE*) PURE;
	STDMETHOD(SetBufferSamples)(THIS_ BOOL) PURE;
	STDMETHOD(GetCurrentBuffer)(THIS_ long*, long*) PURE;
	STDMETHOD(GetCurrentSample)(THIS_ IMediaSample**) PURE;
	STDMETHOD(SetCallBack)(THIS_ ISampleGrabberCB*, long) PURE;
};
#undef INTERFACE

ComPtr< IPin > getPin(IBaseFilter* filter, PIN_DIRECTION direction, int num)
{
	ComPtr< IPin > retVal;
	ComPtr< IEnumPins > enumPins;

	if(!FAILED(filter->EnumPins(&enumPins))) {
		ULONG found;
		ComPtr< IPin > pin;

		while(enumPins->Next(1, &pin, &found) == S_OK) {
			PIN_DIRECTION pinDirection = (PIN_DIRECTION)(-1);
			pin->QueryDirection(&pinDirection);

			if(pinDirection == direction) {
				if(num == 0) {
					retVal = pin;
					break;
				};

				num--;
			};
		};
	} else {
		MessageBox(NULL, "Enumeration of filter is failed.", "Error", MB_OK);
	}

	return retVal;
}

static ComPtr<IGraphBuilder>	m_graphBuilder;
static VideoSourceCallBack*		__pCB	= NULL;

class Callback: public ISampleGrabberCB {
public:
	Callback(void);
	virtual ~Callback(void);
	STDMETHODIMP QueryInterface(REFIID riid, void** ppv);
	STDMETHODIMP_(ULONG) AddRef(void);
	STDMETHODIMP_(ULONG) Release(void);
	STDMETHODIMP SampleCB(double, IMediaSample*);
	STDMETHODIMP BufferCB(double, BYTE*, long);
protected:
	long m_refCount;
};

Callback::Callback(void): m_refCount(1)
{
}

Callback::~Callback(void)
{
}

STDMETHODIMP Callback::QueryInterface(REFIID riid, void** ppv)
{
	HRESULT retval;

	if(ppv == NULL) return E_POINTER;

	{
		retval = E_NOINTERFACE;
	};
	return retval;
};

STDMETHODIMP_(ULONG) Callback::AddRef(void)
{
	m_refCount++;
	return m_refCount;
}

STDMETHODIMP_(ULONG) Callback::Release(void)
{
	if(!InterlockedDecrement(&m_refCount)) delete this;

	return m_refCount;
}

STDMETHODIMP Callback::SampleCB(double SampleTime, IMediaSample* pSample)
{
	if(pSample && __pCB) {
		BYTE*	pBuffer;
		pSample->GetPointer(&pBuffer);
		__pCB->SampleVideoSource(pSample->GetSize(), pBuffer);
	}

	return S_OK;
}

STDMETHODIMP Callback::BufferCB(double, BYTE*, long)
{
	return E_NOTIMPL;
}

Callback* callback = NULL;

static ComPtr<IGraphBuilder>		__graphBuilder;

DWORD	VideoSource::m_dwRefCount	= 0;

VideoSource::VideoSource(void) : m_Sema(1)
{
	if(!m_dwRefCount) CoInitialize(NULL);

	m_dwRefCount++;
}

VideoSource::~VideoSource(void)
{
	Release();
	m_dwRefCount--;

	if(!m_dwRefCount) CoUninitialize();
}

void SysLog(DWORD dwColor, const char* sFormat, ...);

#define _E_( condition, message ) \
	if ( !( condition ) ) { \
		SysLog(0xFF3F3F, "VideoSource : %s", message); \
		Release();\
		return FALSE; \
	};

#define _F_( condition, message ) \
	if ( (condition) != S_OK ) { \
		SysLog(0xFF3F3F, "VideoSource : %s", message); \
		Release();\
		return FALSE; \
	};

BOOL VideoSource::Create(int iWidth, int iHeight, SOURCE_TYPE type)
{
	SysLog(0xFFFFFF, "VideoSource : Trying to catch Video input(%dx%d)", iWidth, iHeight);
	_F_(__graphBuilder.coCreateInstance(CLSID_FilterGraph, IID_IGraphBuilder), "Could not create graph builder interface");
	ComPtr< ICreateDevEnum > createDevEnum;
	_F_(createDevEnum.coCreateInstance(CLSID_SystemDeviceEnum, IID_ICreateDevEnum), "Could not create device enumerator");
	ComPtr< IEnumMoniker > enumMoniker;
	_F_(createDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &enumMoniker, 0), "no video input moniker.");

	if(!enumMoniker.IsValid()) {
		Release();
		return FALSE;
	}

	_F_(enumMoniker->Reset(), "Error resetting moniker enumerator");
	int index = 0;
	ComPtr< IMoniker > moniker;

	for(int i = 0; i <= index; i++) {
		ULONG fetched = 0;
		_F_(enumMoniker->Next(1, &moniker, &fetched), "Error fetching next moniker");
	};

	enumMoniker.Release();

	ComPtr< IBaseFilter > source;

	_F_(moniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&source), "Error binding moniker to base filter");

	moniker.Release();

	_F_(__graphBuilder->AddFilter(source.get(), L"Source"), "Error adding camera source to filter graph");

	ComPtr< IPin > sourceOut = getPin(source.get(), PINDIR_OUTPUT, 0);

	_E_(sourceOut.get() != NULL, "Error getting output pin of camera source");

	ComPtr< IAMStreamConfig > streamConfig;

	_F_(sourceOut->QueryInterface(IID_IAMStreamConfig, (void**)&streamConfig), "Error requesting stream configuration API");

	int count, size;

	_F_(streamConfig->GetNumberOfCapabilities(&count, &size), "Error getting number of capabilities");

	{
		// 적절한 포멧 찾기
		BOOL bFound = FALSE;

		for(int i = 0; i < count; i++) {
			VIDEO_STREAM_CONFIG_CAPS videoConfig;
			AM_MEDIA_TYPE* mediaType;
			_F_(streamConfig->GetStreamCaps(i, &mediaType, (BYTE*)&videoConfig), "Error getting stream capabilities");

			//if(mediaType->majortype == MEDIATYPE_Video && mediaType->subtype == MEDIASUBTYPE_H264 && mediaType->cbFormat != 0) {
			if(mediaType->majortype == MEDIATYPE_Video && mediaType->cbFormat != 0) {
				VIDEOINFOHEADER* infoHeader = (VIDEOINFOHEADER*)mediaType->pbFormat;

				if(infoHeader->bmiHeader.biWidth && infoHeader->bmiHeader.biHeight) {
					// 여기서 포멧을 선택!!!
					if(iWidth) if(infoHeader->bmiHeader.biWidth != iWidth) goto BYPASS_CHECK;

					if(iHeight) if(infoHeader->bmiHeader.biHeight != iHeight) goto BYPASS_CHECK;

					if(!IsEqualGUID(mediaType->subtype, MEDIASUBTYPE_MJPG)) goto BYPASS_CHECK;

					streamConfig->SetFormat(mediaType);
					bFound = TRUE;
				}
			}

		BYPASS_CHECK:

			if(mediaType->cbFormat)
				CoTaskMemFree((PVOID)mediaType->pbFormat);

			if(mediaType->pUnk != NULL) mediaType->pUnk->Release();

			CoTaskMemFree((PVOID)mediaType);

			if(bFound) break;
		};

		streamConfig.Release();

		_E_(bFound, "Could not found a requested video format");
	}

	// MJPG decoder
	ComPtr< IBaseFilter > mjpg_decoder;
	_F_(CoCreateInstance(CLSID_MjpegDec, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&mjpg_decoder), "Error creating MPEG Video decoder");
	_F_(__graphBuilder->AddFilter(mjpg_decoder.get(), L"MPEG Video Deocder"), "Error adding MPEG Video decoder to filter graph");
	// grabber
	ComPtr< IBaseFilter > grabberBase;
	_F_(CoCreateInstance(CLSID_SampleGrabber, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&grabberBase), "Error creating sample grabber");
	_F_(__graphBuilder->AddFilter(grabberBase.get(), L"Grabber"), "Error adding sample grabber to filter graph");
	// sample grabber
	ComPtr< ISampleGrabber > sampleGrabber;
	_F_(grabberBase->QueryInterface(IID_ISampleGrabber, (void**)&sampleGrabber), "Error requesting sample grabber interface");
	_F_(sampleGrabber->SetOneShot(FALSE), "Error disabling one-shot mode");
	_F_(sampleGrabber->SetBufferSamples(TRUE), "Error enabling buffer sampling");
	// set callback
	callback = new Callback;
	_F_(sampleGrabber->SetCallBack(callback, 0), "Error setting callback interface for grabbing");
	// null renderer
	ComPtr< IBaseFilter > nullRenderer;
	_F_(CoCreateInstance(CLSID_NullRenderer, NULL, CLSCTX_INPROC, IID_IBaseFilter, (void**)&nullRenderer), "Error creating Null Renderer");
	_F_(__graphBuilder->AddFilter(nullRenderer.get(), L"Sink"), "Error adding null renderer to filter graph");
	// Attempting to connect
	{
		// source -> MPEG
		ComPtr< IPin > MPEGIn	= getPin(mjpg_decoder.get(), PINDIR_INPUT, 0);
		ComPtr< IPin > MPEGOut	= getPin(mjpg_decoder.get(), PINDIR_OUTPUT, 0);
		_F_(__graphBuilder->Connect(sourceOut.get(), MPEGIn.get()), "Error connecting source to MJPG grabber");
		// source -> grabber
		ComPtr< IPin > grabberIn = getPin(grabberBase.get(), PINDIR_INPUT, 0);
		_E_(grabberIn.get() != NULL, "Error getting input of sample grabber");
		ComPtr< IPin > grabberOut = getPin(grabberBase.get(), PINDIR_OUTPUT, 0);
		_E_(grabberOut.get() != NULL, "Error getting output of sample grabber");
		_F_(__graphBuilder->Connect(MPEGOut.get(), grabberIn.get()), "Error connecting source to sample grabber");
		// grabber -> null
		ComPtr< IPin > nullIn = getPin(nullRenderer.get(), PINDIR_INPUT, 0);
		_F_(__graphBuilder->Connect(grabberOut.get(), nullIn.get()), "Error connecting sample grabber to sink");
	}
	return TRUE;
}

void VideoSource::Release(void)
{
	SAFE_RELEASE(callback);
	__graphBuilder.Release();
}

BOOL VideoSource::IsValid(void)
{
	return __graphBuilder.IsValid();
}

BOOL VideoSource::Start(VideoSourceCallBack* pCB)
{
	if(__graphBuilder.IsValid()) {
		__pCB	= pCB;
		ComPtr<IMediaControl>	mediaControl;

		if(__graphBuilder->QueryInterface(IID_IMediaControl, (void**)&mediaControl) != S_OK)
			return FALSE;

		mediaControl->Run();	// why return S_FALSE??
	}

	return TRUE;
}

BOOL VideoSource::Stop(void)
{
	if(__graphBuilder.IsValid()) {
		ComPtr<IMediaControl>	mediaControl;
		ComPtr<IMediaEvent>		mediaEvent;
		_F_(__graphBuilder->QueryInterface(IID_IMediaControl, (void**)&mediaControl), "Error requesting media control interface");
		_F_(__graphBuilder->QueryInterface(IID_IMediaEvent, (void**)&mediaEvent), "Error requesting event interface");

		if(mediaControl.IsValid())
			mediaControl->Stop();

		Lock();
		// make sure copy is done.
		Sleep(10);
		Unlock();
		long evCode = 0;

		if(mediaEvent.IsValid())
			mediaEvent->WaitForCompletion(INFINITE, &evCode);

		Release();
	} else {
		return FALSE;
	}

	return TRUE;
}
