#include "TiledMap2P5D.h"
#include "TiledMap2P5DFileParser.h"
#include "TiledLayer.h"
#include "TM25CommonProperty.h"
#include "Chank.h"
#include <iostream>
#include <cmath>

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

	//Touch events
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [](Touch* touch,Event* event)
	{
		return true;
	};
	listener->onTouchMoved = [this](Touch* touch,Event* event)
	{
		Vec2 delta = touch->getDelta();
		Vec2 now = this->getPosition();
		this->setPosition(now.x + delta.x,0);
		poolDeltaX(delta.x);
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	//Test
	_layer = TM25Component::TiledLayer::createWithParams(3,_tilesheetInfoMap.at("test_sheet"),_tiledLayerInfoMap.at("test_layer"),_tiledMapInfo);
	this->addChild(_layer);

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

/**
 * Private
 */
void TiledMap2P5D::poolDeltaX(float dx)
{
	_xDeltaPool += dx;
	if(std::fabs(_xDeltaPool) > TM25Component::Chank::GRID_WIDTH * _tiledMapInfo->getTileOneSide())
	{
		_layer->loadNewChank(1,(_xDeltaPool > 0) ? TM25Component::LoadDirection::LEFT : TM25Component::LoadDirection::RIGHT);
		_xDeltaPool = 0;
	}
}
