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
#include "MainFrm.h"
#include "PlayerLoopBar.h"
#include "LoopEditDlg.h"
#include "DSUtil/SysVersion.h"
#include "DSUtil/DSUtil.h"

// CPlayerLoopBar

IMPLEMENT_DYNAMIC(CPlayerLoopBar, CPlayerBar)

CPlayerLoopBar::CPlayerLoopBar(CMainFrame* pMainFrame)
	: m_pMainFrame(pMainFrame)
	, m_currentLoop(-1)
{
}

BOOL CPlayerLoopBar::Create(CWnd* pParentWnd, UINT defDockBarID)
{
	if (!__super::Create(L"Loop Manager", pParentWnd, ID_VIEW_LOOPBAR, defDockBarID, L"Loop Manager")) {
		return FALSE;
	}

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER);

	CRect rect;
	GetClientRect(&rect);
	
	// Create list control
	m_list.Create(WS_VISIBLE | WS_CHILD | LVS_REPORT | LVS_AUTOARRANGE | LVS_NOSORTHEADER, 
		CRect(0, 0, rect.Width(), rect.Height() - 30), this, IDC_LOOP_LIST);

	// Add columns
	m_list.InsertColumn(COL_NAME, L"Name", LVCFMT_LEFT, 120);
	m_list.InsertColumn(COL_START, L"Start", LVCFMT_LEFT, 80);
	m_list.InsertColumn(COL_END, L"End", LVCFMT_LEFT, 80);
	m_list.InsertColumn(COL_DURATION, L"Duration", LVCFMT_LEFT, 80);

	// Create buttons
	int btnWidth = 60;
	int btnHeight = 25;
	int btnY = rect.Height() - 28;
	
	m_addBtn.Create(L"Add", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
		CRect(5, btnY, 5 + btnWidth, btnY + btnHeight), this, IDC_ADD_LOOP);
	
	m_removeBtn.Create(L"Remove", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
		CRect(70, btnY, 70 + btnWidth, btnY + btnHeight), this, IDC_REMOVE_LOOP);
	
	m_editBtn.Create(L"Edit", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
		CRect(135, btnY, 135 + btnWidth, btnY + btnHeight), this, IDC_EDIT_LOOP);
	
	m_exportBtn.Create(L"Export", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 
		CRect(200, btnY, 200 + btnWidth, btnY + btnHeight), this, IDC_EXPORT_LOOP);

	return TRUE;
}

void CPlayerLoopBar::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOOP_LIST, m_list);
}

BOOL CPlayerLoopBar::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!__super::PreCreateWindow(cs)) {
		return FALSE;
	}

	cs.dwExStyle |= WS_EX_ACCEPTFILES;

	return TRUE;
}

BOOL CPlayerLoopBar::PreTranslateMessage(MSG* pMsg)
{
	if (IsWindow(pMsg->hwnd) && IsVisible() && pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) {
		if (IsDialogMessage(pMsg)) {
			return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CPlayerLoopBar, CPlayerBar)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_ADD_LOOP, OnAddLoop)
	ON_BN_CLICKED(IDC_REMOVE_LOOP, OnRemoveLoop)
	ON_BN_CLICKED(IDC_EDIT_LOOP, OnEditLoop)
	ON_BN_CLICKED(IDC_EXPORT_LOOP, OnExportLoop)
	ON_NOTIFY(NM_DBLCLK, IDC_LOOP_LIST, OnNMDblclkList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LOOP_LIST, OnLvnKeydownList)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void CPlayerLoopBar::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	Resize();
}

void CPlayerLoopBar::Resize()
{
	if (::IsWindow(m_list.GetSafeHwnd())) {
		CRect rect;
		GetClientRect(&rect);
		
		m_list.SetWindowPos(nullptr, 0, 0, rect.Width(), rect.Height() - 30, 
			SWP_NOZORDER | SWP_NOACTIVATE);

		int btnWidth = 60;
		int btnHeight = 25;
		int btnY = rect.Height() - 28;
		
		m_addBtn.SetWindowPos(nullptr, 5, btnY, btnWidth, btnHeight, SWP_NOZORDER | SWP_NOACTIVATE);
		m_removeBtn.SetWindowPos(nullptr, 70, btnY, btnWidth, btnHeight, SWP_NOZORDER | SWP_NOACTIVATE);
		m_editBtn.SetWindowPos(nullptr, 135, btnY, btnWidth, btnHeight, SWP_NOZORDER | SWP_NOACTIVATE);
		m_exportBtn.SetWindowPos(nullptr, 200, btnY, btnWidth, btnHeight, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

HBRUSH CPlayerLoopBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (*pWnd == m_list) {
		hbr = GetStockBrush(WHITE_BRUSH);
	}

	return hbr;
}

void CPlayerLoopBar::Clear()
{
	m_loops.clear();
	m_currentLoop = -1;
	UpdateLoopList();
}

void CPlayerLoopBar::AddLoop(const CString& name, REFERENCE_TIME start, REFERENCE_TIME end)
{
	m_loops.emplace_back(name, start, end);
	UpdateLoopList();
}

void CPlayerLoopBar::UpdateLoopList()
{
	m_list.DeleteAllItems();
	
	for (size_t i = 0; i < m_loops.size(); i++) {
		const auto& loop = m_loops[i];
		
		int item = m_list.InsertItem((int)i, loop.name);
		m_list.SetItemText(item, COL_START, ReftimeToString2(loop.startTime));
		m_list.SetItemText(item, COL_END, ReftimeToString2(loop.endTime));
		m_list.SetItemText(item, COL_DURATION, ReftimeToString2(loop.endTime - loop.startTime));
		
		if ((int)i == m_currentLoop) {
			m_list.SetItemState(item, LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

void CPlayerLoopBar::OnAddLoop()
{
	REFERENCE_TIME currentPos = 0;
	if (m_pMainFrame->m_pMS) {
		m_pMainFrame->m_pMS->GetCurrentPosition(&currentPos);
	}
	
	CLoopEditDlg dlg(currentPos, currentPos + 10 * UNITS, this);
	if (dlg.DoModal() == IDOK) {
		AddLoop(dlg.GetName(), dlg.GetStartTime(), dlg.GetEndTime());
	}
}

void CPlayerLoopBar::OnRemoveLoop()
{
	int sel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (sel >= 0 && sel < (int)m_loops.size()) {
		m_loops.erase(m_loops.begin() + sel);
		if (m_currentLoop == sel) {
			m_currentLoop = -1;
		} else if (m_currentLoop > sel) {
			m_currentLoop--;
		}
		UpdateLoopList();
	}
}

void CPlayerLoopBar::OnEditLoop()
{
	int sel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (sel >= 0 && sel < (int)m_loops.size()) {
		auto& loop = m_loops[sel];
		CLoopEditDlg dlg(loop.startTime, loop.endTime, this);
		dlg.SetName(loop.name);
		
		if (dlg.DoModal() == IDOK) {
			loop.name = dlg.GetName();
			loop.startTime = dlg.GetStartTime();
			loop.endTime = dlg.GetEndTime();
			UpdateLoopList();
		}
	}
}

void CPlayerLoopBar::OnExportLoop()
{
	int sel = m_list.GetNextItem(-1, LVNI_SELECTED);
	if (sel >= 0 && sel < (int)m_loops.size()) {
		const auto& loop = m_loops[sel];
		m_pMainFrame->ExportLoop(loop.name, loop.startTime, loop.endTime);
	}
}

void CPlayerLoopBar::OnNMDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->iItem >= 0 && pNMLV->iItem < (int)m_loops.size()) {
		GotoLoop(pNMLV->iItem);
	}
	
	*pResult = 0;
}

void CPlayerLoopBar::OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	if (pLVKeyDow->wVKey == VK_DELETE) {
		OnRemoveLoop();
	} else if (pLVKeyDow->wVKey == VK_RETURN) {
		int sel = m_list.GetNextItem(-1, LVNI_SELECTED);
		if (sel >= 0) {
			GotoLoop(sel);
		}
	}
	
	*pResult = 0;
}

void CPlayerLoopBar::GotoLoop(int index)
{
	if (index >= 0 && index < (int)m_loops.size()) {
		const auto& loop = m_loops[index];
		m_currentLoop = index;
		
		if (m_pMainFrame->m_pMS) {
			m_pMainFrame->m_pMS->SetPositions(&loop.startTime, AM_SEEKING_AbsolutePositioning,
				nullptr, AM_SEEKING_NoPositioning);
		}
		
		UpdateLoopList();
	}
}

bool CPlayerLoopBar::IsInLoop(REFERENCE_TIME currentTime, int& loopIndex)
{
	for (size_t i = 0; i < m_loops.size(); i++) {
		const auto& loop = m_loops[i];
		if (loop.enabled && currentTime >= loop.startTime && currentTime <= loop.endTime) {
			loopIndex = (int)i;
			return true;
		}
	}
	loopIndex = -1;
	return false;
}

const LoopEntry* CPlayerLoopBar::GetLoop(int index) const
{
	if (index >= 0 && index < (int)m_loops.size()) {
		return &m_loops[index];
	}
	return nullptr;
}

void CPlayerLoopBar::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	__super::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CPlayerLoopBar::SaveLoops()
{
	// TODO: Implement saving loops to profile/settings
}

void CPlayerLoopBar::LoadLoops()
{
	// TODO: Implement loading loops from profile/settings
}