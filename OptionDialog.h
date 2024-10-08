﻿#pragma once
#include "afxdialogex.h"


// Диалоговое окно COptionDialog
class COptionDialog : public CDialog
{
	DECLARE_DYNAMIC(COptionDialog)

public:
	// Стандартный конструктор
	COptionDialog(bool bRowColumn, CWnd* pParent = nullptr);
	virtual ~COptionDialog();

	// Данные диалогового окна
	enum { IDD = IDD_DIALOG_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // поддержка DDX/DDV
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctrlStaticText1;
	CStatic m_ctrlStaticText2;
	int m_nValue1;
	int m_nValue2;
	afx_msg void OnBnClickedButtonDefaults();
	virtual BOOL OnInitDialog();
private:
	// Является ли это диалоговое окно для строки/столбца (true) или для ширины/высоты (false)?
	bool m_bRowColumnDialog;
};