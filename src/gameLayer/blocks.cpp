#pragma once
#include "blocks.h"
#include "items.h"

bool isCraftable(int type)
{
	switch (type)
	{
	case Block::dirt:
	case Block::stone:
	case Block::woodLog:
		return true;
	default:
		return false;
	}
}

bool isCollidable(int type)
{
	switch (type)
	{
	case Block::air:
	case Block::leaves:
	case Block::platform:
	case Block::workBench:
	case Block::painting:
	case Block::sappling:
	case Block::door:
	case Block::jar:
	case Block::table:
	case Block::wordrobe:
	case Block::bookShelf:
	case Block::icePlatform:
	case Block::iceTable:
	case Block::iceWordrobe:
	case Block::iceBookShelf:
	case Block::sandPlatform:
	case Block::sandTable:
	case Block::sandWordrobe:
	case Block::sandBookShelf:
	case Block::bonePlatform:
	case Block::boneBench:
	case Block::boneWordrobe:
	case Block::boneBookShelf:
	case Block::woodLog:
		return false;
	default:
		return true;
	}
	return false;
}