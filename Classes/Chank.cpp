#include "Chank.h"
#include "TiledMap2P5DFileParser.h"

USING_NS_CC;

namespace TM25Component {

/**
 * Public functions
 */
Chank* Chank::createWithParam(cocos2d::Size size, size_t index)
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

bool Chank::initWithParam(cocos2d::Size size,size_t index)
{
	//Setup params
	_index = index;
	_size.setSize(size.width * this->GRID_WIDTH,size.height * this->GRID_HEIGHT);
	_origin.set(_size.width * _index,0);

	return true;
}

/**
 * Private functions
 */

} /* namespace TM25Component */
