#include "TiledMap2P5DDevelopLayer.h"

USING_NS_CC;

/**
 * Protected functions
 */
TiledMap2P5DDevelopLayer::TiledMap2P5DDevelopLayer()
:_tiledMap(nullptr)
{}

TiledMap2P5DDevelopLayer::~TiledMap2P5DDevelopLayer()
{
	CC_SAFE_RELEASE_NULL(_tiledMap);
}

bool TiledMap2P5DDevelopLayer::initWithTiledMap(TiledMap2P5D *map)
{
	if(map == nullptr
		|| !Layer::init()
		|| !LayerColor::initWithColor(Color4B(84,110,112,250)))
	{
		return false;
	}



	return true;
}

/**
 * Public functions
 */
TiledMap2P5DDevelopLayer* TiledMap2P5DDevelopLayer::createWithTiledMap(TiledMap2P5D *map)
{
	TiledMap2P5DDevelopLayer* ref = new TiledMap2P5DDevelopLayer();
	if(ref->initWithTiledMap(map))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}
