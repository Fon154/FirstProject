#pragma once
//#include <vector>

class CSameGameBoard
{
public:
	// Конструктор по умолчанию
	CSameGameBoard();

	// Конструктор глубокого копирования
	CSameGameBoard(const CSameGameBoard& board);

	// Деструктор
	~CSameGameBoard();

	void SetupBoard(); 

	COLORREF GetBoardSpace(int row, int col); // получаем цвет в определенном участке игрового поля

	// Функции доступа для получения/изменения размера игровой доски
	int GetWidth(void) const { return m_nWidth; }
	void SetWidth(int nWidth) { m_nWidth = (nWidth >= 3) ? nWidth : 3; }

	int GetHeight(void) const { return m_nHeight; }
	void SetHeight(int nHeight) { m_nHeight = (nHeight >= 3) ? nHeight : 3; }

	int GetColumns(void) const { return m_nColumns; }
	void SetColumns(int nColumns) { m_nColumns = (nColumns >= 5) ? nColumns : 5; }

	int GetRows(void) const { return m_nRows; }
	void SetRows(int nRows) { m_nRows = (nRows >= 5) ? nRows : 5; }

	//мы закончили игру ?
	bool IsGameOver(void) const;

	//подсчет количества оставшихся блоков
	int GetRemainingCount(void) const { return m_nRemaining; }

	//функция для удаления всех примыкающих блоков
	int DeleteBlocks(int row, int col);

	void DeleteBoard();

	//гетеры и сетеры для кол-ва цветов
	int GetNumColors(void) { return m_nColors; }

	void SetNumColors(int nColors)
	{
		m_nColors = (nColors >= 3 && nColors <= 7) ? nColors : m_nColors;
	}
	
private:
	void CreateBoard();

	//перечисление с вариантами направления (откуда мы пришли) потребуется для удаления блоков
	enum Direction
	{
		DIRECTION_UP,
		DIRECTION_DOWN,
		DIRECTION_LEFT,
		DIRECTION_RIGHT
	};

	//вспомогательная рекурсивная функция для удаления примыкающих блоков 
	int DeleteNeighborBlocks(int row, int col, int color, Direction direction);

	//функция для сжатия доски после того, как были удалены блоки
	void CompactBoard();

	//указатель на двумерный массив
	int** m_arrBoard; //std::vector<std::vector<int>> m_arrBoard

	// список цветов: 0 - цвет фона, 1-7 - цвета блоков
	static COLORREF m_arrColors[8]; 

	//инфа о размере игрового поля
	int m_nColumns;
	int m_nRows;
	int m_nHeight;
	int m_nWidth;

	//кол-во оставшихся блоков
	int m_nRemaining;

	//кол-во цветов
	int m_nColors;
};

