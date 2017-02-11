#include "TiledMapInfoFileParser.h"
#include <ifstream>

USING_NS_CC;

/**
 * Implementation of the TiledMapInfoFileParser class
 */

/**
 * Protected functions
 */
TiledMapInfoFileParser::TiledMapInfoFileParser()
{}

TiledMapInfoFileParser::~TiledMapInfoFileParser()
{}

bool TiledMapInfoFileParser::init()
{
	return true;
}

/**
 * Private functions
 */
void TiledMapInfoFileParser::parseTiledLayerInfoFile(TiledLayerInfo *tiledLayerInfo)
{
	//Parse a tiled-layer-info file
}

void TiledMapInfoFileParser::parseTilesheetFile(const cocos2d::Vector<TilesheetInfo*>& tilesheetInfos)
{
	//Parse a tilesheet-info file
}

/**
 * Public functions
 */
TiledMapInfoFileParser* TiledMapInfoFileParser::create()
{
	TiledMapInfoFileParser* ref = new TiledMapInfoFileParser();
	if(ref && ref->init())
	{
		ref->autorelease();
		return ref;
	}

	//If initialization fail,release object and return nullptr
	CC_SAFE_DELETE(ref);
	return nullptr;
}

void TiledMapInfoFileParser::parseOriginFile(
	std::string file,
	TiledMapInfo* tiledMapInfo,
	const cocos2d::Vector<TiledLayerInfo*>& tiledLayerInfos,
	const cocos2d::Vector<TilesheetInfo*>& tilesheetInfos)
{
	//Create instance
	tiledMapInfo = TiledMapInfo::create();

	std::ifstream file_stream(file,std::ios::in);
	std::string line_buff;

	if(i_file_stream.eof)
	{
		log("WORNIG!! >> The file '%s' has nothing to be read.",file.c_str());
		return;
	}

	while(!i_file_stream.eof())
	{
		//Parse the map-info file
		//1.The number of chanks
		
	}

}

/**
 * Implementation of the TiledMapInfo class
 */

/**
 * Protected functions
 */
TiledMapInfo::TiledMapInfo()
:_numChank(0)
,_numTiledLayer(0)
,_numBunchedLayer(0)
{}

TiledMapInfo::~TiledMapInfo()
{}

bool TiledMapInfo::init()
{
	return true;
}

/**
 * Public functions
 */
TiledMapInfo* TiledMapInfo::create()
{
	TiledMapInfo* ref = new TiledMapInfo();
	if(ref && ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Implementation of the TiledLayerInfo class
 */

/**
 * Protected functions
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
 * Public functions
 */
TiledLayerInfo* TiledLayerInfo::create()
{
	TiledLayerInfo* ref = new TiledLayerInfo();
	if(ref && ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Implementation of the TilesheetInfo class
 */

/**
 * Protected functions
 */
TilesheetInfo::TilesheetInfo()
:_name("")
,_numTileType(0)
,_textureRects(nullptr)
{}

TilesheetInfo::~TilesheetInfo()
{
	delete [] _textureRects;
}

bool TilesheetInfo::initWithN(size_t n)
{
	_numTileType = n;
	_textureRects = new cocos2d::Rect[n];

	return true;
}

/**
 * Public functions
 */
TilesheetInfo* TilesheetInfo::createWithN(size_t n)
{
	TilesheetInfo* ref = new TilesheetInfo();
	if(ref && ref->initWithN(n))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}
