#include "TiledMap2P5D.h"

USING_NS_CC;

/**
 * Protected functions
 */
TiledMap2P5D::TiledMap2P5D()
{}

TiledMap2P5D::~TiledMap2P5D()
{}

bool TiledMap2P5D::init()
{
	if(!Node::init())
	{
		return false;
	}

	return true;
}

/**
 * Public functions
 */
TiledMap2P5D* TiledMap2P5D::create()
{
	TiledMap2P5D* ref = new TiledMap2P5D();
	if(ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

TiledLayerInfo* TiledMap2P5D::getTiledLayerInfoByName(std::string name)
{
	return _tiledLayerInfoMap.at(name);
}

BunchedLayerInfo* TiledMap2P5D::getBunchedLayerInfoByName(std::string name)
{
	return _bunchedLayerInfoMap.at(name);
}

TilesheetInfo* TiledMap2P5D::getTilesheetInfoByName(std::string name)
{
	return _tilesheetInfoMap.at(name);
}
