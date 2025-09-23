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

#pragma once

#include "DSUtil/DSUtil.h"

// CLoopEditDlg dialog

class CLoopEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CLoopEditDlg)

public:
	CLoopEditDlg(REFERENCE_TIME startTime = 0, REFERENCE_TIME endTime = 0, CWnd* pParent = nullptr);
	virtual ~CLoopEditDlg() = default;

	enum { IDD = IDD_LOOP_EDIT_DLG };

	CString GetName() const { return m_name; }
	void SetName(const CString& name) { m_name = name; }
	REFERENCE_TIME GetStartTime() const { return m_startTime; }
	REFERENCE_TIME GetEndTime() const { return m_endTime; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	CString m_name;
	CString m_startTimeStr;
	CString m_endTimeStr;
	REFERENCE_TIME m_startTime;
	REFERENCE_TIME m_endTime;

	afx_msg void OnBnClickedCurrentPos();
	afx_msg void OnBnClickedCurrentPosEnd();

	void UpdateTimeStrings();
	bool ParseTimeString(const CString& timeStr, REFERENCE_TIME& time);
};