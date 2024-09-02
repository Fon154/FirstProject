
// SameGameView.cpp: реализация класса CSameGameView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SameGame.h"
#endif

#include "SameGameDoc.h"
#include "SameGameView.h"
#include "OptionDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CSameGameView

IMPLEMENT_DYNCREATE(CSameGameView, CView)

BEGIN_MESSAGE_MAP(CSameGameView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()


	ON_COMMAND(ID_32772, &CSameGameView::On32772)
	ON_COMMAND(ID_32773, &CSameGameView::On32773)
	ON_COMMAND(ID_32774, &CSameGameView::On32774)
	ON_COMMAND(ID_32775, &CSameGameView::On32775)
	ON_COMMAND(ID_32776, &CSameGameView::On32776)
	ON_UPDATE_COMMAND_UI(ID_32772, &CSameGameView::OnUpdate32772)
	ON_UPDATE_COMMAND_UI(ID_32773, &CSameGameView::OnUpdate32773)
	ON_UPDATE_COMMAND_UI(ID_32774, &CSameGameView::OnUpdate32774)
	ON_UPDATE_COMMAND_UI(ID_32775, &CSameGameView::OnUpdate32775)
	ON_UPDATE_COMMAND_UI(ID_32776, &CSameGameView::OnUpdate32776)

//	ON_COMMAND(ID_32777, &CSameGameView::On32777)
ON_COMMAND(ID_32777, &CSameGameView::On32777)
ON_COMMAND(ID_32778, &CSameGameView::On32778)
ON_COMMAND(ID_32771, &CSameGameView::On32771)
ON_UPDATE_COMMAND_UI(ID_32771, &CSameGameView::OnUpdate32771)
ON_COMMAND(ID_EDIT_UNDO, &CSameGameView::OnEditUndo)
ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CSameGameView::OnUpdateEditUndo)
END_MESSAGE_MAP()

// Создание или уничтожение CSameGameView

CSameGameView::CSameGameView() 
{}

CSameGameView::~CSameGameView()
{}

BOOL CSameGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CView::PreCreateWindow(cs);
}

// Рисование CSameGameView

void CSameGameView::OnDraw(CDC* pDC)
{
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	//сохраняем текущее состояние контекста устройства
	int nDCSave = pDC->SaveDC();

	//получаем размеры клиентской области
	CRect rcClient;
	GetClientRect(&rcClient);

	//получаем фоновый цвет доски
	COLORREF clr = pDoc->GetBoardSpace(-1, -1);

	//сначала отрисовываем фон
	pDC->FillSolidRect(&rcClient, clr);

	//создаем кисть для рисования
	CBrush br;
	br.CreateStockObject(HOLLOW_BRUSH);
	CBrush* pbrOld = pDC->SelectObject(&br);

	//рисуем блоки
	for (int row = 0; row < pDoc->GetRows(); row++)
	{
		for (int col = 0; col < pDoc->GetColumns(); col++)
		{
			// Получаем цвет для пространства доски
			clr = pDoc->GetBoardSpace(row, col);

			// Рассчитываем размер и положение пространства доски
			CRect rcBlock;
			rcBlock.top = row * pDoc->GetHeight();
			rcBlock.left = col * pDoc->GetWidth();
			rcBlock.right = rcBlock.left + pDoc->GetWidth();
			rcBlock.bottom = rcBlock.top + pDoc->GetHeight();

			// Заполняем блок правильным цветом
			pDC->FillSolidRect(&rcBlock, clr);

			// Рисуем контур блока
			pDC->Rectangle(&rcBlock);
		}
	}
	//восстанавливаем контекст устройства
	pDC->RestoreDC(nDCSave);
	br.DeleteObject();
}


// Диагностика CSameGameView

#ifdef _DEBUG
void CSameGameView::AssertValid() const
{
	CView::AssertValid();
}

void CSameGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSameGameDoc* CSameGameView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSameGameDoc)));
	return (CSameGameDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CSameGameView

void CSameGameView::OnInitialUpdate() {
	CView::OnInitialUpdate();

	//изменяем размер окна
	ResizeWindow();
}

void CSameGameView::ResizeWindow() {
	//создаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//получаем размер клиентской области и окна
	CRect rcClient, rcWindow;
	GetClientRect(&rcClient);
	GetParentFrame()->GetWindowRect(&rcWindow);

	//вычисляем разницу
	int nWidthDiff = rcWindow.Width() - rcClient.Width();
	int nHeightDiff = rcWindow.Height() - rcClient.Height();

	// Изменяем размеры окна, исходя из размеров нашей доски
	rcWindow.right = rcWindow.left +
		pDoc->GetWidth() * pDoc->GetColumns() + nWidthDiff;
	rcWindow.bottom = rcWindow.top + 
		pDoc->GetHeight() * pDoc->GetRows() + nHeightDiff;

	// Функция MoveWindow() изменяет размер окна фрейма
	GetParentFrame()->MoveWindow(&rcWindow);
}

void CSameGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//вначале создаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	/*получаем индекс строки и столбца элемента, по которому был
	осуществлен клик мышкой*/
	int row = point.y / pDoc->GetHeight();
	int col = point.x / pDoc->GetWidth();

	//удаляем блоки из Document 
	int count = pDoc->DeleteBlocks(row, col);

	//проверяем, было ли удаление блоков
	if (count > 0) 
	{
		//перерисовываем View
		Invalidate();
		UpdateWindow();

		//проверяем закончилась ли игра
		if (pDoc->IsGameOver())
		{
			//получаем кол-во оставшихся блоков
			int remaining = pDoc->GetRemainingCount();
			CString message;
			message.Format(_T("No more moves left\nBlocks"
				" remaining: %d"), remaining);

			//отображаем пользователю результат игры
			MessageBox(message, _T("Game Over"), 
				MB_OK | MB_ICONINFORMATION);
		}
	}
	//OnLButtonDown по умолчанию
	CView::OnLButtonDown(nFlags, point);
}

void CSameGameView::setColorCount(int numColors)
{
	//сначала получаем указатель на документ
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	//устанавливаем количество цветов 
	pDoc->SetNumColors(numColors);
	//перерисовываем View
	Invalidate();
	UpdateWindow();
}

void CSameGameView::On32772()
{
	setColorCount(3);
}


void CSameGameView::OnUpdate32772(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 3);
}


void CSameGameView::On32773()
{
	setColorCount(4);
}


void CSameGameView::OnUpdate32773(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 4);
}


void CSameGameView::On32774()
{
	setColorCount(5);
}


void CSameGameView::OnUpdate32774(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 5);
}


void CSameGameView::On32775()
{
	setColorCount(6);
}


void CSameGameView::OnUpdate32775(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 6);
}


void CSameGameView::On32776()
{
	setColorCount(7);
}


void CSameGameView::OnUpdate32776(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Проверка установленного уровня сложности
	pCmdUI->SetCheck(pDoc->GetNumColors() == 7);
}

void CSameGameView::On32777()
{
	OnSetupBlockcount();
}

void CSameGameView::On32778()
{
	OnSetupBlocksize();
}

void CSameGameView::OnSetupBlockcount()
{
	// Получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Создаем диалоговое окно
	COptionDialog dlg(true, this);

	// Устанавливаем параметры строк и столбцов
	dlg.m_nValue1 = pDoc->GetRows();
	dlg.m_nValue2 = pDoc->GetColumns();

	// Отображаем полученное окно
	if (dlg.DoModal() == IDOK)
	{
		// Сначала удаляем игровое поле
		pDoc->DeleteBoard();

		// Устанавливаем значения, переданные пользователем
		pDoc->SetRows(dlg.m_nValue1);
		pDoc->SetColumns(dlg.m_nValue2);

		// Обновляем игровое поле
		pDoc->SetupBoard();

		// Изменяем размеры View
		ResizeWindow();
	}
}

void CSameGameView::OnSetupBlocksize()
{
	// Указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Создаем диалоговое окно
	COptionDialog dlg(false, this);

	// Устанавливаем параметры «Ширины/Высоты»
	dlg.m_nValue1 = pDoc->GetWidth();
	dlg.m_nValue2 = pDoc->GetHeight();

	// Отображаем окно
	if (dlg.DoModal() == IDOK)
	{
		// Удаляем игровое поле
		pDoc->DeleteBoard();

		// Считываем введенные пользователем параметры
		pDoc->SetWidth(dlg.m_nValue1);
		pDoc->SetHeight(dlg.m_nValue2);

		// Обновляем игровую доску
		pDoc->SetupBoard();

		// Изменяем размеры View
		ResizeWindow();
	}
}


void CSameGameView::OnEditUndo()
{
		// Получаем указатель на Document
		CSameGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
		pDoc->UndoLast();

		// Перерисовываем View
		Invalidate();
		UpdateWindow();
	}

	void CSameGameView::On32771()
	{
		// Получаем указатель на Document
		CSameGameDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;
		pDoc->RedoLast();

		// Перерисовываем View
		Invalidate();
		UpdateWindow();
}


void CSameGameView::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Включаем опцию, если она доступна
	pCmdUI->Enable(pDoc->CanUndo());
}

void CSameGameView::OnUpdate32771(CCmdUI* pCmdUI)
{
	// Сначала получаем указатель на Document
	CSameGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Включаем опцию, если она доступна
	pCmdUI->Enable(pDoc->CanRedo());
}
