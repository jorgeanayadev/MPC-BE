/*
 * (C) 2006-2025 see Authors.txt
 *
 * This file is part of MPC-BE.
 *
 * MPC-BE is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-BE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "LoopEditDlg.h"
#include "MainFrm.h"

// CLoopEditDlg dialog

IMPLEMENT_DYNAMIC(CLoopEditDlg, CDialog)

CLoopEditDlg::CLoopEditDlg(REFERENCE_TIME startTime, REFERENCE_TIME endTime, CWnd* pParent)
	: CDialog(CLoopEditDlg::IDD, pParent)
	, m_startTime(startTime)
	, m_endTime(endTime)
	, m_name(L"New Loop")
{
	UpdateTimeStrings();
}

void CLoopEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOOP_NAME, m_name);
	DDX_Text(pDX, IDC_START_TIME, m_startTimeStr);
	DDX_Text(pDX, IDC_END_TIME, m_endTimeStr);
}

BEGIN_MESSAGE_MAP(CLoopEditDlg, CDialog)
	ON_BN_CLICKED(IDC_CURRENT_POS, OnBnClickedCurrentPos)
	ON_BN_CLICKED(IDC_CURRENT_POS_END, OnBnClickedCurrentPosEnd)
END_MESSAGE_MAP()

BOOL CLoopEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;
}

void CLoopEditDlg::OnOK()
{
	UpdateData(TRUE);

	// Validate and parse time strings
	REFERENCE_TIME startTime, endTime;
	if (!ParseTimeString(m_startTimeStr, startTime)) {
		AfxMessageBox(L"Invalid start time format. Use HH:MM:SS or HH:MM:SS.mmm", MB_ICONERROR);
		return;
	}

	if (!ParseTimeString(m_endTimeStr, endTime)) {
		AfxMessageBox(L"Invalid end time format. Use HH:MM:SS or HH:MM:SS.mmm", MB_ICONERROR);
		return;
	}

	if (startTime >= endTime) {
		AfxMessageBox(L"Start time must be less than end time.", MB_ICONERROR);
		return;
	}

	if (m_name.IsEmpty()) {
		AfxMessageBox(L"Loop name cannot be empty.", MB_ICONERROR);
		return;
	}

	m_startTime = startTime;
	m_endTime = endTime;

	CDialog::OnOK();
}

void CLoopEditDlg::OnBnClickedCurrentPos()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrame && pMainFrame->m_pMS) {
		REFERENCE_TIME currentPos;
		if (SUCCEEDED(pMainFrame->m_pMS->GetCurrentPosition(&currentPos))) {
			m_startTime = currentPos;
			UpdateTimeStrings();
			UpdateData(FALSE);
		}
	}
}

void CLoopEditDlg::OnBnClickedCurrentPosEnd()
{
	CMainFrame* pMainFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	if (pMainFrame && pMainFrame->m_pMS) {
		REFERENCE_TIME currentPos;
		if (SUCCEEDED(pMainFrame->m_pMS->GetCurrentPosition(&currentPos))) {
			m_endTime = currentPos;
			UpdateTimeStrings();
			UpdateData(FALSE);
		}
	}
}

void CLoopEditDlg::UpdateTimeStrings()
{
	m_startTimeStr = ReftimeToString(m_startTime, false);
	m_endTimeStr = ReftimeToString(m_endTime, false);
}

bool CLoopEditDlg::ParseTimeString(const CString& timeStr, REFERENCE_TIME& time)
{
	// Parse time strings in format HH:MM:SS or HH:MM:SS.mmm
	CString str = timeStr;
	str.Trim();

	int hours = 0, minutes = 0, seconds = 0, milliseconds = 0;
	
	// Check for milliseconds
	int dotPos = str.ReverseFind('.');
	if (dotPos > 0) {
		CString msStr = str.Mid(dotPos + 1);
		if (msStr.GetLength() > 3) msStr = msStr.Left(3);
		while (msStr.GetLength() < 3) msStr += L"0";
		milliseconds = _wtoi(msStr);
		str = str.Left(dotPos);
	}

	// Parse HH:MM:SS
	int count = swscanf_s(str, L"%d:%d:%d", &hours, &minutes, &seconds);
	if (count != 3) {
		return false;
	}

	if (hours < 0 || minutes < 0 || minutes >= 60 || seconds < 0 || seconds >= 60 || milliseconds < 0 || milliseconds >= 1000) {
		return false;
	}

	time = (REFERENCE_TIME)(hours * 3600 + minutes * 60 + seconds) * UNITS + milliseconds * (UNITS / 1000);
	return true;
}