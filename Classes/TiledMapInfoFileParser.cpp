#include "TiledMapInfoFileParser.h"
#include <fstream>

USING_NS_CC;

const size_t SSCANF_BUFF_LEN =  64;

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
void TiledMapInfoFileParser::parseTiledLayerInfoFile(
	TiledLayerInfo *tiledLayerInfo,std::string file)
{
	//Parse a tiled-layer-info file
}

void TiledMapInfoFileParser::parseBunchedLayerInfoFile(
	BunchedLayerInfo *bunchedLayerInfo, std::string file)
{
	//Parse a bunched-layer-info file
}

void TiledMapInfoFileParser::parseTilesheetFile(
	const cocos2d::Map<std::string,TilesheetInfo*>& tilesheetInfos,std::string file)
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
	cocos2d::Map<std::string,TiledLayerInfo*>& tiledLayerInfos,
	cocos2d::Map<std::string,BunchedLayerInfo*>& bunchedLayerInfos,
	cocos2d::Map<std::string,TilesheetInfo*>& tilesheetInfos)
{
	std::ifstream i_file_stream(file,std::ios::in);
	std::string line_buff;
	char buff[SSCANF_BUFF_LEN];
	size_t tmp;

	//Create instance
	tiledMapInfo = TiledMapInfo::create();

	if(i_file_stream.eof())
	{
		log("WORNIG!! >> The file '%s' has nothing to be read.",file.c_str());
		return;
	}

	while(!i_file_stream.eof())
	{
		//Parse the map-info file
		//Read a line
		std::getline(i_file_stream,line_buff);

		//Skip a comment
		if(line_buff.find("#") == static_cast<size_t>(-1))
			continue;
		//The name of the directory which contains information files of a map
		else if(line_buff.find("<Directory") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<Directory=%s/>",buff);
			tiledMapInfo->setDirectory(buff);
		}
		//The number of chanks in a layer
		else if(line_buff.find("<NumOfChankInLayer") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<NumOfChankInLayer=%zu/>",&tmp);
			tiledMapInfo->setNumChank(tmp);
		}
		//The number of all tiled layers
		else if(line_buff.find("<NumOfTiledLayer") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<NumOfTiledLayer=%zu/>",&tmp);
			tiledMapInfo->setNumTiledLayer(tmp);
		}
		//The number of bunched layers
		else if(line_buff.find("<NumOfBunchedLayer") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<NumOfBunchedLayer=%zu/>",&tmp);
			tiledMapInfo->setNumBunchedLayer(tmp);
		}
		//The name of a tiled layer information file
		else if(line_buff.find("<TiledLayerInfoFile") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<TiledLayerInfoFile=%s/>",buff);
			TiledLayerInfo* tli = TiledLayerInfo::create();
			parseTiledLayerInfoFile(tli,std::string(buff));
			tiledLayerInfos.insert(tli->getName(),tli);
		}
		//The name of a bunched layer information file
		else if(line_buff.find("<BunchedLayerInfoFile") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<BunchedLayerInfoFile=%s/>",buff);
			BunchedLayerInfo* bli = BunchedLayerInfo::create();
			parseBunchedLayerInfoFile(bli,std::string(buff));
			bunchedLayerInfos.insert(bli->getName(),bli);
		}
		//The name of the tilesheet information file
		else if(line_buff.find("<TilesheetInfoFile") != static_cast<size_t>(-1))
		{
			sscanf(line_buff.c_str(),"<TilesheetInfoFile=%s/>",buff);
			parseTilesheetFile(tilesheetInfos,std::string(buff));
		}
		//The architecture of the map
		else if(line_buff == "<MapArchitecture>")
		{
			std::getline(i_file_stream,line_buff);
			while (line_buff != "<MapArchitecture/>") {
				if(line_buff.find("<BunchedLayer") != static_cast<size_t>(-1))
				{
					sscanf(line_buff.c_str(),"<BunchedLayer=%s/>",buff);
					tiledMapInfo->getArchitecture().push_back(std::string(buff));
				}
				//Next line
				std::getline(i_file_stream,line_buff);
			}
		}
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
,_directory("")
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

std::vector<std::string>& TiledMapInfo::getArchitecture()
{
	return _architecture;
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
 * Implementation of the BunchedLayerInfo class
 */

/**
 * Protected functions
 */
BunchedLayerInfo::BunchedLayerInfo()
{}

BunchedLayerInfo::~BunchedLayerInfo()
{}

bool BunchedLayerInfo::init()
{
	return true;
}

/**
 * Private functuins
 */

/**
 * Public functuins
 */
BunchedLayerInfo* BunchedLayerInfo::create()
{
	BunchedLayerInfo* ref = new BunchedLayerInfo();
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
