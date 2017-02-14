#include "TiledMap2P5DFileParser.h"
#include <fstream>
#include <regex>

USING_NS_CC;

/**
 * Implementation of TiledMap2P5DFileParser class
 */

const size_t SSCANF_BUFF_LEN =  64;
const size_t NOT_MACHING_FLAG = static_cast<size_t>(-1);

//Regexes
const std::string REGEX_TAG_BEGIN("<*>");
const std::string REGEX_TAG_END("<*/>");
const std::string REGEX_VALUE("<@*>*");

//Value tags
const std::string VALUE_SIZE_T("<@sizeT>");
const std::string VALUE_PATH("<@Path>");
const std::string VALUE_LAYER_NAME("<@LayerName>");

//Formats for sscanf()
const std::string FORMAT_SIZE_T(VALUE_SIZE_T + std::string("%zu"));
const std::string FORMAT_PATH(VALUE_PATH + std::string("%s"));
const std::string FORMAT_LAYER_NAME(VALUE_LAYER_NAME + std::string("%s"));

//Tags
const std::string TAG_GRID_WIDTH("<GridWidth");
const std::string TAG_GRID_HEIGHT("<GridHeight");
const std::string TAG_TILED_LAYER_INFORMATION("<TiledLayerInformation");
const std::string TAG_TILED_LAYER_BUNDLER_INFORMATION("<TiledLayerBundlerInformation");
const std::string TAG_TILESHEET_INFORMATION("<TilesheetInformation");
const std::string TAG_ARCHITECTURE("<Architecture");
const std::string TAG_NAME("<Name");

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

bool TiledMap2P5DFileParser::parseOriginFile(
	const std::string& path,
	TiledMapInfo* tiledMapInfo,
	cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
	cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{
	std::ifstream i_file_stream(path,std::ios::in);
	std::string line_buff;
	std::string current_tag("");
	char buff[SSCANF_BUFF_LEN];
	size_t tmp;
	//If any information file couldn't be loaded,this is false.
	bool flag = true;


	if(i_file_stream.eof())
	{
		log("WORNIG!! >> The file '%s' has nothing to be read.",path.c_str());
		return false;
	}

	while(!i_file_stream.eof())
	{
		//Parse the map-info file
		//Read a line
		std::getline(i_file_stream,line_buff);
		deleteHeadSpace(line_buff);

		//Skip a comment
		if(findStr(line_buff,"#") || line_buff == "\n")
			continue;
		//The line is end of a tag
		else if(matchingRegex(line_buff,REGEX_VALUE))
		{
			//Get a value for a tag.
			if(findStr(line_buff,VALUE_SIZE_T))
			{
				//A value type of size_t
				sscanf(line_buff.c_str(),FORMAT_SIZE_T.c_str(),&tmp);

				if(current_tag == TAG_GRID_WIDTH)
				{
					tiledMapInfo->setMapGridWidth(tmp);
				}
				else if(current_tag == TAG_GRID_HEIGHT)
				{
					tiledMapInfo->setMapGridHeight(tmp);
				}
			}
			else if(findStr(line_buff,VALUE_PATH))
			{
				//A value type of file-path
				sscanf(line_buff.c_str(),FORMAT_PATH.c_str(),buff);

				if(current_tag == TAG_TILED_LAYER_INFORMATION)
				{
					TiledLayerInfo* tli = parseTiledLayerInfoFile(std::string(buff));
					if(tli != nullptr)
						mapTiledLayerInfo.insert(tli->getLayerName(),tli);
					else
						flag = false;
				}
				else if(current_tag == TAG_TILED_LAYER_BUNDLER_INFORMATION)
				{
					TiledLayerBundlerInfo* bli =
						parseTiledLayerBundlerInfoFile(std::string(buff));
					if(bli != nullptr)
						mapTiledLayerBundlerInfo.insert(bli->getName(),bli);
					else
						flag = false;
				}
				else if(current_tag == TAG_TILESHEET_INFORMATION)
				{
					flag = parseTilesheetInfoFile(std::string(buff),mapTilesheetInfo);
				}
			}
			else if(findStr(line_buff,VALUE_LAYER_NAME))
			{
				//A value type of layer-name
				sscanf(line_buff.c_str(),FORMAT_LAYER_NAME.c_str(),buff);

				if(current_tag == TAG_ARCHITECTURE)
				{
					tiledMapInfo->addElementToArchitecture(std::string(buff));
				}
			}
		}
		//The line is end of a tag
		else if(matchingRegex(line_buff,REGEX_TAG_END))
		{
			current_tag.erase(current_tag.begin(),current_tag.end());
		}
		//The line is begining of a tag
		else if(matchingRegex(line_buff,REGEX_TAG_BEGIN))
		{
			current_tag = findTagInLine(line_buff);
		}
	}

	// DEBUG
	log("\n**\tDEBUG OF parseOriginFile()\t**");
	log("TiledMapInfo::GridWidth::%zu",tiledMapInfo->getMapGridWidth());
	log("TiledMapInfo::GridHeight::%zu",tiledMapInfo->getMapGridHeight());
	for(auto str:tiledMapInfo->getArchitecture())
	{
		log("TiledMapInfo::Architecture::%s",str.c_str());
	}
	log("\t**\tEND\t**\n");

	return flag;
}

TiledLayerBundlerInfo* TiledMap2P5DFileParser::parseTiledLayerBundlerInfoFile(
	const std::string& path)
{
	std::ifstream i_file_stream(path,std::ios::in);
	std::string line_buff;
	std::string current_tag("");
	char buff[SSCANF_BUFF_LEN];
	size_t tmp;
	//If any information file couldn't be loaded,this is false.
	bool flag = true;


	if(i_file_stream.eof())
	{
		log("WORNIG!! >> The file '%s' has nothing to be read.",path.c_str());
		return nullptr;
	}

	auto info = TiledLayerBundlerInfo::create();

	if(info == nullptr)
		return nullptr;

	while(!i_file_stream.eof())
	{
		//Read line
		std::getline(i_file_stream,line_buff);
		deleteHeadSpace(line_buff);

		//Skip a comment and empty line.
		if(findStr(line_buff,"#") || line_buff == "\n")
			continue;
		//Value tag
		else if(matchingRegex(line_buff,REGEX_VALUE))
		{
			if(findStr(line_buff,VALUE_LAYER_NAME))
			{
				sscanf(line_buff.c_str(),FORMAT_LAYER_NAME.c_str(),buff);
				if(current_tag == TAG_NAME)
				{
					info->setName(std::string(buff));
				}
				else if(current_tag == TAG_ARCHITECTURE)
				{
					info->addElementToArchitecture(std::string(buff));
				}
			}
		}
		//End tag
		else if(matchingRegex(line_buff,REGEX_TAG_END))
		{
			current_tag.erase(current_tag.begin(),current_tag.end());
		}
		//Begin tag
		else if(matchingRegex(line_buff,REGEX_TAG_BEGIN))
		{
			current_tag = findTagInLine(line_buff);
		}
	}

	// DEBUG
	log("\n**\tDEBUG OF parseTiledLayerBundlerInfoFile()\t**");
	log("TiledLayerBundlerInfo::Name::%s",info->getName().c_str());
	for(auto str:info->getArchitecture())
	{
		log("TiledLayerBundlerInfo::Architecture::%s",str.c_str());
	}
	log("\t**\tEND\t**\n");

	return info;
}

TiledLayerInfo* TiledMap2P5DFileParser::parseTiledLayerInfoFile(const std::string& path)
{
	return nullptr;
}

bool TiledMap2P5DFileParser::parseTilesheetInfoFile(
	const std::string& path,
	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{
	return true;
}

bool TiledMap2P5DFileParser::matchingRegex(
	const std::string &text,
	const std::string &regex,
	std::string::const_iterator i_txt,
	std::string::const_iterator i_rgx)
{
	while(1)
	{
		if(*i_txt == 0)
			return *i_rgx == 0;
		else if(*i_rgx == 0)
			return false;
		else if((*i_rgx == '$' && '0' <= *i_txt && *i_txt <= '9') || *i_rgx == '*')
			return (i_rgx + 1) == regex.end()
			|| matchingRegex(text,regex,i_txt,i_rgx + 1)
			|| matchingRegex(text,regex,i_txt + 1,i_rgx);
		else if(*i_rgx == '$')
			return matchingRegex(text,regex,i_txt,i_rgx + 1);
		else if(*i_rgx == *i_txt || *i_rgx == '?')
		{
			++i_rgx;
			++i_txt;
		}
		else
			return false;
	}
}

bool TiledMap2P5DFileParser::matchingRegex(
	const std::string &text,const std::string &regex)
{
	return matchingRegex(text,regex,text.begin(),regex.begin());
}

void TiledMap2P5DFileParser::deleteHeadSpace(std::string &buff)
{
	size_t pos;
	while((pos = buff.find_first_of(" \t")) == 0)
	{
		buff.erase(buff.begin());
		if(buff.empty())
			break;
	}
}

bool TiledMap2P5DFileParser::findStr(
	const std::string &string,const std::string &substring)
{
	return string.find(substring) != NOT_MACHING_FLAG;
}


std::string TiledMap2P5DFileParser::findTagInLine(const std::string &line)
{
	if(findStr(line,TAG_GRID_WIDTH))
		return TAG_GRID_WIDTH;
	else if(findStr(line,TAG_GRID_HEIGHT))
		return TAG_GRID_HEIGHT;
	else if(findStr(line,TAG_ARCHITECTURE))
		return TAG_ARCHITECTURE;
	else if(findStr(line,TAG_TILESHEET_INFORMATION))
		return TAG_TILESHEET_INFORMATION;
	else if(findStr(line,TAG_TILED_LAYER_INFORMATION))
		return TAG_TILED_LAYER_INFORMATION;
	else if(findStr(line,TAG_TILED_LAYER_BUNDLER_INFORMATION))
		return TAG_TILED_LAYER_BUNDLER_INFORMATION;
	else if(findStr(line,TAG_NAME))
		return TAG_NAME;
	else
		return std::string("");
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

const std::vector<std::string>& TiledMapInfo::getArchitecture()
{
	return _architecture;
}

void TiledMapInfo::addElementToArchitecture(std::string element)
{
	_architecture.push_back(element);
}

/**
 * Protected
 */

TiledMapInfo::TiledMapInfo()
:_mapGridWidth(0)
,_mapGridHeight(0)
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

const std::vector<std::string>& TiledLayerBundlerInfo::getArchitecture()
{
	return _architecture;
}

void TiledLayerBundlerInfo::addElementToArchitecture(std::string element)
{
	_architecture.push_back(element);
}

/**
 * Protected
 */

TiledLayerBundlerInfo::TiledLayerBundlerInfo()
:_name("")
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
:_layerName("")
,_pathTerrainFile("")
,_tilesheetName("")
,_isVisible(false)
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

TilesheetInfo* TilesheetInfo::createWithTileTypeNum(const size_t num)
{
	auto ref = new TilesheetInfo();
	if(ref->initWithTileTypeNum(num))
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
:_fileName("")
,_sheetName("")
,_numTileType(0)
,_tileSize(0,0)
,_textureRects(nullptr)
{}

TilesheetInfo::~TilesheetInfo()
{
	if(_textureRects != nullptr)
		delete [] _textureRects;
}

bool TilesheetInfo::initWithTileTypeNum(const size_t num)
{
	_textureRects = new Rect[num];

	return true;
}
