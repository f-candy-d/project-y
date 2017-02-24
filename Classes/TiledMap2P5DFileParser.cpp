#include "TiledMap2P5DFileParser.h"
#include <fstream>
#include <iostream>

using namespace cocos2d;

/**
 * TiledMap2P5DFileParser,TiledMapInfo,TiledLayerInfo,TiledLayerBundlerInfo,
 * TilesheetInfo classes are member of the namespace TM25Component.
 */
namespace TM25Component {

	//The following constant strings are only used in TiledMap2P5DFileParser class
	namespace {

	//For sscanf()
	static const size_t SSCANF_BUFF_LEN =  64;

	//Modifiers
	static const std::string MOD_OBJECT("$");
	static const std::string MOD_VALUE("@");
	static const std::string MOD_INCLUDE("<include>");

	//Object elements
	static const std::string OBJ_TILED_MAP_INFO("$(TiledMapInfo");
	static const std::string OBJ_TILED_LAYER_INFO("$(TiledLayerInfo");
	static const std::string OBJ_TILED_LAYER_BUNDLER_INFO("$(TiledLayerBundlerInfo");
	static const std::string OBJ_TILESHEET_INFO("$(TilesheetInfo");

	//Elements
	static const std::string ELM_GRID_WIDTH("(GridWidth");
	static const std::string ELM_GRID_HEIGHT("(GridHeight");
	static const std::string ELM_ARCHITECTURE("(Architecture");
	static const std::string ELM_LAYER_NAME("(LayerName");
	static const std::string ELM_SHEET_NAME("(SheetName");
	static const std::string ELM_TERRAIN("(Terrain");
	static const std::string ELM_VISIBLE("(Visible");
	static const std::string ELM_RESOURCE("(Resource");
	static const std::string ELM_TILE_SIZE("(TileSize");
	static const std::string ELM_NUM_OF_TILE_TYPES("(NumOfTileTypes");
	static const std::string ELM_NUM_OF_CHANKS("(NumOfChanks");
	static const std::string ELM_TEXTURE_RECTS("(TextureRects");

	//Value elements
	static const std::string RGX_VAL_SIZE_T("@(sizeT:*");
	static const std::string RGX_VAL_STRING("@(string:*");
	static const std::string RGX_VAL_BOOL("@(bool:*");
	static const std::string RGX_VAL_CC_RECT("@(ccRect:*");
	static const std::string RGX_VAL_CC_SIZE("@(ccSize:*");

	//Formats or sscanf()
	static const std::string FORMAT_SIZE_T("@(sizeT:%zu)");
	static const std::string FORMAT_STRING("@(string:%s");
	static const std::string FORMAT_BOOL("@(bool:%s");
	static const std::string FORMAT_CC_RECT("@(ccRect:%f,%f,%f,%f");
	static const std::string FORMAT_CC_SIZE("@(ccSize:%f,%f");

	} /* unnamed namespace */

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
		origin,
		tiledMapInfo,
		mapTiledLayerBundlerInfo,
		mapTiledLayerInfo,
		mapTilesheetInfo))
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
	if(!parseWithArgs(
		origin,
		tiledMapInfo,
		mapTiledLayerBundlerInfo,
		mapTiledLayerInfo,
		mapTilesheetInfo))
	{
		std::cout << "WARNIG!! :: Something has been wrong in persing files." << '\n';
		return false;
	}

	return true;
}

/**
 * Private
 */

bool TiledMap2P5DFileParser::parseWithArgs(
	const std::string& path,
	TiledMapInfo* tiledMapInfo,
	Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
	Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
	Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
{
	std::ifstream stream(path,std::ios::in);
	std::string line;
	std::vector<std::string> tokens;

	std::cout << "Parse file >> " << path << '\n';

	if(stream.eof())
		return false;

	//Read by line and create tokens
	while(!stream.eof())
	{
		std::getline(stream,line);

		//Skip a comment
		if(line.find("##") != std::string::npos)
			continue;

		deleteSpace(line);
		tokenize(line,tokens);
	}

	//Create objects using tokens
	for(auto token = tokens.begin(); token != tokens.end(); ++token)
	{
		//Modifier $
		if(token->find(MOD_OBJECT) != std::string::npos)
		{
			//Create objects
			if(*token == OBJ_TILED_MAP_INFO)
			{
				if(tiledMapInfo == nullptr)
					tiledMapInfo = TiledMapInfo::create();
				tokenToTiledMapInfo(tokens,++token,tiledMapInfo);
			}
			else if(*token == OBJ_TILED_LAYER_INFO)
			{
				auto info = tokenToTiledLayerInfo(tokens,++token);
				mapTiledLayerInfo.insert(info->getLayerName(),info);
			}
			else if(*token == OBJ_TILED_LAYER_BUNDLER_INFO)
			{
				auto info = tokenToTiledLayerBundlerInfo(tokens,++token);
				mapTiledLayerBundlerInfo.insert(info->getName(),info);
			}
			else if(*token == OBJ_TILESHEET_INFO)
			{
				auto info = tokenToTilesheetInfo(tokens,++token);
				mapTilesheetInfo.insert(info->getSheetName(),info);
			}
		}
		//Modifier <include>
		else if(*token == MOD_INCLUDE)
		{
			//Parse additional file
			//Recursive call
			if(!parseWithArgs(
				tokenToString(*(++token)),
				tiledMapInfo,
				mapTiledLayerBundlerInfo,
				mapTiledLayerInfo,
				mapTilesheetInfo))
				return false;
		}
	}

	//Clear tokens
	std::vector<std::string>().swap(tokens);

	return true;
}

void TiledMap2P5DFileParser::deleteSpace(std::string& text)
{
	//Delete all white-spaces and tabs
	size_t pos;
	while((pos = text.find_first_of(" \t")) != std::string::npos)
	{
		text.erase(pos, 1);
	}
}

void TiledMap2P5DFileParser::tokenize(std::string text,std::vector<std::string>& tokens)
{
	//Tokenize
	size_t left = 0;
	std::string substr;
	int boolean;

	for(size_t pos = 1;
		(pos = text.find_first_of("()",pos)) != std::string::npos;
		pos++)
	{
		boolean = !(text.at(pos - 1) != MOD_VALUE[0] && text.at(pos - 1) != MOD_OBJECT[0]);

		substr = text.substr(left,pos - left - boolean);
		if(!substr.empty())
			tokens.push_back(substr);

		left = pos - boolean;
	}

	//Get the remainig token
	substr = text.substr(left,text.size() - left);
	if(!substr.empty())
		tokens.push_back(substr);
}

std::string TiledMap2P5DFileParser::tokenToString(std::string token)
{
	char buff[SSCANF_BUFF_LEN];

	sscanf(token.c_str(),FORMAT_STRING.c_str(),buff);

	return std::string(buff);
}

size_t TiledMap2P5DFileParser::tokenToSizeT(std::string token)
{
	size_t sizeT = 0;

	sscanf(token.c_str(),FORMAT_SIZE_T.c_str(),&sizeT);

	return sizeT;
}

bool TiledMap2P5DFileParser::tokenToBool(std::string token)
{
	char buff[SSCANF_BUFF_LEN];

	sscanf(token.c_str(),FORMAT_BOOL.c_str(),buff);

	return (std::string(buff) == "true");
}

Size TiledMap2P5DFileParser::tokenToCCSize(std::string token)
{
	Size size(0,0);

	sscanf(token.c_str(),FORMAT_CC_SIZE.c_str(),&size.width,&size.height);

	return size;
}

Rect TiledMap2P5DFileParser::tokenToCCRect(std::string token)
{
	Rect rect(0,0,0,0);

	sscanf(token.c_str(),FORMAT_CC_RECT.c_str(),
		&rect.origin.x,&rect.origin.y,&rect.size.width,&rect.size.height);

	return rect;
}

void TiledMap2P5DFileParser::tokenToTiledMapInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr,
	TiledMapInfo* info)
{
	int hier;
	std::string current_element("");

	//To prevent release of the object while parsing the file
	info->retain();

	for(hier = 1; hier > 0 && itr != tokens.end(); ++itr)
	{
		//Value (size_t) element
		if(matchingRegex(*itr,RGX_VAL_SIZE_T))
		{
			++hier;
			if(current_element == ELM_GRID_WIDTH)
				info->setMapGridWidth(tokenToSizeT(*itr));
			else if(current_element == ELM_GRID_HEIGHT)
				info->setMapGridHeight(tokenToSizeT(*itr));
			else if(current_element == ELM_NUM_OF_CHANKS)
				info->setNumOfChanks(tokenToSizeT(*itr));
			else if(current_element == ELM_TILE_SIZE)
				info->setTileOneSide(tokenToSizeT(*itr));
		}
		//Value (std::string) element
		else if(matchingRegex(*itr,RGX_VAL_STRING))
		{
			++hier;
			if(current_element == ELM_ARCHITECTURE)
				info->addElementToArchitecture(tokenToString(*itr));
		}
		//An element
		else if(itr->at(0) == '(')
		{
			++hier;
			current_element = *itr;
		}
		//End of a element
		else if(itr->at(0) == ')')
		{
			hier--;
		}
	}

	//Undo last incrementation of iterator in for loop.
	--itr;

	//debug
	debugLogForTiledMapInfo(info);

	//Release
	info->release();
}

TiledLayerInfo* TiledMap2P5DFileParser::tokenToTiledLayerInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TiledLayerInfo::create();
	std::string current_element("");

	if(info == nullptr)
		return nullptr;
	//To prevent release of the object while parsing the file
	info->retain();

	for(hier = 1; hier > 0 && itr != tokens.end(); ++itr)
	{
		//Value (bool) element
		if(matchingRegex(*itr,RGX_VAL_BOOL))
		{
			++hier;
			if(current_element == ELM_VISIBLE)
				info->setIsVisible(tokenToBool(*itr));
		}
		//Value (std::string) element
		else if(matchingRegex(*itr,RGX_VAL_STRING))
		{
			++hier;
			if(current_element == ELM_LAYER_NAME)
				info->setLayerName(tokenToString(*itr));
			else if(current_element == ELM_SHEET_NAME)
				info->setTilesheetName(tokenToString(*itr));
			else if(current_element == ELM_TERRAIN)
				info->setPathTerrainFile(tokenToString(*itr));
		}
		//An element
		else if(itr->at(0) == '(')
		{
			++hier;
			current_element = *itr;
		}
		//End of a element
		else if(itr->at(0) == ')')
		{
			hier--;
		}
	}

	//Undo last incrementation of iterator in for loop.
	--itr;

	//debug
	debugLogForTiledLayerInfo(info);
	//Release
	info->release();

	return info;
}

TiledLayerBundlerInfo* TiledMap2P5DFileParser::tokenToTiledLayerBundlerInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TiledLayerBundlerInfo::create();
	std::string current_element("");

	if(info == nullptr)
		return nullptr;
	//To prevent release of the object while parsing the file
	info->retain();

	for(hier = 1; hier > 0 && itr != tokens.end(); ++itr)
	{
		//Value (std::string) element
		if(matchingRegex(*itr,RGX_VAL_STRING))
		{
			++hier;
			if(current_element == ELM_LAYER_NAME)
				info->setName(tokenToString(*itr));
			else if(current_element == ELM_ARCHITECTURE)
				info->addElementToArchitecture(tokenToString(*itr));
		}
		//An element
		else if(itr->at(0) == '(')
		{
			++hier;
			current_element = *itr;
		}
		//End of a element
		else if(itr->at(0) == ')')
		{
			hier--;
		}
	}

	//Undo last incrementation of iterator in for loop.
	--itr;

	//debug
	debugLogForTiledLayerBundlerInfo(info);
	//Release
	info->release();

	return info;
}

TilesheetInfo* TiledMap2P5DFileParser::tokenToTilesheetInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TilesheetInfo::create();
	std::string current_element("");

	if(info == nullptr)
		return nullptr;
	//To prevent release of the object while parsing the file
	info->retain();

	for(hier = 1; hier > 0 && itr != tokens.end(); ++itr)
	{
		//Value (std::string) element
		if(matchingRegex(*itr,RGX_VAL_STRING))
		{
			++hier;
			if(current_element == ELM_SHEET_NAME)
				info->setSheetName(tokenToString(*itr));
			else if(current_element == ELM_RESOURCE)
				info->setFileName(tokenToString(*itr));
		}
		else if(matchingRegex(*itr,RGX_VAL_SIZE_T))
		{
			++hier;
			if(current_element == ELM_NUM_OF_TILE_TYPES)
				info->setNumOfTileType(tokenToSizeT(*itr));
		}
		else if(matchingRegex(*itr,RGX_VAL_CC_SIZE))
		{
			++hier;
			if(current_element == ELM_TILE_SIZE)
				info->setTileSize(tokenToCCSize(*itr));
		}
		else if(matchingRegex(*itr,RGX_VAL_CC_RECT))
		{
			++hier;
			if(current_element == ELM_TEXTURE_RECTS)
				info->addTextureRect(tokenToCCRect(*itr));
		}
		//An element
		else if(itr->at(0) == '(')
		{
			++hier;
			current_element = *itr;
		}
		//End of a element
		else if(itr->at(0) == ')')
		{
			hier--;
		}
	}

	//Undo last incrementation of iterator in for loop.
	--itr;

	//debug
	debugLogForTilesheetInfo(info);
	//Release
	info->release();

	return info;
}

void TiledMap2P5DFileParser::debugLogForTiledMapInfo(TiledMapInfo* info)
{
	// DEBUG
	log("\n**\tDEBUG LOG OF TiledMapInfo\t**");
	log("TiledMapInfo::GridWidth::%zu",info->getMapGridWidth());
	log("TiledMapInfo::GridHeight::%zu",info->getMapGridHeight());
	log("TiledMapInfo::NumOfChanks::%zu",info->getNumOfChanks());
	for(auto str:info->getArchitecture())
	{
		log("TiledMapInfo::Architecture::%s",str.c_str());
	}
	log("\t**\tEND\t**\n");
}

void TiledMap2P5DFileParser::debugLogForTiledLayerInfo(TiledLayerInfo* info)
{
	// DEBUG
	log("\n**\tDEBUG LOG OF TiledLayerInfo\t**");
	log("TiledLayerInfo::LayerName::%s",info->getLayerName().c_str());
	log("TiledLayerInfo::Visible::%s",(info->getIsVisible()) ? "true" : "false");
	log("TiledLayerInfo::Terrain::%s",info->getPathTerrainFile().c_str());
	log("TiledLayerInfo::SheetName::%s",info->getTilesheetName().c_str());
	log("\t**\tEND\t**\n");
}

void TiledMap2P5DFileParser::debugLogForTiledLayerBundlerInfo(
	TiledLayerBundlerInfo* info)
{
	// DEBUG
	log("\n**\tDEBUG LOG OF TiledLayerBundlerInfo\t**");
	log("TiledLayerBundlerInfo::Name::%s",info->getName().c_str());
	for(auto str:info->getArchitecture())
	{
		log("TiledLayerBundlerInfo::Architecture::%s",str.c_str());
	}
	log("\t**\tEND\t**\n");
}

void TiledMap2P5DFileParser::debugLogForTilesheetInfo(TilesheetInfo* info)
{
	// DEBUG
	log("\n**\tDEBUG LOG OF TilesheetInfo\t**");
	log("TilesheetInfo::SheetName::%s",info->getSheetName().c_str());
	log("TilesheetInfo::Resource::%s",info->getFileName().c_str());
	log("TilesheetInfo::TileSize::%.5f,%.5f",info->getTileSize().width,info->getTileSize().height);
	log("TilesheetInfo::NumOfTileTypes::%zu",info->getNumOfTileType());
	for(auto rct:info->getTextureRectAsVector())
	{
		log("TilesheetInfo::TextureRect::%.5f,%.5f,%.5f,%.5f",rct.origin.x,rct.origin.y,rct.size.width,rct.size.height);
	}
	log("\t**\tEND\t**\n");
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
,_numOfChanks(0)
,_tileOneSide(0)
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

void TilesheetInfo::addTextureRect(Rect rect)
{
	_textureRects.push_back(rect);
}

const Rect TilesheetInfo::getTextureRectForType(size_t type)
{
	return _textureRects.at(type);
}

const std::vector<Rect>& TilesheetInfo::getTextureRectAsVector()
{
	return _textureRects;
}

size_t TilesheetInfo::getNumOfTileType()
{
	return _numTileType;
}

void TilesheetInfo::setNumOfTileType(size_t num)
{
	_numTileType = num;
	//Set the capacity
	_textureRects.reserve(num);
}

/**
 * Protected
 */

TilesheetInfo::TilesheetInfo()
:_fileName("")
,_sheetName("")
,_tileSize(0,0)
,_numTileType(0)
{}

TilesheetInfo::~TilesheetInfo()
{}

bool TilesheetInfo::init()
{
	return true;
}

} /* namespace TM25Component */
