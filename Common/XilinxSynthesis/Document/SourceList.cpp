//================================================================================
// Copyright (c) 2013 ~ 2023. HyungKi Jeong(clonextop@gmail.com)
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
// Title : Xilinx synthesis
// Rev.  : 8/3/2023 Thu (clonextop@gmail.com)
//================================================================================
#include "SourceList.h"
#include <tchar.h>

SourceVector::SourceVector(LPCTSTR sName, SourceGroup* pGroup)
{
	m_sName			= sName;
	m_pGroup		= pGroup;
	m_sFullName.Format(_T("%s/%s"), m_pGroup->TargetPath(), Name());
	// reset whole profile
	Reset();
	memset(&m_PreviousInfo, 0, sizeof(m_PreviousInfo));
	m_PreviousInfo.timing.frequency		= -1;
	m_PreviousInfo.timing.input_time	= -1;
	m_PreviousInfo.timing.output_time	= -1;
	m_PreviousInfo.timing.logic_delay	= -1;
	// restore profile
	UpdateProfile();
}

SourceVector::~SourceVector(void)
{
}

void SourceVector::Reset(void)
{
	memset(&m_Info, 0, sizeof(m_Info));
	m_Info.timing.frequency				= -1;
	m_Info.timing.input_time			= -1;
	m_Info.timing.output_time			= -1;
	m_Info.timing.logic_delay			= -1;
}

void SourceVector::MakeTable(void)
{
	m_pTable->NewRow();
	m_pTable->NewCell(CELL_TD);
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_LEFT);
	m_pTable->SetBody(_T("<a href='open:%s'><img src='edit.png' title='%s' border='0'/></a> <a href='source:%s'>%s</a> <a href='report:%s'><img src='report.png' title='%s' border='0'/></a>"),
					  (LPCTSTR)FullName(),
					  _L(EDIT),
					  (LPCTSTR)FullName(), (LPCTSTR)Name(),
					  (LPCTSTR)FullName(),
					  _L(REPORT));
	m_pTable->NewCell(CELL_TD, _T("date_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	// timing
	m_pTable->NewCell(CELL_TD, _T("frequency_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	m_pTable->NewCell(CELL_TD, _T("hold_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	m_pTable->NewCell(CELL_TD, _T("comb_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	// area
	m_pTable->NewCell(CELL_TD, _T("regs_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	m_pTable->NewCell(CELL_TD, _T("luts_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	m_pTable->NewCell(CELL_TD, _T("brams_%s"), (LPCTSTR)FullName());
	m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	m_pTable->NewCell(CELL_TD, _T("dsps_%s"), (LPCTSTR)FullName());
	m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
}

static void __PrintTime(CString& str, int r, int g, int b, LPCTSTR sFormat, int iTime = 0, BOOL bBold = FALSE)
{
	CString sExFormat;
	sExFormat.Format(_T("%s%s%s%s%s"), bBold ? _T("<b>") : _T(""), _T("<font color='#%02X%02X%02X'>"), sFormat, _T("</font>"), bBold ? _T("</b>") : _T(""));
	str.Format(sExFormat, r, g, b, iTime);
}

static void __StrAppendRatio(CString& str, int ratio)
{
	int		cc		= (ratio * 255) / 100;
	BOOL	bOver	= ratio > 100;

	if(cc > 255) cc = 255;

	str	+= _T("<small><font color='#9F9F9F'> (</font>");

	if(bOver) str += _T("<u>");

	str.AppendFormat(_T("<font color='#%02X00%02X'><small>%d%%</small></font>"), cc, 255 - cc, ratio);

	if(bOver) str += _T("</u>");

	str	+= _T("<font color='#9F9F9F'>)</font></small>");
}

static void __StrAppendMark(CString& str, LPCTSTR sFormat, float diff, BOOL bUpPositive = TRUE)
{
	if(diff == 0) return;

	BOOL	bPositive	= bUpPositive ^ (diff < 0);
	DWORD	texColor	= bPositive ? 0x0000FF : 0xFF0000;
	CString	sValue;
	sValue.Format(sFormat, diff);
	str.AppendFormat(_T("<font color='#%06X'><small> : %s<img src='arrow_%s.png' border='0'/></small></font>"), texColor, (LPCTSTR)sValue, bPositive ? _T("up") : _T("down"));
}

void SourceVector::UpdateTable(void)
{
	// date
	{
		CString sBody;

		if(m_Info.time_stamp) {
			DWORD	day_diff	= ((GetCurrentDayTime() - m_Info.time_stamp) / (60 * 60 * 24));

			switch(day_diff) {
			case 0:
				__PrintTime(sBody, 0, 230, 0, _L(TODAY), 0, TRUE);
				break;

			case 1:
				__PrintTime(sBody, 0, 0, 255, _L(YESTERDAY), 0, TRUE);
				break;

			default: {
				int	ratio	= (255 * day_diff) / 31;

				if(ratio > 255) ratio = 255;

				{
					LPCTSTR	sFormat	= _L(DAYS_AGO);

					if(day_diff > (356 * 2)) {
						sFormat	= _L(YEARS_AGO);
						day_diff	/= 365;
					} else if(day_diff > 365) {
						sFormat	= _L(A_YEAR_AGO);
					} else if(day_diff > 61) {
						sFormat	= _L(MONTHS_AGO);
						day_diff	/= 30.416666f;	// 365/12
					} else if(day_diff > 30) {
						sFormat	= _L(A_MONTH_AGO);
					}

					__PrintTime(sBody, ratio, 0, 255 - ratio, sFormat, day_diff);
				}
			}
			}
		}

		m_pTable->SetField(_T("date_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);
	}
	// timing
	{
		// frequency
		CString sBody;

		if(m_Info.timing.frequency >= 0) {
			sBody.Format(_T("%.2f<font color='#9F9F9F'><small><small>MHz</small></small></font>"), m_Info.timing.frequency);

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.timing.frequency > 0)
				__StrAppendMark(sBody, _T("%+.2f"), m_Info.timing.frequency - m_PreviousInfo.timing.frequency);
		} else {
			float fMaxDelay = -1;

			if(m_Info.timing.input_time > fMaxDelay)	fMaxDelay	= m_Info.timing.input_time;

			if(m_Info.timing.output_time > fMaxDelay)	fMaxDelay	= m_Info.timing.output_time;

			if(m_Info.timing.logic_delay > fMaxDelay)	fMaxDelay	= m_Info.timing.logic_delay;

			if(fMaxDelay <= 0)
				sBody	= sBLANK_FILED;
			else
				sBody.AppendFormat(_T("<small><font color='#AFAFAF'>%.2f<small>MHz</small>%s</font></small>"), (1000.f / fMaxDelay), _L(ESTIMATED));
		}

		m_pTable->SetField(_T("frequency_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);

		// i/o hold time
		if(m_Info.timing.input_time >= 0) {
			sBody.Format(_T("%.2f<font color='#9F9F9F'><small><small>ns</small></small></font>"), m_Info.timing.input_time);

			if(m_Info.timing.frequency >= 0) {
				__StrAppendRatio(sBody, (int)(m_Info.timing.frequency / (10.f / m_Info.timing.input_time)));
			}

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.timing.input_time > 0)
				__StrAppendMark(sBody, _T("%+.2f"), m_Info.timing.input_time - m_PreviousInfo.timing.input_time, FALSE);
		} else {
			sBody	= sBLANK_FILED;
		}

		sBody	+= _T("<font color='#9F9F9F'> / </font>");

		if(m_Info.timing.output_time >= 0) {
			sBody.AppendFormat(_T("%.2f<font color='#9F9F9F'><small><small>ns</small></small></font>"), m_Info.timing.output_time);

			if(m_Info.timing.frequency >= 0) {
				__StrAppendRatio(sBody, (int)(m_Info.timing.frequency / (10.f / m_Info.timing.output_time)));
			}

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.timing.output_time > 0)
				__StrAppendMark(sBody, _T("%+.2f"), m_Info.timing.output_time - m_PreviousInfo.timing.output_time, FALSE);
		} else {
			sBody	+= sBLANK_FILED;
		}

		/*if(m_Info.timing.frequency>=0 && (m_Info.timing.input_time>=0 || m_Info.timing.output_time>=0)){
			// calculate margin
			CString sMargin;
			sMargin.Format(_T("<u ONMOUSEOVER=\\\"popup('margin shows here!!!',200)\\\"; ONMOUSEOUT=\\\"kill()\\\">%s</u>"),
					(LPCTSTR)sBody);
			sBody	= sMargin;
		}*/
		m_pTable->SetField(_T("hold_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);

		// combinational delay
		if(m_Info.timing.logic_delay >= 0) {
			sBody.Format(_T("%.3f<font color='#9F9F9F'><small><small>ns</small></small></font>"), m_Info.timing.logic_delay);

			if(m_Info.timing.frequency >= 0) {
				__StrAppendRatio(sBody, (int)(m_Info.timing.frequency / (10.f / m_Info.timing.logic_delay)));
			}

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.timing.logic_delay > 0)
				__StrAppendMark(sBody, _T("%+.3f"), m_Info.timing.logic_delay - m_PreviousInfo.timing.logic_delay, FALSE);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("comb_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);
	}
	// area
	{
		CString		sBody;
		auto GetColor = [](float fRatio) -> DWORD {
			DWORD dwColor;

			if(fRatio > 100) fRatio = 100;

			DWORD	dwBlack		= (159 * (100 - fRatio)) / 100;
			DWORD	dwRed		= (159 * (100 - fRatio) + (255 * fRatio)) / 100;
			dwColor				= ((dwRed << 16) | (dwBlack << 8) | dwBlack);
			return dwColor;
		};
		auto FormatRatio = [&](CString & sBody, int reg_count, float fRatio) {
			BOOL	bOverflow	= (fRatio > 100);
			LPCTSTR		sRatiox2	= _T("%d<font color='#9F9F9F'><small><small> (%s<font color='#%06X'>%.2f%%</font>%s)</small></small></font>");
			LPCTSTR		sRatiox1	= _T("%d<font color='#9F9F9F'><small><small> (%s<font color='#%06X'>%.1f%%</font>%s)</small></small></font>");
			sBody.Format(fRatio < 10.f ? sRatiox2 : sRatiox1, reg_count, bOverflow ? _T("<u>") : _T(""), GetColor(fRatio), fRatio, bOverflow ? _T("</u>") : _T(""));
		};
		auto FormatRatio2 = [&](CString & sBody, int reg_count1, int reg_count2, float fRatio1, float fRatio2) {
			BOOL	bOverflow1		= (fRatio1 > 100);
			BOOL	bOverflow2		= (fRatio2 > 100);
			sBody.clear();
			sBody.AppendFormat(_T("%d"), reg_count1);
			sBody.Append(_T("<font color='#9F9F9F'>/</font>"));
			sBody.AppendFormat(_T("%d"), reg_count2);
			sBody.Append(_T("<font color='#9F9F9F'><small><small> ("));
			{
				if(bOverflow1)sBody.Append(_T("<u>"));

				if(fRatio1) {
					sBody.AppendFormat(_T("<font color='#%06X'>"), GetColor(fRatio1));
					sBody.AppendFormat(fRatio1 < 10.f ? _T("%.2f%%") : _T("%.1f%%"), fRatio1);
					sBody.Append(_T("</font>"));
				} else {
					sBody.AppendFormat(_T("-"));
				}

				if(bOverflow1)sBody.Append(_T("</u>"));
			}
			sBody.Append(_T("<font color='#9F9F9F'>/</font>"));
			{
				if(bOverflow2)sBody.Append(_T("<u>"));

				if(fRatio2) {
					sBody.AppendFormat(_T("<font color='#%06X'>"), GetColor(fRatio2));
					sBody.AppendFormat(fRatio2 < 10.f ? _T("%.2f%%") : _T("%.1f%%"), fRatio2);
					sBody.Append(_T("</font>"));
				} else {
					sBody.AppendFormat(_T("-"));
				}

				if(bOverflow2)sBody.Append(_T("</u>"));
			}
			sBody.Append(_T(")</small></small></font>"));
		};

		// registers
		if(m_Info.area.regs.count > 0) {
			FormatRatio(sBody, m_Info.area.regs.count, m_Info.area.regs.ratio);

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.area.regs.count >= 0)
				__StrAppendMark(sBody, _T("%+.0f"), (float)(m_Info.area.regs.count - m_PreviousInfo.area.regs.count), FALSE);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("regs_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);

		// luts
		if(m_Info.area.luts.count > 0) {
			FormatRatio(sBody, m_Info.area.luts.count, m_Info.area.luts.ratio);

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.area.luts.count >= 0)
				__StrAppendMark(sBody, _T("%+.0f"), (float)(m_Info.area.luts.count - m_PreviousInfo.area.luts.count), FALSE);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("luts_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);

		// brams / urams
		if(m_Info.area.brams.count > 0 || m_Info.area.urams.count > 0) {
			FormatRatio2(sBody, m_Info.area.brams.count, m_Info.area.urams.count, m_Info.area.brams.ratio, m_Info.area.urams.ratio);

			if(m_PreviousInfo.time_stamp) {
				if(m_PreviousInfo.area.brams.count >= 0) {
					__StrAppendMark(sBody, _T("%+.0f"), (float)(m_Info.area.brams.count - m_PreviousInfo.area.brams.count), FALSE);
				} else {
					sBody.Append(_T("-"));
				}

				sBody.Append(_T("<font color='#9F9F9F'>/</font>"));

				if(m_PreviousInfo.area.urams.count >= 0) {
					__StrAppendMark(sBody, _T("%+.0f"), (float)(m_Info.area.urams.count - m_PreviousInfo.area.urams.count), FALSE);
				} else {
					sBody.Append(_T("-"));
				}
			}
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("brams_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);

		// dsps
		if(m_Info.area.dsps.count > 0) {
			FormatRatio(sBody, m_Info.area.dsps.count, m_Info.area.dsps.ratio);

			if(m_PreviousInfo.time_stamp && m_PreviousInfo.area.dsps.count >= 0)
				__StrAppendMark(sBody, _T("%+.0f"), (float)(m_Info.area.dsps.count - m_PreviousInfo.area.dsps.count), FALSE);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("dsps_%s"), _T("%s"), (LPCTSTR)FullName(), (LPCTSTR)sBody);
	}
}

BOOL SourceVector::Synthesis_Vivado(void)
{
	BOOL bRet				= FALSE;
	float	fClockLatency	= 0;
	CString sWorkDir		= RetrievePath(WORK_DIR_NAME);
	CString sIncludePaths	= _T("$env(TESTDRIVE_PROFILE)Common/System/SystemSim/HDL  $env(PROJECT)System\\HDL");
	CString sName			= Name();
	CString	sTopName;	// top module name
	sName.Replace(_T('.'), _T('\0'));

	// get top module name
	if(!GetModuleName(sTopName)) {
		g_pSystem->LogInfo(_L(MODULE_NAME_IS_NOT_FOUND));
		return FALSE;
	}

	g_pSystem->LogInfo(_L(CURRENT_TOP_MODULE), (LPCTSTR)sName);
	{
		// clock latency
		int mhz			= _ttoi(m_Config.spec.sTargetSpeed);

		if(mhz) fClockLatency	= 1000.f / mhz;

		if(!fClockLatency)
			return FALSE;
	}

	if(_taccess(sWorkDir, 0) == -1) {	// not exist
		_tmkdir(sWorkDir);
	}

	{
		// making default constraint
		FILE*	fp		= NULL;
		{
			// create script file
			CString	sPath;
			sPath.Format(_T("%s\\default.xdc"), (LPCTSTR)sWorkDir);

			if((fp = _tfopen(sPath, _T("wt")))) {
				FilePrint(fp, _T("create_clock -name main_clk -period %f [get_ports -filter {NAME =~  \"%s\"}]\n"), fClockLatency, (LPCTSTR)m_Config.spec.sClocks);
				FilePrint(fp, _T("set_false_path -from [get_ports -filter {NAME =~  \"%s\"}]\n"), (LPCTSTR)m_Config.spec.sResets);
			} else return FALSE;

			fclose(fp);
		}
	}

	{
		// making synthesis script.
		FILE*	fp		= NULL;
		{
			// create script file
			CString	sPath;
			sPath.Format(_T("%s\\%s.tcl"), (LPCTSTR)sWorkDir, (LPCTSTR)g_sGlobalName);
			fp			= _tfopen(sPath, _T("wt"));
		}

		if(fp) {
			CString	sDevice;
			sDevice	= MakeDevicePartName();
			FilePrint(fp, _T("#\n# Synthesis run script generated by TestDrive Profiling Master\n#\n\n" \
							 "create_project -in_memory -part %s\n\n"), (LPCTSTR)sDevice);
			{
				char* sEnv	= getenv("NUMBER_OF_PROCESSORS");

				if(sEnv) {
					int iProcessCount	= atoi(sEnv);

					if(iProcessCount > 8) iProcessCount = 8;

					if(iProcessCount < 1) iProcessCount = 2;

					FilePrint(fp, _T(
								  "var vivado_version_year\n" \
								  "scan [version -short] %%d vivado_version_year\n" \
								  "if {$vivado_version_year >= 2015} {\n" \
								  "\tset_param general.maxThreads %d\n" \
								  "\tset_param synth.maxThreads %d\n}\n"),
							  iProcessCount, iProcessCount);
				}
			}
			{
				CString sIncList, sSrcList;
				sIncList	+= _T("  $env(TESTDRIVE_PROFILE)Common/System/SystemSim/HDL\n");
				sIncList.AppendFormat(_T("  %s\n"), g_pSystem->RetrieveFullPath(_T("%PROJECT%System\\HDL")));
				sSrcList.AppendFormat(_T("read_xdc default.xdc\n"));
				{
					CString st;
					RetrieveSourcePath(m_sFullName, st);
				}
				{
					// add source list
					CString sSourceListPath;

					if(RetrieveSourcePath(m_sFullName, sSourceListPath)) {
						FILE* fp_src = _tfopen(sSourceListPath, _T("rt"));
						TCHAR sLine[8192];
						{
							// add source list path to include search path
							int iPos	= sSourceListPath.ReverseFind(_T('\\'));
							sSourceListPath.Delete(iPos, sSourceListPath.GetLength());
							sSourceListPath.Replace(_T('\\'), _T('/'));
							sIncList.AppendFormat(_T("  %s\n"), (LPCTSTR)sSourceListPath);
							sIncludePaths.AppendFormat(_T("  %s"), (LPCTSTR)sSourceListPath);
						}

						while(_fgetts(sLine, 8192, fp_src)) {
							CString sSrc(sLine);
							// eliminate comment
							sSrc.Replace(_T("//"), _T(";"));
							sSrc.Replace(_T(';'), _T('\0'));
							// trimming
							sSrc.TrimLeft(_T(" \t"));
							sSrc.TrimRight(_T(" \t\r\n"));

							// add source
							if(!sSrc.IsEmpty()) {
								CString sFullPath;

								if(sSrc.GetAt(0) == _T('@')) {
									// add includes
									sSrc.Delete(0, 1);
									sIncList.AppendFormat(_T("  %s\n"), (LPCTSTR)RetrievePath(sSrc));
								} else if(sSrc.GetAt(0) == _T('#')) {
									// bypass string
									sSrc.Delete(0, 1);
									FilePrint(fp, _T("%s\n"), (LPCTSTR)sSrc);
								} else {
									// add source(s)
									sFullPath	= RetrievePath(sSrc);

									if(sFullPath.Find(_T('*')) != -1) {
										HANDLE				hFind;
										WIN32_FIND_DATA		FindFileData;
										hFind	= FindFirstFile(sFullPath, &FindFileData);

										if(hFind != INVALID_HANDLE_VALUE) {
											TCHAR sDirPath[MAX_PATH], *pFilePart;
											GetFullPathName(sFullPath, MAX_PATH, sDirPath, &pFilePart);

											if(pFilePart) *pFilePart = _T('\0');

											do {
												if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
													sFullPath.Format(_T("%s%s"), sDirPath, FindFileData.cFileName);

													if(sFullPath.Find(_T(".xdc")) != -1)
														sSrcList.AppendFormat(_T("read_xdc %s\n"), (LPCTSTR)sFullPath);
													else
														sSrcList.AppendFormat(_T("read_verilog -library xil_defaultlib %s\n"), (LPCTSTR)sFullPath);
												}
											} while(FindNextFile(hFind, &FindFileData));

											FindClose(hFind);
										}
									} else {
										if(!IsFileExist(sFullPath)) {	// not actual exist
											// search with testdrive common HDL path
											sFullPath.Format(_T("%%TESTDRIVE_PROFILE%%Common/System/SystemSim/HDL/%s"), sSrc.c_str());
											sFullPath	= g_pSystem->RetrieveFullPath(sFullPath);

											if(!IsFileExist(sFullPath)) {
												g_pSystem->LogError(_L(SOURCE_IS_NOT_FOUND), (LPCTSTR)sSrc);
												bRet	= FALSE;
											}
										}

										if(sFullPath.Find(_T(".xdc")) != -1)
											sSrcList.AppendFormat(_T("read_xdc %s\n"), (LPCTSTR)sFullPath);
										else
											sSrcList.AppendFormat(_T("read_verilog -library xil_defaultlib %s\n"), (LPCTSTR)sFullPath);
									}
								}
							}
						}

						fclose(fp_src);
					} else {
						fclose(fp);
						g_pSystem->LogError(_L(SOURCE_LIST_IS_NEEDED), SOURCE_LIST_FILE_NAME);
						return FALSE;
					}

					// set include paths & sources
					sIncList.Replace(_T('\\'), _T('/'));
					sSrcList.Replace(_T('\\'), _T('/'));
					FilePrint(fp, _T("set_param synth.vivado.isSynthRun true\n" \
									 "set_msg_config -source 4 -id {IP_Flow 19-2162} -severity warning -new_severity info\n" \
									 "set_property default_lib xil_defaultlib [current_project]\n" \
									 "set_property target_language Verilog [current_project]\n" \
									 "set_property vhdl_version vhdl_2k [current_fileset]\n" \
									 "set_property include_dirs {\n" \
									 "%s" \
									 "} [current_fileset]\n\n" \
									 "%s\n"),
							  (LPCTSTR)sIncList,
							  (LPCTSTR)sSrcList);
				}
			}
			FilePrint(fp, _T("synth_design -directive PerformanceOptimized -include_dirs \"%s\" -top %s -part %s -no_iobuf\n"),
					  (LPCTSTR)sIncludePaths,
					  (LPCTSTR)sTopName,
					  (LPCTSTR)sDevice);
			FilePrint(fp, _T("catch { report_utilization -file synthesis_%s.rpt }\n"), (LPCTSTR)sName);
			// setup timing
			FilePrint(fp, _T("catch { report_timing -from [get_ports *] -to main_clk -no_header -quiet -max_paths %d -file timing_setup_%s.rpt}\n"), m_Config.iMaxPaths, (LPCTSTR)sName);
			// hold timing
			FilePrint(fp, _T("catch { report_timing -from main_clk -to [get_ports *] -no_header -quiet -max_paths %d -file timing_hold_%s.rpt}\n"), m_Config.iMaxPaths, (LPCTSTR)sName);
			// clock path timing
			FilePrint(fp, _T("catch { report_timing -from main_clk -to main_clk -no_header -quiet -max_paths %d -file timing_clock_%s.rpt}\n"), m_Config.iMaxPaths, (LPCTSTR)sName);
			// port to port
			FilePrint(fp, _T("catch { report_timing -from [get_ports *] -to [get_ports *] -no_header -quiet -max_paths %d -file timing_port_%s.rpt}\n"), m_Config.iMaxPaths, (LPCTSTR)sName);
			fclose(fp);
		}
	}

	{
		// run synthesis
		CString sBatch, sArg;
		sBatch.Format(_T("%s\\%s_vivado.bat"), (LPCTSTR)m_Config.sDocPath, g_sGlobalName);
		sArg.Format(_T("\"%s\\settings64.bat\" %s"),
					(LPCTSTR)m_Config.sXilinxPath,
					(LPCTSTR)sName);
		{
			CString sEnvPath;
			sEnvPath.Format(_T("%s\\settings64.bat"), (LPCTSTR)m_Config.sXilinxPath);

			if(!IsFileExist(sEnvPath)) {
				g_pSystem->LogError(_L(NO_XILINX_ENVIRONMENT));
				return FALSE;
			}
		}
		int iRet = g_pSystem->ExecuteFile(sBatch, sArg, TRUE, NULL, m_Config.sProjectPath,
										  _T("failed:"), -1,
										  _T("ERROR:"), -1,
										  _T("WARNING:"), 0,
										  _T("synth_design completed successfully"), 1,
										  NULL);

		if(iRet == 1) {
			g_pSystem->LogInfo(_L(SYNTHESIS_IS_DONE));
		} else {
			g_pSystem->LogError(_L(SYNTHESIS_IS_FAILED));
			return FALSE;
		}
	}

	{
		// merge down timing reports...
		FILE* fp;
		FILE* fp_src;
		LPCTSTR sFormat = _T(
							  "============================================================================================\n" \
							  "= %s\n" \
							  "============================================================================================\n\n");
		CString sFileName;
		TCHAR sLine[4096];
		// read utilization
		sFileName.Format(_T("%s\\timing_%s.txt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

		if((fp = _tfopen(sFileName, _T("wt")))) {
			//put setup timing
			FilePrint(fp, sFormat, _L(PATH_CLOCK_SETUP));
			{
				sFileName.Format(_T("%s\\timing_setup_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

				if((fp_src = _tfopen(sFileName, _T("rt")))) {
					while(_fgetts(sLine, 4096, fp_src))
						FilePrint(fp, _T("%s"), sLine);

					fclose(fp_src);
				}
			}
			//put setup timing
			FilePrint(fp, sFormat, _L(PATH_CLOCK_HOLD));
			{
				sFileName.Format(_T("%s\\timing_hold_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

				if((fp_src = _tfopen(sFileName, _T("rt")))) {
					while(_fgetts(sLine, 4096, fp_src))
						FilePrint(fp, _T("%s"), sLine);

					fclose(fp_src);
				}
			}
			//put setup timing
			FilePrint(fp, sFormat, _L(PATH_CLOCK_PULSE));
			{
				sFileName.Format(_T("%s\\timing_clock_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

				if((fp_src = _tfopen(sFileName, _T("rt")))) {
					while(_fgetts(sLine, 4096, fp_src))
						FilePrint(fp, _T("%s"), sLine);

					fclose(fp_src);
				}
			}
			//put setup timing
			FilePrint(fp, sFormat, _L(PATH_COMBINATIONAL));
			{
				sFileName.Format(_T("%s\\timing_port_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

				if((fp_src = _tfopen(sFileName, _T("rt")))) {
					while(_fgetts(sLine, 4096, fp_src))
						FilePrint(fp, _T("%s"), sLine);

					fclose(fp_src);
				}
			}
			fclose(fp);
		}
	}

	Reset();
	m_Info.time_stamp	= GetCurrentDayTime();
	{
		// read from systhesis result
		FILE* fp;
		{
			// read utilization
			CString sFileName;
			sFileName.Format(_T("%s\\synthesis_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);
			fp = _tfopen(sFileName, _T("rt"));

			if(!fp) return FALSE;
		}
		{
			TCHAR sLine[1024];
			LPCTSTR	sTokDlim	= _T(" |");
			struct {
				BOOL	Registers;
				BOOL	LUTs;
				BOOL	BRAM;
				BOOL	URAM;
				BOOL	DSPs;
			} found = {FALSE};

			while(_fgetts(sLine, 1024, fp)) {
				TCHAR* sTok;

				if(!found.Registers && ((sTok = _tcsstr(sLine, _T("| Slice Registers"))) || (sTok = _tcsstr(sLine, _T("| CLB Registers"))))) {
					CString st	= (sTok + 17);
					int pos		= 0;
					m_Info.area.regs.count	= _ttoi(st.Tokenize(sTokDlim, pos));
					st.Tokenize(sTokDlim, pos);	// fixed
					st.Tokenize(sTokDlim, pos);	// prohibited
					int iTotal				= _ttoi(st.Tokenize(sTokDlim, pos));

					if(iTotal) m_Info.area.regs.ratio	= (100.f * m_Info.area.regs.count) / iTotal;

					found.Registers		= TRUE;
				} else if(!found.LUTs && ((sTok = _tcsstr(sLine, _T("| Slice LUTs*"))) || (sTok = _tcsstr(sLine, _T("| CLB LUTs*"))))) {
					CString st	= (sTok + 13);
					int pos		= 0;
					m_Info.area.luts.count	= _ttoi(st.Tokenize(sTokDlim, pos));
					st.Tokenize(sTokDlim, pos);	// fixed
					st.Tokenize(sTokDlim, pos);	// prohibited
					int iTotal				= _ttoi(st.Tokenize(sTokDlim, pos));

					if(iTotal) m_Info.area.luts.ratio	= (100.f * m_Info.area.luts.count) / iTotal;

					found.LUTs			= TRUE;
				} else if(!found.BRAM && (sTok = _tcsstr(sLine, _T("| Block RAM Tile ")))) {
					CString st	= (sTok + 16);
					int pos		= 0;
					{
						// round up.
						float fCount	= 0;
						_stscanf(st.Tokenize(sTokDlim, pos), _T("%f"), &fCount);
						m_Info.area.brams.count	= (int)(fCount + 0.9999f);
					}
					st.Tokenize(sTokDlim, pos);	// fixed
					st.Tokenize(sTokDlim, pos);	// prohibited
					int iTotal				= _ttoi(st.Tokenize(sTokDlim, pos));	// total

					if(iTotal) m_Info.area.brams.ratio	= (100.f * m_Info.area.brams.count) / iTotal;

					found.BRAM			= TRUE;
				} else if(!found.URAM && (sTok = _tcsstr(sLine, _T("| URAM ")))) {
					CString st	= (sTok + 6);
					int pos		= 0;
					{
						// round up.
						float fCount	= 0;
						_stscanf(st.Tokenize(sTokDlim, pos), _T("%f"), &fCount);
						m_Info.area.urams.count	= (int)(fCount + 0.9999f);
					}
					st.Tokenize(sTokDlim, pos);	// fixed
					st.Tokenize(sTokDlim, pos);	// prohibited
					int iTotal				= _ttoi(st.Tokenize(sTokDlim, pos));	// total

					if(iTotal) m_Info.area.urams.ratio	= (100.f * m_Info.area.urams.count) / iTotal;

					found.URAM			= TRUE;
				} else if(!found.DSPs && (sTok = _tcsstr(sLine, _T("| DSPs")))) {
					CString st	= (sTok + 6);
					int pos		= 0;
					m_Info.area.dsps.count	= _ttoi(st.Tokenize(sTokDlim, pos));
					st.Tokenize(sTokDlim, pos);	// fixed
					st.Tokenize(sTokDlim, pos);	// prohibited
					int iTotal				= _ttoi(st.Tokenize(sTokDlim, pos));

					if(iTotal) m_Info.area.dsps.ratio	= (100.f * m_Info.area.dsps.count) / iTotal;

					found.DSPs			= TRUE;
				}
			}

			fclose(fp);
		}
		{
			// read setup timing
			CString sFileName;
			sFileName.Format(_T("%s\\timing_setup_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

			if((fp = _tfopen(sFileName, _T("rt")))) {
				TCHAR sLine[1024];

				while(_fgetts(sLine, 1024, fp)) {
					TCHAR* sTok;

					if((sTok = _tcsstr(sLine, _T("Data Path Delay:")))) {
						sTok	+= 16;
						_stscanf(sTok, _T("%f"), &m_Info.timing.input_time);
						break;
					}
				}

				fclose(fp);
			}
		}
		{
			// read hold timing
			CString sFileName;
			sFileName.Format(_T("%s\\timing_hold_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

			if((fp = _tfopen(sFileName, _T("rt")))) {
				TCHAR sLine[1024];

				while(_fgetts(sLine, 1024, fp)) {
					TCHAR* sTok;

					if((sTok = _tcsstr(sLine, _T("Data Path Delay:")))) {
						sTok	+= 16;
						_stscanf(sTok, _T("%f"), &m_Info.timing.output_time);
						break;
					}
				}

				fclose(fp);
			}
		}
		{
			// read clock width timing
			CString sFileName;
			sFileName.Format(_T("%s\\timing_clock_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

			if((fp = _tfopen(sFileName, _T("rt")))) {
				TCHAR sLine[1024];
				float	fSlack			= 0.f;

				while(_fgetts(sLine, 1024, fp)) {
					TCHAR* sTok;

					if(sLine == _tcsstr(sLine, _T("Slack ("))) {
						sTok	= _tcsstr(sLine, _T(":")) + 1;
						_stscanf(sTok, _T("%f"), &fSlack);
						float fTotal	= (fClockLatency - fSlack);

						if(fTotal) {
							m_Info.timing.frequency	= 1000.f / fTotal;
							break;
						}
					}
				}

				fclose(fp);
			}
		}
		{
			// read combinational delay
			CString sFileName;
			sFileName.Format(_T("%s\\timing_port_%s.rpt"), (LPCTSTR)sWorkDir, (LPCTSTR)sName);

			if((fp = _tfopen(sFileName, _T("rt")))) {
				TCHAR sLine[1024];

				while(_fgetts(sLine, 1024, fp)) {
					TCHAR* sTok;

					if((sTok = _tcsstr(sLine, _T("Data Path Delay:")))) {
						sTok	+= 16;
						float fDelay	= 0;

						if(_stscanf(sTok, _T("%f"), &fDelay)) {
							m_Info.timing.logic_delay	= fDelay;
							break;
						}
					}
				}

				fclose(fp);
			}
		}
		bRet	= TRUE;
	}
	return bRet;
}

BOOL SourceVector::Synthesis(LPCTSTR sComparePath)
{
	BOOL bSuccess	= FALSE;

	if(sComparePath) {
		CString sCmp;
		sCmp.Format(_T("%s/%s"), m_pGroup->TargetPath(), (LPCTSTR)Name());

		if(sCmp.Compare(sComparePath)) return FALSE;
	}

	g_pSystem->ClearLog();
	g_pSystem->LogInfo(_L(CURRENT_SYNTHESIS_TARGET), m_pGroup->TargetPath(), (LPCTSTR)Name());

	if(!CheckConfigCompleteness()) {
		g_pSystem->LogInfo(_L(MUST_FILL_ALL_PROPERTIES));
		return FALSE;
	}

	// Do Synthesis
	m_pTable->JScript(_T("ShowWait(\"%s<br><font color=#3F3FFF><b>'%s/%s'</b></font>\");"), _L(SYNTHESIS_IS_RUNNING), m_pGroup->TargetPath(), (LPCTSTR)Name());
	bSuccess	= Synthesis_Vivado();
	m_pTable->JScript(_T("ShowWait(0);"));

	// update table
	if(bSuccess) {
		UpdateProfile(FALSE);
		m_pGroup->ResetData();
		UpdateTable();
		m_pGroup->UpdateTable();
	}

	return bSuccess;
}

void SourceVector::UpdateProfile(BOOL bUpdate)
{
	static LPCTSTR	__sCURRENT		= _T("CURRENT");
	static	LPCTSTR	__sPREVIOUS		= _T("PREVIOUS");
	BYTE*		pInfo				= (BYTE*)&m_Info;
	CString		sScorePath			= m_pGroup->ScorePath();

	if(bUpdate) {	// read
		TCHAR	sData[2048];
		Reset();
		// get current
		GetPrivateProfileString(__sCURRENT, FullName(), _T(""), sData, 2048, sScorePath);

		if(_tcslen(sData) == 2 * sizeof(SYNTHESIS_INFO)) {
			for(int i = 0; i < sizeof(SYNTHESIS_INFO); i++) {
				DWORD	dwData;
				_stscanf(&sData[i * 2], _T("%02X"), (unsigned int*)&dwData);
				pInfo[i]	= dwData;
			}
		}

		// get previous
		pInfo	= (BYTE*)&m_PreviousInfo;
		GetPrivateProfileString(__sPREVIOUS, FullName(), _T(""), sData, 2048, sScorePath);

		if(_tcslen(sData) == 2 * sizeof(SYNTHESIS_INFO)) {
			for(int i = 0; i < sizeof(SYNTHESIS_INFO); i++) {
				DWORD	dwData;
				_stscanf(&sData[i * 2], _T("%02X"), (unsigned int*)&dwData);
				pInfo[i]	= dwData;
			}
		}
	} else {			// write
		CString		sData;

		// write current
		for(int i = 0; i < sizeof(SYNTHESIS_INFO); i++)
			sData.AppendFormat(_T("%02X"), pInfo[i]);

		WritePrivateProfileString(__sCURRENT, FullName(), (LPCTSTR)sData, sScorePath);

		// write previous
		if(m_PreviousInfo.time_stamp) {
			pInfo	= (BYTE*)&m_PreviousInfo;
			sData.Empty();

			for(int i = 0; i < sizeof(SYNTHESIS_INFO); i++)
				sData.AppendFormat(_T("%02X"), pInfo[i]);

			WritePrivateProfileString(__sPREVIOUS, FullName(), (LPCTSTR)sData, sScorePath);
		}
	}
}

void SourceVector::SetMark(void)
{
	memcpy(&m_PreviousInfo, &m_Info, sizeof(SYNTHESIS_INFO));
	UpdateProfile(FALSE);
	UpdateTable();
}

void SourceVector::Report(void)
{
	CString sName	 = Name();
	CString sWorkDir = RetrievePath(WORK_DIR_NAME);
	sName.Replace(_T('.'), _T('\0'));
	{
		CString sReportFile;
		sReportFile.Format(_T("synthesis_%s.txt"), (LPCTSTR)sName);
		//ShellExecute(NULL, NULL, g_pSystem->RetrieveFullPath(g_sNotepadPath), sReportFile, sWorkDir, SW_SHOW);
		ShellExecute(NULL, NULL, sReportFile, NULL, sWorkDir, SW_SHOW);
		sReportFile.Format(_T("timing_%s.txt"), (LPCTSTR)sName);
		///ShellExecute(NULL, NULL, g_pSystem->RetrieveFullPath(g_sNotepadPath), sReportFile, sWorkDir, SW_SHOW);
		ShellExecute(NULL, NULL, sReportFile, NULL, sWorkDir, SW_SHOW);
	}
}

BOOL SourceVector::GetModuleName(CString& sModuleName)
{
	FILE* fp;
	{
		CString sFullName;
		sFullName.Format(_T("%s%s"), (LPCTSTR)m_Config.sProjectPath, (LPCTSTR)FullName());
		sFullName	= g_pSystem->RetrieveFullPath(sFullName);
		fp = _tfopen(sFullName, _T("rt"));
	}

	if(fp) {
		TCHAR	sLine[4096];
		LPCTSTR	sTag;
		{
			CString sExt(Name());
			int iPos = sExt.ReverseFind(_T('.'));

			if(iPos > 0) sExt.Delete(0, iPos + 1);

			sExt.MakeLower();

			if(!sExt.Compare(_T("v")) || !sExt.Compare(_T("sv"))) {
				// verilog
				sTag	= _T("module");
			} else {
				// vhdl?
				sTag	= _T("entity");
			}
		}

		while(_fgetts(sLine, 4096, fp)) {
			LPCTSTR	sText	= sLine;

			if(_tcsstr(sText, _T("/* TOP_MODULE")) == sText)
				sText	+= 13;
			else if(_tcsstr(sText, sTag) == sText)
				sText	+= 6;
			else continue;

			{
				CString	sToken(sText);
				int iPos	= 0;
				sModuleName	= sToken.Tokenize(_T(" \t;:#[]()*=\n\r"), iPos);
				fclose(fp);
				return (iPos > 0);
			}
		}

		fclose(fp);
	}

	return FALSE;
}

//------------------------------------------------------------------------------------------------------------
SourceGroup::SourceGroup(LPCTSTR sTargetPath, SourceGroup* pGroup)
{
	m_pParent	= pGroup;

	if(sTargetPath) m_sTargetPath	= sTargetPath;

	if(sTargetPath) {
		int iPos		= 0;
		m_sScorePath	= sTargetPath;

		// search profiling root path
		while(iPos >= 0) {
			CString	sSearchPath;
			sSearchPath.Format(_T("%s/") SOURCE_LIST_FILE_NAME, (LPCTSTR)m_sScorePath);
			sSearchPath	= RetrievePath(sSearchPath);

			if(!m_sScorePath.length() || !_taccess(sSearchPath, 0)) {
				sSearchPath.Format(_T("%s/") SCORE_FILE_NAME, (LPCTSTR)m_sScorePath);
				m_sScorePath	= RetrievePath(sSearchPath);
				break;
			}

			iPos	= m_sScorePath.ReverseFind(_T('/'));

			if(iPos >= 0) m_sScorePath.Delete(iPos, -1);
		}
	} else {
		m_sScorePath	= RetrievePath(SCORE_FILE_NAME);
	}

	Scan();
}

SourceGroup::~SourceGroup(void)
{
	ReleaseAll();
}

void SourceGroup::ReleaseAll(void)
{
	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++) delete(*i);

	m_GroupList.clear();

	for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++) delete(*i);

	m_SourceList.clear();
}

size_t SourceGroup::Size(void)
{
	size_t	iSize	= m_SourceList.size();

	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++)
		iSize	+= (*i)->Size();

	return iSize;
}

void SourceGroup::Scan(void)
{
	HANDLE				hFind;
	WIN32_FIND_DATA		FindFileData;
	CString				sFindPath		= g_pSystem->RetrieveFullPath(m_Config.sProjectPath);
	BOOL				bNoSource		= FALSE;
	ReleaseAll();

	if(sFindPath.IsEmpty()) return;

	sFindPath	+= TargetPath();
	{
		CString sNoSourcePath;
		sNoSourcePath.Format(_T("%s\\%s"), sFindPath, NO_SEARCH_NAME);

		if(_taccess(sNoSourcePath, 0) != -1) {
			bNoSource = TRUE;
			return;
		}
	}
	sFindPath	+= _T("\\*.*");
	hFind	= FindFirstFile(sFindPath, &FindFileData);

	if(hFind == INVALID_HANDLE_VALUE) return;

	do {
		// find sub directories
		if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && FindFileData.cFileName[0] != _T('.')) {
			CString	sNextSubPath;
			sNextSubPath 	= TargetPath();
			sNextSubPath	+= _T('/');
			sNextSubPath	+= FindFileData.cFileName;
			CString	sFilterSearch;
			sFilterSearch.Format(_T("%s/"), (LPCTSTR)sNextSubPath);

			if(!m_sNameFilter[0] || sFilterSearch.Find(m_sNameFilter) >= 0) {
				SourceGroup* pGroup	= new SourceGroup(sNextSubPath, this);

				if(!pGroup->Size()) delete pGroup;
				else {
					m_GroupList.push_back(pGroup);
				}
			}
		} else if(!bNoSource) {
			CString	sExt(FindFileData.cFileName);
			{
				int iPos = sExt.ReverseFind(_T('.'));

				if(iPos >= 0) sExt.Delete(0, iPos + 1);

				sExt.MakeLower();
			}

			if((!sExt.Compare(_T("v")) || !sExt.Compare(_T("sv")) || !sExt.Compare(_T("vhd")))) {
				CString	sFullPath(m_Config.sProjectPath);
				sFullPath	+= TargetPath();
				sFullPath	+= _T('/');
				sFullPath	+= FindFileData.cFileName;

				if(!CheckHidden(g_pSystem->RetrieveFullPath(sFullPath)))
					m_SourceList.push_back(new SourceVector(FindFileData.cFileName, this));
			}
		}
	} while(FindNextFile(hFind, &FindFileData));

	ResetData();
	FindClose(hFind);
}

BOOL SourceGroup::CheckHidden(LPCTSTR sFilePath)
{
	FILE* fp	= _tfopen(sFilePath, _T("rt"));

	if(fp) {
		TCHAR	sLine[4096];
		int		i = 0;

		while(_fgetts(sLine, 4096, fp) && i < 200) {
			if(_tcsstr(sLine, _T("/* HIDDEN */"))) return TRUE;

			if(_tcsstr(sLine, _T("module")) == sLine || _tcsstr(sLine, _T("entity")) == sLine) break;
		}

		fclose(fp);
	} else  return TRUE;	// can't open

	return FALSE;
}

void SourceGroup::MakeTable(void)
{
	if(IsRoot()) {
		m_pTable->NewHead();
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetSpan(TABLE_SPAN_COL, 2);
		m_pTable->SetBody(_T("<b>%s</b>"), _L(SYNTHESIS_TARGET));
		// speed
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<b>%s</b>"), _L(OPERATION_SPEED));
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<b>%s</b>"), _L(CLOCK_IO_DELAY));
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<b>%s</b>"), _L(COMBINATIONAL_DELAY));
		// area
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<b>Registers</b>"));
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<b>LUTs</b>"));
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<b>B/URAMs</b>"));
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 1, _T("solid #ccc"));
		m_pTable->SetBody(_T("<b>DSPs</b>"));
	}

	// local score table line
	if(m_SourceList.size()) {
		m_pTable->NewRow();
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_LEFT);
		m_pTable->SetBody(_T("<a href='folder:%s'><img src='document.png' title='%s' border='0'/></a> <a href='group:%s'>%s</a> (%d)"),
						  TargetPath(),
						  _L(OPEN_FOLDER),
						  TargetPath(), IsRoot() ? _T("/ (*ROOT*)") : TargetPath(),
						  (int)m_SourceList.size());
		m_pTable->NewCell(CELL_TH);
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetBody(_T("<a href='mark:%s'><u ONMOUSEOVER=\\\"popup('%s',300)\\\"; ONMOUSEOUT=\\\"kill()\\\"><img src='anchor.png' border='0'/></u></a>"),
						  TargetPath(),
						  _L(SET_MARK));
		// speed
		m_pTable->NewCell(CELL_TH, _T("frequency_%s"), TargetPath());
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->NewCell(CELL_TH, _T("hold_%s"), TargetPath());
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->NewCell(CELL_TH, _T("comb_%s"), TargetPath());
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		// area
		m_pTable->NewCell(CELL_TH, _T("regs_%s"), TargetPath());
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->NewCell(CELL_TH, _T("luts_%s"), TargetPath());
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->NewCell(CELL_TH, _T("brams_%s"), TargetPath());
		m_pTable->SetBoarderWidth(TABLE_BOARDER_RIGHT, 0);
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
		m_pTable->NewCell(CELL_TH, _T("dsps_%s"), TargetPath());
		m_pTable->SetTextAlignment(TABLE_ALIGN_CENTER);
	}

	// add sources
	for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++) {
		(*i)->MakeTable();
	}

	// add sub groups
	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++) {
		(*i)->MakeTable();
	}
}

void SourceGroup::SetMark(void)
{
	for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++)
		(*i)->SetMark();
}

void SourceGroup::ResetData(void)
{
	memset(&m_Info, 0, sizeof(m_Info));
	m_Info.timing.frequency		= -1;
	m_Info.timing.input_time	= -1;
	m_Info.timing.output_time	= -1;
	m_Info.timing.logic_delay	= -1;

	for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++) {
		const SYNTHESIS_DATA* pInfo	= (*i)->Info();

		if(!pInfo) continue;

		if(pInfo->timing.frequency > 0 && ((m_Info.timing.frequency < 0) || (pInfo->timing.frequency < m_Info.timing.frequency)))
			m_Info.timing.frequency	= pInfo->timing.frequency;

		if(pInfo->timing.input_time > m_Info.timing.input_time)
			m_Info.timing.input_time	= pInfo->timing.input_time;

		if(pInfo->timing.output_time > m_Info.timing.output_time)
			m_Info.timing.output_time	= pInfo->timing.output_time;

		if(pInfo->timing.logic_delay > m_Info.timing.logic_delay)
			m_Info.timing.logic_delay	= pInfo->timing.logic_delay;

		if(pInfo->area.regs.count > m_Info.area.regs.count) {
			m_Info.area.regs.count	= pInfo->area.regs.count;
			m_Info.area.regs.ratio	= pInfo->area.regs.ratio;
		}

		if(pInfo->area.luts.count > m_Info.area.luts.count) {
			m_Info.area.luts.count	= pInfo->area.luts.count;
			m_Info.area.luts.ratio	= pInfo->area.luts.ratio;
		}

		if(pInfo->area.brams.count > m_Info.area.brams.count) {
			m_Info.area.brams.count	= pInfo->area.brams.count;
			m_Info.area.brams.ratio	= pInfo->area.brams.ratio;
		}

		if(pInfo->area.urams.count > m_Info.area.urams.count) {
			m_Info.area.urams.count	= pInfo->area.urams.count;
			m_Info.area.urams.ratio	= pInfo->area.urams.ratio;
		}

		if(pInfo->area.dsps.count > m_Info.area.dsps.count) {
			m_Info.area.dsps.count	= pInfo->area.dsps.count;
			m_Info.area.dsps.ratio	= pInfo->area.dsps.ratio;
		}
	}
}

void SourceGroup::UpdateTable(BOOL bUpdateSubTable)
{
	// speed
	{
		CString sBody;

		// frequency
		if(m_Info.timing.frequency >= 0) {
			sBody.Format(_T("%.2f<font color='#9F9F9F'><small><small>MHz</small></small></font>"), m_Info.timing.frequency);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("frequency_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);

		// i/o hold time
		if(m_Info.timing.input_time >= 0) {
			sBody.Format(_T("%.2f<font color='#9F9F9F'><small><small>ns</small></small></font>"), m_Info.timing.input_time);
		} else {
			sBody	= sBLANK_FILED;
		}

		sBody	+= _T("<font color='#9F9F9F'> / </font>");

		if(m_Info.timing.output_time >= 0) {
			sBody.AppendFormat(_T("%.2f<font color='#9F9F9F'><small><small>ns</small></small></font>"), m_Info.timing.output_time);
		} else {
			sBody	+= sBLANK_FILED;
		}

		m_pTable->SetField(_T("hold_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);

		// combinational delay
		if(m_Info.timing.logic_delay >= 0) {
			sBody.Format(_T("%.3f<font color='#9F9F9F'><small><small>ns</small></small></font>"), m_Info.timing.logic_delay);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("comb_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);
	}
	// area
	{
		LPCTSTR		sRatiox2	= _T("%d<font color='#9F9F9F'><small><small> (%.2f%%)</small></small></font>");
		LPCTSTR		sRatiox1	= _T("%d<font color='#9F9F9F'><small><small> (%.1f%%)</small></small></font>");
		CString sBody;

		// registers
		if(m_Info.area.regs.count > 0) {
			sBody.Format(m_Info.area.regs.ratio < 10.f ? sRatiox2 : sRatiox1, m_Info.area.regs.count, m_Info.area.regs.ratio);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("regs_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);

		// luts
		if(m_Info.area.luts.count > 0) {
			sBody.Format(m_Info.area.luts.ratio < 10.f ? sRatiox2 : sRatiox1, m_Info.area.luts.count, m_Info.area.luts.ratio);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("luts_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);

		// brams / urams
		if(m_Info.area.brams.count > 0 || m_Info.area.urams.count > 0) {
			sBody.clear();
			sBody.AppendFormat(m_Info.area.brams.count ? _T("%d") : _T("-"), m_Info.area.brams.count);
			sBody.AppendFormat(_T("<font color='#9F9F9F'>/</font>"));
			sBody.AppendFormat(m_Info.area.urams.count ? _T("%d") : _T("-"), m_Info.area.urams.count);
			sBody.Append(_T("<font color='#9F9F9F'><small><small> ("));
			sBody.AppendFormat(m_Info.area.brams.ratio ? (m_Info.area.brams.ratio < 10.f ? _T("%.2f%%") : _T("%.1f%%")) : _T("-"), m_Info.area.brams.ratio);
			sBody.AppendFormat(_T("<font color='#9F9F9F'>/</font>"));
			sBody.AppendFormat(m_Info.area.urams.ratio ? (m_Info.area.urams.ratio < 10.f ? _T("%.2f%%") : _T("%.1f%%")) : _T("-"), m_Info.area.urams.ratio);
			sBody.Append(_T(")</small></small></font>"));
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("brams_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);

		// dsps
		if(m_Info.area.dsps.count > 0) {
			sBody.Format(m_Info.area.dsps.ratio < 10.f ? sRatiox2 : sRatiox1, m_Info.area.dsps.count, m_Info.area.dsps.ratio);
		} else {
			sBody	= sBLANK_FILED;
		}

		m_pTable->SetField(_T("dsps_%s"), _T("%s"), TargetPath(), (LPCTSTR)sBody);
	}

	if(bUpdateSubTable) {
		for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++)
			(*i)->UpdateTable(bUpdateSubTable);

		for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++)
			(*i)->UpdateTable();
	}
}

void SourceGroup::OpenFolder(LPCTSTR sPath)
{
	CString		sFolderPath	= g_pSystem->RetrieveFullPath(m_Config.sProjectPath);

	if(!sPath) sPath = TargetPath();

	sFolderPath	+= sPath;
	sFolderPath	+= _T('\\');
	ShellExecute(NULL, NULL, sFolderPath, NULL, NULL, SW_SHOW);
}

void SourceGroup::OpenSource(LPCTSTR sPath)
{
	CString		sSourcePath	= g_pSystem->RetrieveFullPath(m_Config.sProjectPath);
	sSourcePath	+= sPath;
	ShellExecute(NULL, _T("open"), g_pSystem->RetrieveFullPath(g_sNotepadPath), (LPCTSTR)sSourcePath, NULL, SW_SHOW);
}

SourceGroup* SourceGroup::FindGroup(LPCTSTR sPath)
{
	if(!m_sTargetPath.Compare(sPath)) return this;

	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++) {
		SourceGroup* pGroup	= (*i)->FindGroup(sPath);

		if(pGroup) return pGroup;
	}

	return NULL;
}

SourceVector* SourceGroup::FindVector(LPCTSTR sPath)
{
	for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++) {
		if(!(*i)->FullName().Compare(sPath)) return (*i);
	}

	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++) {
		SourceVector* pVector	= (*i)->FindVector(sPath);

		if(pVector) return pVector;
	}

	return NULL;
}

SourceVector* SourceGroup::GetNextSource(SourceVector* pCurrent)
{
	if(!pCurrent) {
		if(!m_SourceList.size()) return NULL;

		return m_SourceList.front();
	} else {
		for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++) {
			if((*i) == pCurrent) {
				i++;
				return (i == m_SourceList.end()) ? NULL : (*i);
			}
		}
	}

	return NULL;
}

void SourceGroup::RemoveScoreFile(void)
{
	_tremove(ScorePath());

	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++)
		(*i)->RemoveScoreFile();
}

void SourceGroup::FlushScoreFile(void)
{
	for(auto i = m_SourceList.begin(); i != m_SourceList.end(); i++)
		(*i)->UpdateProfile(FALSE);

	for(auto i = m_GroupList.begin(); i != m_GroupList.end(); i++)
		(*i)->FlushScoreFile();
}
