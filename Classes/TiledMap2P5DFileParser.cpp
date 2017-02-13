#include "TiledMap2P5DFileParser.h"
#include <fstream>

USING_NS_CC;

/**
 * Implementation of TiledMap2P5DFileParser class
 */

/**
 * Public
 */

TiledMap2P5DFileParser* TiledMap2P5DFileParser::createWithParams(
	const std::string& origin,
	TiledMapInfo* tiledMapInfo,
	cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
	cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{
	auto ref = new TiledMap2P5DFileParser();
	if(ref->initWithParams(
		origin,tiledMapInfo,mapTiledLayerBundlerInfo,mapTiledLayerInfo,mapTilesheetInfo))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Protected
 */

TiledMap2P5DFileParser::TiledMap2P5DFileParser()
{}

TiledMap2P5DFileParser::~TiledMap2P5DFileParser()
{}

bool TiledMap2P5DFileParser::initWithParams(
	const std::string& origin,
	TiledMapInfo* tiledMapInfo,
	cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
	cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{
	//Parse origin file.
	this->parseOriginFile(
		origin,tiledMapInfo,mapTiledLayerBundlerInfo,mapTiledLayerInfo,mapTilesheetInfo);

	return true;
}

/**
 * Private
 */

void TiledMap2P5DFileParser::parseOriginFile(
	const std::string& path,
	TiledMapInfo* tiledMapInfo,
	cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
	cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{
	
}

TiledLayerBundlerInfo* TiledMap2P5DFileParser::parseTiledLayerBundlerInfoFile(
	const std::string& path)
{

}

TiledLayerInfo* TiledMap2P5DFileParser::parseTiledLayerInfoFile(const std::string& path)
{

}

void TiledMap2P5DFileParser::parseTilesheetInfoFile(
	const std::string& path,
	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{

}


/**
 * Implementation of TiledMapInfo class
 */

/**
 * Public
 */

TiledMapInfo* TiledMapInfo::create()
{
	auto ref = new TiledMapInfo();
	if(ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Protected
 */

TiledMapInfo::TiledMapInfo()
{}

TiledMapInfo::~TiledMapInfo()
{}

bool TiledMapInfo::init()
{
	return true;
}


/**
 * Implementation of TiledLayerBundlerInfo class
 */

/**
 * Public
 */

TiledLayerBundlerInfo* TiledLayerBundlerInfo::create()
{
	auto ref = new TiledLayerBundlerInfo();
	if(ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Protected
 */

TiledLayerBundlerInfo::TiledLayerBundlerInfo()
{}

TiledLayerBundlerInfo::~TiledLayerBundlerInfo()
{}

bool TiledLayerBundlerInfo::init()
{
	return true;
}


/**
 * Implementation of TiledLayerInfo class
 */

/**
 * Public
 */

TiledLayerInfo* TiledLayerInfo::create()
{
	auto ref = new TiledLayerInfo();
	if(ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Protected
 */

TiledLayerInfo::TiledLayerInfo()
{}

TiledLayerInfo::~TiledLayerInfo()
{}

bool TiledLayerInfo::init()
{
	return true;
}


/**
 * Implementation of TilesheetInfo class
 */

/**
 * Public
 */

TilesheetInfo* TilesheetInfo::create()
{
	auto ref = new TilesheetInfo();
	if(ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Protected
 */

TilesheetInfo::TilesheetInfo()
{}

TilesheetInfo::~TilesheetInfo()
{}

bool TilesheetInfo::init()
{
	return true;
}
