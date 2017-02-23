#include "TiledMap2P5D.h"
#include "TiledMap2P5DFileParser.h"
#include "TiledLayer.h"
#include <iostream>

USING_NS_CC;

/**
 * Protected functions
 */
TiledMap2P5D::TiledMap2P5D()
:_tiledMapInfo(nullptr)
{}

TiledMap2P5D::~TiledMap2P5D()
{
	if(_tiledMapInfo)
		_tiledMapInfo->release();
}

bool TiledMap2P5D::initWithFile(std::string path)
{
	if(!Node::init())
	{
		return false;
	}

	//Parse a origin file and create information objects.
	_tiledMapInfo = TM25Component::TiledMapInfo::create();
	_tiledMapInfo->retain();
	if(!TM25Component::TiledMap2P5DFileParser::parseWithArgs(
		path,_tiledMapInfo,_TiledLayerBundlerInfoMap,_tiledLayerInfoMap,_tilesheetInfoMap))
		return false;

	//Test
	auto layer = TM25Component::TiledLayer::createWithParams(2,_tilesheetInfoMap.at("test_sheet"),_tiledLayerInfoMap.at("test_layer"),_tiledMapInfo);
	this->addChild(layer);

	return true;
}

/**
 * Public functions
 */
TiledMap2P5D* TiledMap2P5D::createWithFile(std::string path)
{
	TiledMap2P5D* ref = new TiledMap2P5D();
	if(ref->initWithFile(path))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}
