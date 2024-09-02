#include "pch.h"
#include "CSameGameBoard.h"

COLORREF CSameGameBoard::m_arrColors[8];

CSameGameBoard::CSameGameBoard() :
	m_arrBoard(NULL),
	m_nColumns(15), m_nRows(15),
	m_nHeight(35), m_nWidth(35),
	m_nRemaining(0), m_nColors(3)
{
	m_arrColors[0] = RGB(0, 0, 0); //черный
	m_arrColors[1] = RGB(255, 0, 0); //красный
	m_arrColors[2] = RGB(255, 255, 64); //зеленый
	m_arrColors[3] = RGB(0, 0, 255); //синий

	//новые 4 цвета для сложности
	m_arrColors[4] = RGB(0, 255, 0);
	m_arrColors[5] = RGB(0, 255, 255);
	m_arrColors[6] = RGB(255, 0, 128);
	m_arrColors[7] = RGB(0, 64, 0);

	//создание и настройка параметров игровой доски
	SetupBoard();
}

CSameGameBoard::CSameGameBoard(const CSameGameBoard& board)
{
	// Копирование всех элементов класса
	m_nColumns = board.m_nColumns;
	m_nRows = board.m_nRows;
	m_nHeight = board.m_nHeight;
	m_nWidth = board.m_nWidth;
	m_nRemaining = board.m_nRemaining;
	m_nColors = board.m_nColors;

	// Копирование цветовых элементов
	for (int i = 0; i < 8; i++)
		m_arrColors[i] = board.m_arrColors[i];
	m_arrBoard = NULL;

	// Создание нового игрового поля
	CreateBoard();

	// Копирование содержимого игрового поля
	for (int row = 0; row < m_nRows; row++)
		for (int col = 0; col < m_nColumns; col++)
			m_arrBoard[row][col] = board.m_arrBoard[row][col];
}

CSameGameBoard::~CSameGameBoard() {
	DeleteBoard();
}

void CSameGameBoard::SetupBoard() {
	//при необходимости создаем доску
	if (m_arrBoard == NULL)
		CreateBoard();

	//установка случайного цвета каждому блоку
	for (int row = 0; row < m_nRows; row++) {
		for (int col = 0; col < m_nColumns; col++)
			m_arrBoard[row][col] = (rand() % m_nColors) + 1;
	}

	//устанавливаем кол-во оставшихся блоков
	m_nRemaining = m_nRows * m_nColumns;
}

COLORREF CSameGameBoard::GetBoardSpace(int row, int col) {
	//проверяем границы массива
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return m_arrColors[0];
	return m_arrColors[m_arrBoard[row][col]];
}

void CSameGameBoard::DeleteBoard() {
	if (m_arrBoard != NULL) {
		for (int row = 0; row < m_nRows; row++) {
			if (m_arrBoard != NULL) {
				delete[] m_arrBoard[row]; 
				m_arrBoard[row] = NULL;
			}
		}
		delete[] m_arrBoard; 
		m_arrBoard = NULL;
	}
}

void CSameGameBoard::CreateBoard() {
	if (m_arrBoard != NULL)
		DeleteBoard();

	m_arrBoard = new int* [m_nRows];

	for (int row = 0; row < m_nRows; row++) {
		m_arrBoard[row] = new int[m_nColumns];

		// Устанавливаем для каждого блока значение цвета, равное цвету фона
		for (int col = 0; col < m_nColumns; col++)
			m_arrBoard[row][col] = 0;
	}
}

int CSameGameBoard::DeleteBlocks(int row, int col) {
	//проверяем на корректность индексы ячейки и столбца 
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return -1;

	//если блок уже имеет цвет фона, то удалить его не получится
	int nColor = m_arrBoard[row][col];
	if (nColor == 0)
		return -1;

	//сначала проверим, есть ли примыкающие блоки с тем же цветом
	int nCount = -1;
	if ((row - 1 >= 0 && m_arrBoard[row - 1][col] == nColor) ||
		(row + 1 < m_nRows && m_arrBoard[row + 1][col] == nColor) ||
		(col - 1 >= 0 && m_arrBoard[row][col - 1] == nColor) ||
		(col + 1 < m_nColumns && m_arrBoard[row][col + 1] == nColor))
	{
		//затем рекурсивно вызываем функцию для удаления примыкающих блоков одного цвета...
		m_arrBoard[row][col] = 0;
		nCount = 1;

		//...сверху...
		nCount += DeleteNeighborBlocks(row - 1, col, nColor, DIRECTION_DOWN);

		//...снизу...
		nCount += DeleteNeighborBlocks(row + 1, col, nColor, DIRECTION_UP);

		//...слева...
		nCount += DeleteNeighborBlocks(row, col - 1, nColor, DIRECTION_RIGHT);

		//...справа...
		nCount += DeleteNeighborBlocks(row, col + 1, nColor, DIRECTION_LEFT);

		//в конце выполняем сжатие игровой доски
		CompactBoard();

		//вычитаем число удаленных блоков из общего кол-ва блоков
		m_nRemaining -= nCount;
	}

	//возвращаем кол-во удаленных блоков 
	return nCount;
}

int CSameGameBoard::DeleteNeighborBlocks(int row, int col, int color, Direction direction) {
	//проверяем на корректность индексы ячейки и столбца
	if (row < 0 || row >= m_nRows || col < 0 || col >= m_nColumns)
		return 0;

	//проверка на то, что блок имеет такой же цвет
	if (m_arrBoard[row][col] != color)
		return 0;
	int nCount = 1;
	m_arrBoard[row][col] = 0;

	//выполняем проверку направлений
	if (direction != DIRECTION_UP)
		nCount += DeleteNeighborBlocks(row - 1, col, color, DIRECTION_DOWN);
	if (direction != DIRECTION_DOWN)
		nCount += DeleteNeighborBlocks(row + 1, col, color, DIRECTION_UP);
	if (direction != DIRECTION_LEFT)
		nCount += DeleteNeighborBlocks(row, col - 1, color, DIRECTION_RIGHT);
	if (direction != DIRECTION_RIGHT)
		nCount += DeleteNeighborBlocks(row, col + 1, color, DIRECTION_LEFT);

	//возвращаем общее кол-во удаленных блоков
	return nCount;
}

void CSameGameBoard::CompactBoard() {
	//сначала мы все сдвигаем вниз
	for (int col = 0; col < m_nColumns; col++) 
	{
		int nNextEmptyRow = m_nRows - 1;
		int nNextOccupiedRow = nNextEmptyRow;
		while (nNextOccupiedRow >= 0 && nNextEmptyRow >= 0)
		{
			//находим пустую строку
			while (nNextEmptyRow >= 0 &&
				m_arrBoard[nNextEmptyRow][col] != 0)
			{
				nNextEmptyRow--;
			}
			if (nNextEmptyRow >= 0) 
			{
				//затем находим занятую строку, расположенную следом за пустой
				nNextOccupiedRow = nNextEmptyRow - 1;
				while (nNextOccupiedRow >= 0 &&
					m_arrBoard[nNextOccupiedRow][col] == 0)
				{
					nNextOccupiedRow--;
				}
				if (nNextOccupiedRow >= 0)
				{
					//теперь перемещаем блоки с занятой строки на пустую
					m_arrBoard[nNextEmptyRow][col] =
						m_arrBoard[nNextOccupiedRow][col];
					m_arrBoard[nNextOccupiedRow][col] = 0;
				}
			}
		}
	}
	//затем все, что находится справа, смещаем влево
	int nNextEmptyCol = 0;
	int nNextOccupiedCol = nNextEmptyCol;
	while (nNextEmptyCol < m_nColumns && nNextOccupiedCol < m_nColumns)
	{
		//находим пустой столбец
		while (nNextEmptyCol < m_nColumns &&
			m_arrBoard[m_nRows - 1][nNextEmptyCol] != 0)
		{
			nNextEmptyCol++;
		}
		if (nNextEmptyCol < m_nColumns)
		{
			//затем находим занятый столбец, расположенный следом за пустым
			nNextOccupiedCol = nNextEmptyCol + 1;
			while (nNextOccupiedCol < m_nColumns &&
				m_arrBoard[m_nRows - 1][nNextOccupiedCol] == 0)
			{
				nNextOccupiedCol++;
			}
			if (nNextOccupiedCol < m_nColumns)
			{
				//сдвигаем весь столбец влево
				for (int row = 0; row < m_nRows; row++)
				{
					m_arrBoard[row][nNextEmptyCol] = 
						m_arrBoard[row][nNextOccupiedCol];
					m_arrBoard[row][nNextOccupiedCol] = 0;
				}
			}
		}
	}
}

bool CSameGameBoard::IsGameOver() const {
	//проверяем столбец за столбцом (слева-направо)
	for (int col = 0; col < m_nColumns; col++)
	{
		//строку за строкой (снизу-вверх)
		for (int row = m_nRows - 1; row >= 0; row--)
		{
			int nColor = m_arrBoard[row][col];

			//если мы попали на ячейку с цветом фона, то это значит, что столбец уже уничтожен
			if (nColor == 0)
				break;
			else
			{
				//проверяем сверху и справа
				if (row - 1 >= 0 &&
					m_arrBoard[row - 1][col] == nColor)
				{
					return false;
				}
				else if (col + 1 < m_nColumns &&
					m_arrBoard[row][col + 1] == nColor)
				{
					return false;
				}
			}
		}
	}
	//если примыкающих блоков не обнаружено, то
	return true;
}