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
// Title : Xilinx synthesis
// Rev.  : 10/31/2019 Thu (clonextop@gmail.com)
//================================================================================
#ifndef __SOURCE_LIST_H__
#define __SOURCE_LIST_H__
#include "ProjectConfig.h"
#include "HtmlTable.h"
#include <list>
using namespace std;

typedef struct{
	struct{
		float		frequency;		// maximum frequency (MHz)
		float		input_time;		// minimum input arrival time (ns)
		float		output_time;	// maximum output required time (ns)
		float		logic_delay;	// maximum combinational logic delay (ns)
	}timing;
	struct{
		struct{
			int			count;
			float		ratio;
		}regs, luts, brams, dsps;
	}area;
}SYNTHESIS_DATA;

typedef struct : public SYNTHESIS_DATA{
	time_t		time_stamp;
}SYNTHESIS_INFO;

class SourceGroup;

class SourceVector :
	public ProjectConfig
{
public:
	SourceVector(LPCTSTR sName, SourceGroup* pGroup);
	virtual ~SourceVector(void);

	void MakeTable(void);
	void UpdateTable(void);
	inline CString& Name(void)				{return m_sName;}
	inline CString& FullName(void)			{return m_sFullName;}
	inline SourceGroup* Group(void)			{return m_pGroup;}

	BOOL Synthesis(LPCTSTR sComparePath = NULL);

	void UpdateProfile(BOOL bUpdate = TRUE);
	void SetMark(void);

	void Report(void);
	inline const SYNTHESIS_DATA* Info(void)	{return m_Info.time_stamp ? &m_Info : NULL;}

	BOOL GetModuleName(CString& sModuleName);

protected:
	void Reset(void);
	BOOL Synthesis_Vivado(void);

private:
	time_t					m_TimeStamp;
	CString					m_sName;
	CString					m_sFullName;
	SourceGroup*			m_pGroup;
	SYNTHESIS_INFO			m_Info;
	SYNTHESIS_INFO			m_PreviousInfo;
};

class SourceGroup :
		public ProjectConfig
	{
public:
	SourceGroup(LPCTSTR sTargetPath = NULL, SourceGroup* pGroup = NULL);
	~SourceGroup(void);

	void ReleaseAll(void);

	size_t Size(void);
	inline LPCTSTR TargetPath(void)			{return m_sTargetPath;}
	inline LPCTSTR ScorePath(void)			{return m_sScorePath;}
	inline BOOL IsRoot(void)					{return m_sTargetPath.IsEmpty();}
	inline const SYNTHESIS_DATA* Info(void)	{return &m_Info;}

	void MakeTable(void);
	void UpdateTable(BOOL bUpdateSubTable = FALSE);
	void OpenFolder(LPCTSTR sPath = NULL);
	void OpenSource(LPCTSTR sPath);
	SourceGroup* FindGroup(LPCTSTR sPath);
	SourceVector* FindVector(LPCTSTR sPath);
	SourceVector* GetNextSource(SourceVector* pCurrent = NULL);

	void RemoveScoreFile(void);
	void FlushScoreFile(void);
	void SetMark(void);

	void ResetData(void);

private:
	void Scan(void);
	BOOL CheckHidden(LPCTSTR sFilePath);

	list<SourceGroup*>		m_GroupList;
	list<SourceVector*>		m_SourceList;
	SourceGroup*			m_pParent;

	CString					m_sTargetPath;
	CString					m_sScorePath;
	SYNTHESIS_DATA			m_Info;
};
#endif//__SOURCE_LIST_H__
