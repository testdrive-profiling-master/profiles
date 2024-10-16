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
// Title : Serial console
// Rev.  : 10/16/2024 Wed (clonextop@gmail.com)
//================================================================================
#ifndef __SERIAL_CONSOLE_H__
#define __SERIAL_CONSOLE_H__
#include "testdrive_document.h"
#include "Locale.h"

typedef enum {
	PROPERTY_ID_COM_PORT,
	PROPERTY_ID_BAUDRATE,
	PROPERTY_ID_DATA_BITS,
	PROPERTY_ID_PARITY,
	PROPERTY_ID_STOP_BITS,
	PROPERTY_ID_FLOW_CONTROL,
	PROPERTY_ID_SIZE
} PROPERTY_ID;

typedef enum {
	CONNECTABLE_FALSE,
	CONNECTABLE_TRUE,
	CONNECTABLE_OCCUPIED
} CONNECTABLE;

class SerialConsole : public TDImplDocumentBase
{
public:
	SerialConsole(ITDDocument *pDoc);
	virtual ~SerialConsole(void);

	STDMETHOD_(BOOL, OnCommand)(DWORD command, WPARAM wParam = NULL, LPARAM lParam = NULL);
	STDMETHOD_(BOOL, OnPropertyUpdate)(ITDPropertyData *pProperty);

	CONNECTABLE CheckPortExist(LPCTSTR lpszDevice);
	BOOL		AutoDetectPort(BOOL bWarning = TRUE);
	void		UpdateComportList(ITDPropertyData *pProperty = NULL);
	bool		InvokeSerialConsole(void);

protected:
	ITDDocument *m_pDoc;
	CString		 m_sComPort;
	DWORD		 m_dwBaudRate;
	DWORD		 m_dwDataBits;
	CString		 m_sParity;
	CString		 m_sStopBits;
	CString		 m_sFlowControl;
};
#endif //__SERIAL_CONSOLE_H__
