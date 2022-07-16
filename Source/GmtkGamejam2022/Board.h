#pragma once

class Board
{
public:
	virtual ~Board() = default;
	
	virtual TSubclassOf<ATile> RandomTileClass() = 0;
	virtual void UpdateCell(int32 Column, int32 Row, TSubclassOf<ATile> TileClass) = 0;
protected:
	Board() = default;
};
