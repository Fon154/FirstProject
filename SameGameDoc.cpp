
// SameGameDoc.cpp: реализация класса CSameGameDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SameGame.h"
#endif

#include "SameGameDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSameGameDoc
IMPLEMENT_DYNCREATE(CSameGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CSameGameDoc, CDocument)
END_MESSAGE_MAP()


// Создание/уничтожение CSameGameDoc
CSameGameDoc::CSameGameDoc()noexcept
{
    // Здесь всегда должна быть игровая доска
    m_board = new CSameGameBoard();
}

CSameGameDoc::~CSameGameDoc()
{
    // Удаляем текущую игровую доску
    delete m_board;

    // Удаляем всё из стека «Отмена» 
    ClearUndo();

    // Удаляем всё из стека «Повтор»
    ClearRedo();
}

BOOL CSameGameDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // Устанавливаем (или сбрасываем) игровую доску
    m_board->SetupBoard();

    // Очистка стеков «Отмена/Повтор»
    ClearUndo();
    ClearRedo();

    return TRUE;
}

void CSameGameDoc::SetNumColors(int nColors)
{
    // Сначала задаем количество цветов
    m_board->SetNumColors(nColors);

    // А затем устанавливаем параметры игровой доски
    m_board->SetupBoard();
}

int CSameGameDoc::DeleteBlocks(int row, int col)
{
    // Сохранение текущего состояния доски в стеке «Отмена»
    m_undo.push(new CSameGameBoard(*m_board));

    // Очищаем стек «Повтор»
    ClearRedo();

    // Затем удаляем блоки
    int blocks = m_board->DeleteBlocks(row, col);

    // Очищаем стек «Отмена» в конце игры
    if (m_board->IsGameOver())
        ClearUndo();

    // Возвращаем количество блоков
    return blocks;
}

void CSameGameDoc::UndoLast()
{
    // Смотрим, есть ли у нас что-нибудь в стеке «Отмена»
    if (m_undo.empty())
        return;

    // Помещаем текущую игровую доску в стек «Повтор»
    m_redo.push(m_board);

    // Назначаем верхний элемент стека «Отмена» текущим
    m_board = m_undo.top();
    m_undo.pop();
}

bool CSameGameDoc::CanUndo()
{
    // Убеждаемся, что у нас есть возможность выполнить отмену действия
    return !m_undo.empty();
}

void CSameGameDoc::RedoLast()
{
    // Смотрим, есть ли у нас что-нибудь в стеке «Повтор»
    if (m_redo.empty())
        return;

    // Помещаем текущую игровую доску в стек «Отмена»
    m_undo.push(m_board);

    // Назначаем верхний элемент стека «Повтор» текущим
    m_board = m_redo.top();
    m_redo.pop();
}

bool CSameGameDoc::CanRedo()
{
    // Убеждаемся, сможем ли мы выполнить повтор действия (не пуст ли стек)
    return !m_redo.empty();
}

void CSameGameDoc::ClearUndo()
{
    // Очищаем стек «Отмена»
    while (!m_undo.empty())
    {
        delete m_undo.top();
        m_undo.pop();
    }
}

void CSameGameDoc::ClearRedo()
{
    // Очищаем стек «Повтор»
    while (!m_redo.empty())
    {
        delete m_redo.top();
        m_redo.pop();
    }
}