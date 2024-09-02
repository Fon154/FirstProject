
// SameGameView.h: интерфейс класса CSameGameView
//

#pragma once


class CSameGameView : public CView
{
protected: // создать только из сериализации
	CSameGameView();
	DECLARE_DYNCREATE(CSameGameView)

// Атрибуты
public:
	CSameGameDoc* GetDocument() const;

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Реализация
public:
	
	void ResizeWindow();

	virtual ~CSameGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void OnInitialUpdate();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	//функции для изменения уровня сложности
	afx_msg void On32772();
	afx_msg void On32773();
	afx_msg void On32774();
	afx_msg void On32775();
	afx_msg void On32776();

	//функции для обновления меню
	afx_msg void OnUpdate32772(CCmdUI* pCmdUI);
	afx_msg void OnUpdate32773(CCmdUI* pCmdUI);
	afx_msg void OnUpdate32774(CCmdUI* pCmdUI);
	afx_msg void OnUpdate32775(CCmdUI* pCmdUI);
	afx_msg void OnUpdate32776(CCmdUI* pCmdUI);

	afx_msg void OnSetupBlockcount();
	afx_msg void OnSetupBlocksize();

public:
	void setColorCount(int numColors);
	afx_msg void On32777();
	afx_msg void On32778();

	// Функции для меню «Отмена/Повтор»
	afx_msg void OnEditUndo();
	afx_msg void On32771();

	// Функции для обновления меню «Отмена/Повтор»
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdate32771(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // версия отладки в SameGameView.cpp
inline CSameGameDoc* CSameGameView::GetDocument() const
   { return reinterpret_cast<CSameGameDoc*>(m_pDocument); }
#endif

