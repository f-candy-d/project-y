#include "Chank.h"
#include "TiledMap2P5DFileParser.h"
#include "TiledLayer.h"

USING_NS_CC;

namespace TM25Component {

/**
 * Public
 */
TiledLayer* TiledLayer::createWithCapacity(size_t capacity)
{
	TiledLayer* ref = new TiledLayer();
	if(ref->initWithCapacity(capacity))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

void TiledLayer::loadNewChank(int num,LoadDirection direction)
{

}

/**
 * Protected
 */
TiledLayer::TiledLayer()
:_sheetInfo(nullptr)
,_layerInfo(nullptr)
,_zOlder(0)
,_capacity(0)
,_chanks(nullptr)
,_indexesOfChank(nullptr)
{}

TiledLayer::~TiledLayer()
{
	if(_capacity)
		saveAllStagedChank();
}

bool TiledLayer::initWithCapacity(size_t capacity)
{
	if(!SpriteBatchNode::init())
		return false;

	if(capacity > 0)
	{
		_chanks = new Chank*[capacity]{nullptr};
		_indexesOfChank = new int[capacity]{-1};
	}

	_capacity = capacity;

	return true;
}

/**
 * Private
 */
void TiledLayer::saveChank(int index)
{
}

void TiledLayer::saveAllStagedChank()
{

}

} /* namespace TM25Component */
