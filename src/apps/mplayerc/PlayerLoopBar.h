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

#include "PlayerBar.h"
#include "PlayerListCtrl.h"

struct LoopEntry {
	CString name;
	REFERENCE_TIME startTime;
	REFERENCE_TIME endTime;
	bool enabled;

	LoopEntry() : startTime(0), endTime(0), enabled(true) {}
	LoopEntry(const CString& _name, REFERENCE_TIME _start, REFERENCE_TIME _end)
		: name(_name), startTime(_start), endTime(_end), enabled(true) {}
};

class CMainFrame;

// CPlayerLoopBar

class CPlayerLoopBar : public CPlayerBar
{
	DECLARE_DYNAMIC(CPlayerLoopBar)

private:
	enum {COL_NAME, COL_START, COL_END, COL_DURATION};

	CImageList m_fakeImageList;
	CPlayerListCtrl m_list;
	CButton m_addBtn;
	CButton m_removeBtn;
	CButton m_exportBtn;
	CButton m_editBtn;

	CMainFrame* m_pMainFrame;

	std::vector<LoopEntry> m_loops;
	int m_currentLoop;

	void Resize();
	void AddNewLoop();
	void RemoveSelectedLoop();
	void EditSelectedLoop();
	void ExportSelectedLoop();

public:
	CPlayerLoopBar(CMainFrame* pMainFrame);
	virtual ~CPlayerLoopBar() = default;

	BOOL Create(CWnd* pParentWnd, UINT defDockBarID);

	void Clear();
	void SaveLoops();
	void LoadLoops();

	void AddLoop(const CString& name, REFERENCE_TIME start, REFERENCE_TIME end);
	void UpdateLoopList();
	void GotoLoop(int index);
	bool IsInLoop(REFERENCE_TIME currentTime, int& loopIndex);
	void SetCurrentLoop(int index) { m_currentLoop = index; }
	int GetCurrentLoop() const { return m_currentLoop; }
	size_t GetLoopCount() const { return m_loops.size(); }
	const LoopEntry* GetLoop(int index) const;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLbnSelchange();
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddLoop();
	afx_msg void OnRemoveLoop();
	afx_msg void OnEditLoop();
	afx_msg void OnExportLoop();
	afx_msg void OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};