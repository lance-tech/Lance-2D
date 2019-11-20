#include "BlockContainer.h"


BlockContainer::BlockContainer() 
	: block(nullptr), IsEmpty(true), IsDelete(false)
{
}

BlockContainer& BlockContainer::operator=(const BlockContainer &other)
{
	this->IsEmpty = other.IsEmpty;
	this->IsDelete = other.IsDelete;
	this->block = other.block;
	return *this;
}

BlockContainer::~BlockContainer()
{
}

bool BlockContainer::HasBlock(int row, int column)
{
	if (block)
	{
		return true;
	}
	return false;
}

glRect2D& BlockContainer::ExtractBlock()
{
	glRect2D& _block = *block;
	block = nullptr;
	return _block;
}

glRect2D& BlockContainer::GetBlock()
{
	return *block;
}

void BlockContainer::SetSettlBlock(glRect2D& block)
{
	IsEmpty = false;
	IsDelete = false;
	this->block = &block;
}

void BlockContainer::SetEmpty(bool state)
{
	IsEmpty = state;
}

void BlockContainer::SetDelete(bool state)
{
	IsDelete = state;
}

bool BlockContainer::GetEmpty()
{
	return IsEmpty;
}

bool BlockContainer::GetDelete()
{
	return IsDelete;
}