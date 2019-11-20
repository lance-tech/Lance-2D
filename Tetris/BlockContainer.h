#pragma once

#include <Engine.h>
#include <GLObject/glRect2D.h>


class BlockContainer
{
public:
	BlockContainer();
	~BlockContainer();
	
	bool HasBlock(int row, int column);
	glRect2D& GetBlock();
	glRect2D& ExtractBlock();
	void SetSettlBlock(glRect2D& block);
	void SetEmpty(bool state);
	void SetDelete(bool state);
	bool GetEmpty();
	bool GetDelete();
	BlockContainer& operator=(const BlockContainer &other);

private:
	glRect2D* block;
	bool IsEmpty;
	bool IsDelete;
};
