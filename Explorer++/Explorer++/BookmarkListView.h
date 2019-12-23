// Copyright (C) Explorer++ Project
// SPDX-License-Identifier: GPL-3.0-only
// See LICENSE in the top level directory

#pragma once

#include "BookmarkHelper.h"
#include "BookmarkItem.h"
#include "BookmarkTree.h"
#include "CoreInterface.h"
#include "ResourceHelper.h"
#include "../Helper/DpiCompatibility.h"
#include "../Helper/WindowSubclassWrapper.h"
#include <wil/resource.h>
#include <optional>

class CBookmarkListView
{
public:

	enum class ColumnType
	{
		Name = 1,
		Location = 2,
		DateCreated = 3,
		DateModified = 4
	};

	struct Column
	{
		ColumnType columnType;
		int width;
		bool active;
	};

	CBookmarkListView(HWND hListView, HMODULE resourceModule, BookmarkTree *bookmarkTree,
		IExplorerplusplus *expp, const std::vector<Column> &initialColumns);

	void NavigateToBookmarkFolder(BookmarkItem *bookmarkItem);
	BookmarkItem *GetBookmarkItemFromListView(int iItem);
	BookmarkItem *GetBookmarkItemFromListViewlParam(LPARAM lParam);

	std::vector<Column> GetColumns();

private:

	static const UINT_PTR PARENT_SUBCLASS_ID = 0;

	static LRESULT CALLBACK ParentWndProcStub(HWND hwnd, UINT uMsg, WPARAM wParam,
		LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	LRESULT CALLBACK ParentWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void InsertColumns(const std::vector<Column> &columns);
	void InsertColumn(const Column &column, int index);
	std::wstring GetColumnText(ColumnType columnType);
	UINT GetColumnTextResourceId(ColumnType columnType);
	static bool IsColumnActive(const Column &column);
	std::optional<ColumnType> GetColumnTypeByIndex(int index) const;

	int InsertBookmarkItemIntoListView(BookmarkItem *bookmarkItem, int position);
	std::wstring GetBookmarkItemColumnInfo(const BookmarkItem *bookmarkItem, ColumnType columnType);
	static std::wstring FormatDate(const FILETIME *date);

	void OnRClick(const NMITEMACTIVATE *itemActivate);
	void OnGetDispInfo(NMLVDISPINFO *dispInfo);
	BOOL OnBeginLabelEdit(const NMLVDISPINFO *dispInfo);
	BOOL OnEndLabelEdit(const NMLVDISPINFO *dispInfo);
	void OnKeyDown(const NMLVKEYDOWN *keyDown);
	void OnRename();

	void OnHeaderRClick(const POINT &pt);
	wil::unique_hmenu BuildHeaderContextMenu();
	void OnHeaderContextMenuItemSelected(int menuItemId);

	HWND m_hListView;
	HMODULE m_resourceModule;
	DpiCompatibility m_dpiCompat;
	wil::unique_himagelist m_imageList;
	IconImageListMapping m_imageListMappings;
	std::vector<Column> m_columns;

	BookmarkTree *m_bookmarkTree;
	BookmarkItem *m_currentBookmarkFolder;

	std::vector<WindowSubclassWrapper> m_windowSubclasses;
};