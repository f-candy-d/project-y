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

void Chank::makeTiles(cocos2d::SpriteBatchNode *parent, TilesheetInfo *tilesheetInfo,bool storeSprites)
{
	
}

void Chank::eraseTiles(cocos2d::SpriteBatchNode *parent)
{

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
