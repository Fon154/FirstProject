#pragma once

#include "CSameGameBoard.h"
#include <stack>

class CSameGameDoc : public CDocument
{
protected:
	CSameGameDoc() noexcept;
	DECLARE_DYNCREATE(CSameGameDoc)

	// Операции
public:
	// Функции доступа к игровой доске 
	COLORREF GetBoardSpace(int row, int col) { return m_board->GetBoardSpace(row, col); }
	void SetupBoard(void) { m_board->SetupBoard(); }

	int GetWidth(void) { return m_board->GetWidth(); }
	void SetWidth(int nWidth) { m_board->SetWidth(nWidth); }

	int GetHeight(void) { return m_board->GetHeight(); }
	void SetHeight(int nHeight) { m_board->SetHeight(nHeight); }

	int GetColumns(void) { return m_board->GetColumns(); }
	void SetColumns(int nColumns) { m_board->SetColumns(nColumns); }

	int GetRows(void) { return m_board->GetRows(); }
	void SetRows(int nRows) { m_board->SetRows(nRows); }

	void DeleteBoard(void) { m_board->DeleteBoard(); }
	bool IsGameOver() { return m_board->IsGameOver(); }

	int DeleteBlocks(int row, int col);
	int GetRemainingCount() { return m_board->GetRemainingCount(); }
	int GetNumColors() { return m_board->GetNumColors(); }
	void SetNumColors(int nColors);




	// Переопределения
	virtual BOOL OnNewDocument();
public:
	void UndoLast();
	void RedoLast();

public:
	bool CanUndo();
	bool CanRedo();


	// Реализация
public:
	virtual ~CSameGameDoc();
#ifdef _DEBUG
#endif

protected:
	// Функции очистки стеков «Отмена/Повтор»
	void ClearUndo();
	void ClearRedo();

	// Экземпляр класса игровой доски. Теперь мы сделали его указателем на класс
	CSameGameBoard* m_board;

	// Стек "Отмена"
	std::stack<CSameGameBoard*> m_undo;

	// Стек "Повтор"
	std::stack<CSameGameBoard*> m_redo;

	// Генерация функции сообщений
protected:
	DECLARE_MESSAGE_MAP()

};