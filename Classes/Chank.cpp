#include "Chank.h"
#include "TiledMap2P5DFileParser.h"
#include <iostream>

USING_NS_CC;

namespace TM25Component {

/**
 * Public functions
 */
Chank* Chank::createWithParam(cocos2d::Size size, int index)
{
	Chank* ref = new Chank();
	if(ref->initWithParam(size,index))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

void Chank::makeTiles(cocos2d::SpriteBatchNode *parent,TilesheetInfo *tilesheetInfo)
{
	if(_sprites.size() > 0)
		return;

	for(size_t y = 0; y < GRID_HEIGHT; ++y)
	{
		for(size_t x = 0; x < GRID_WIDTH; ++x)
		{
			//_tiles[i] = -1 means that there is no tile on the grid.
			if(_tiles[GRID_WIDTH * y + x] >= 0)
			{
				//Get a texture rect
				Rect texture_rect = tilesheetInfo->getTextureRectForType(_tiles[GRID_WIDTH * y + x]);
				//Make tile sprite
				auto sprite = Sprite::create(tilesheetInfo->getSheetName(),texture_rect);
				sprite->setPosition(
					x * tilesheetInfo->getTileSize().width,
					y * tilesheetInfo->getTileSize().height);
				//Set the index of this chank to the tag of a sprite
				sprite->setTag(_index);

				parent->addChild(sprite);

				//Store the sprite
				_sprites.pushBack(sprite);
			}
		}
	}
}

void Chank::eraseTiles(cocos2d::SpriteBatchNode *parent)
{
	//Get tile sprites of this chank from the parent node
	// and remove it.

	if(_sprites.size() == 0)
		return;

	Node* np = parent->getChildByTag(_index);
	while(np)
	{
		parent->removeFromParentAndCleanup(np);
		np = parent->getChildByTag(_index);
	}

	//Remove from the vector too
	_sprites.clear();
}

/**
 * Protected functions
 */
Chank::Chank()
:_index(-1)
,_size(0,0)
,_origin(0,0)
{}

Chank::~Chank()
{}

bool Chank::initWithParam(cocos2d::Size size,int index)
{
	if(index < 0)
		return false;

	//Setup params
	_index = index;
	_size.setSize(size.width * this->GRID_WIDTH,size.height * this->GRID_HEIGHT);
	_origin.set(_size.width * _index,0);

	return true;
}

/**
 * Private functions
 */
int Chank::countDigit(u_int n)
{
	int digit;

	if (n < 100000)
	{
		if (n < 1000)
		{
			if (n < 10)
			digit = 1;
			else if (n < 100)
			digit = 2;
			else
			digit = 3;
		}
		else
		{
			if (n < 10000)
			digit = 4;
			else
			digit = 5;
		}
	}
	else
	{
		if (n < 10000000)
		{
			if (n < 1000000)
			digit = 6;
			else
			digit = 7;
		}
		else
		{
			if (n < 100000000)
			digit = 8;
			else if (n < 1000000000)
			digit = 9;
			else
			digit = 10;
		}
	}

	return digit;
}

u_l_int Chank::pow10(u_int e)
{
	u_l_int p10 = 1;

	//The max of p10 is 10,000,000,000,000,000,000.
	while(0 < e && e <= 20)
	{
		p10*=10;
		e--;
	}

	return p10;
}

u_l_int Chank::makeHashOfCoordinate(u_int x, u_int y)
{
	u_int dig_y = countDigit(y);
	return (x * pow10(dig_y) + y) * 10 + dig_y;
}

Vec2 Chank::decodeHashOfCoordinate(u_l_int hash)
{
	Vec2 vec;
	int y_digit;
	u_l_int pow_10_y_dg;

	y_digit = hash % 10;
	hash /= 10;
	pow_10_y_dg = pow10(y_digit);
	vec.x = hash / pow_10_y_dg;
	vec.y = hash - (u_int)vec.x * pow_10_y_dg;

	return std::move(vec);
}

} /* namespace TM25Component */
