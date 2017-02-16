#include "TiledMap2P5DFileParser.h"
#include <fstream>
#include <iostream>

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
const std::string VALUE_NAME("<@Name>");
const std::string VALUE_BOOL("<@Boolean>");
const std::string VALUE_SIZE("<@Size>");
const std::string VALUE_RECT("<@Rect>");

//Formats for sscanf()
// const std::string FORMAT_SIZE_T(VALUE_SIZE_T + std::string("%zu"));
// const std::string FORMAT_PATH(VALUE_PATH + std::string("%s"));
// const std::string FORMAT_NAME(VALUE_NAME + std::string("%s"));
// const std::string FORMAT_BOOL(VALUE_BOOL + std::string("%s"));
// const std::string FORMAT_SIZE(VALUE_SIZE + std::string("%lf %lf"));
// const std::string FORMAT_RECT(VALUE_RECT + std::string("%lf %lf %lf %lf"));

//Tags
const std::string TAG_GRID_WIDTH("<GridWidth");
const std::string TAG_GRID_HEIGHT("<GridHeight");
const std::string TAG_TILED_LAYER_INFORMATION("<TiledLayerInformation");
const std::string TAG_TILED_LAYER_BUNDLER_INFORMATION("<TiledLayerBundlerInformation");
const std::string TAG_TILESHEET_INFORMATION("<TilesheetInformation");
const std::string TAG_ARCHITECTURE("<Architecture");
const std::string TAG_LAYER_NAME("<LayerName");
const std::string TAG_SHEET_NAME("<SheetName");
const std::string TAG_VISIBLE("<Visible");
const std::string TAG_TERRAIN("<Terrain");
const std::string TAG_NUM_OF_TILE_TYPES("<NumOfTileTypes");
const std::string TAG_TILE_SIZE("<TileSize");
const std::string TAG_RESOURCE("<Resource");
const std::string TAG_TEXTURE_RECTS("<TextureRects");

//New
//Modifiers
const std::string MOD_OBJECT("$");
const std::string MOD_VALUE("@");
const std::string MOD_INCLUDE("<include>");

//Object elements
const std::string OBJ_TILED_MAP_INFO("$(TiledMapInfo");
const std::string OBJ_TILED_LAYER_INFO("$(TiledLayerInfo");
const std::string OBJ_TILED_LAYER_BUNDLER_INFO("$(TiledLayerBundlerInfo");
const std::string OBJ_TILESHEET_INFO("$(TilesheetInfo");

//Elements
const std::string ELM_GRID_WIDTH("(GridWidth");
const std::string ELM_GRID_HEIGHT("(GridHeight");
const std::string ELM_ARCHITECTURE("(Architecture");
const std::string ELM_LAYER_NAME("(LayerName");
const std::string ELM_SHEET_NAME("(SheetName");
const std::string ELM_TERRAIN("(Terrain");
const std::string ELM_VISIBLE("(Visible");
const std::string ELM_RESOURCE("(Resource");
const std::string ELM_TILE_SIZE("(TileSize");
const std::string ELM_NUM_OF_TILE_TYPES("(NumOfTileTypes");
const std::string ELM_TEXTURE_RECTS("(TextureRects");

//Value elements
const std::string RGX_VAL_SIZE_T("@(sizeT:*");
const std::string RGX_VAL_STRING("@(string:*");
const std::string RGX_VAL_BOOL("@(bool:*");
const std::string RGX_VAL_CC_RECT("@(ccRect:*");
const std::string RGX_VAL_CC_SIZE("@(ccSize:*");

//For sscanf
const std::string FORMAT_SIZE_T("@(sizeT:%zu)");
const std::string FORMAT_STRING("@(string:%s");
const std::string FORMAT_BOOL("@(bool:%s");
const std::string FORMAT_CC_RECT("@(ccRect:%f,%f,%f,%f");
const std::string FORMAT_CC_SIZE("@(ccSize:%f,%f");

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
	// this->parseOriginFile(
		// origin,tiledMapInfo,mapTiledLayerBundlerInfo,mapTiledLayerInfo,mapTilesheetInfo);
	parseWithPath(origin);

	return true;
}

/**
 * Private
 */

bool TiledMap2P5DFileParser::parseWithPath(std::string path)
{
	std::ifstream stream(path,std::ios::in);
	std::string line;
	std::vector<std::string> tokens;

	if(stream.eof())
		return false;

	//Create tokens
	while(!stream.eof())
	{
		std::getline(stream,line);
		// std::cout << line << '\n';

		//Skip a comment
		if(line.find("##") != std::string::npos)
			continue;

		deleteSpace(line);
		tokenize(line,tokens);
	}

	//Create objects using tokens
	for(auto token = tokens.begin(); token != tokens.end(); ++token)
	{
		// std::cout << "TOKEN >> " << *token << '\n';
		if(token->find(MOD_OBJECT) != std::string::npos)
		{
			if(*token == OBJ_TILED_MAP_INFO)
			{
				auto info = tokenToTiledMapInfo(tokens,++token);
			}
			else if(*token == OBJ_TILED_LAYER_INFO)
			{
				auto info = tokenToTiledLayerInfo(tokens,++token);
			}
			else if(*token == OBJ_TILED_LAYER_BUNDLER_INFO)
			{
				auto info = tokenToTiledLayerBundlerInfo(tokens,++token);
			}
			else if(*token == OBJ_TILESHEET_INFO)
			{
				auto info = tokenToTilesheetInfo(tokens,++token);
			}
		}
		else if(*token == MOD_INCLUDE)
		{
			//Parse additional file
			//Recursive call
			parseWithPath(tokenToString(*(++token)));
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

TiledMapInfo* TiledMap2P5DFileParser::tokenToTiledMapInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TiledMapInfo::create();
	std::string current_element("");


	for(hier = 1; hier > 0 && itr != tokens.end(); ++itr)
	{
		// std::cout << "[" << hier << "]TOKEN IN TILEDMAPINFO >> " << *itr << '\n';
		//Value (size_t) element
		if(matchingRegex(*itr,RGX_VAL_SIZE_T))
		{
			++hier;
			if(current_element == ELM_GRID_WIDTH)
				info->setMapGridWidth(tokenToSizeT(*itr));
			else if(current_element == ELM_GRID_HEIGHT)
				info->setMapGridHeight(tokenToSizeT(*itr));
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

	return info;
}

TiledLayerInfo* TiledMap2P5DFileParser::tokenToTiledLayerInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TiledLayerInfo::create();
	std::string current_element("");


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

	return info;
}

TiledLayerBundlerInfo* TiledMap2P5DFileParser::tokenToTiledLayerBundlerInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TiledLayerBundlerInfo::create();
	std::string current_element("");


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

	return info;
}

TilesheetInfo* TiledMap2P5DFileParser::tokenToTilesheetInfo(
	const std::vector<std::string>& tokens,
	std::vector<std::string>::iterator& itr)
{
	int hier;
	auto info = TilesheetInfo::create();
	std::string current_element("");


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

	return info;
}

void TiledMap2P5DFileParser::debugLogForTiledMapInfo(TiledMapInfo* info)
{
	// DEBUG
	log("\n**\tDEBUG OF parseOriginFile()\t**");
	log("TiledMapInfo::GridWidth::%zu",info->getMapGridWidth());
	log("TiledMapInfo::GridHeight::%zu",info->getMapGridHeight());
	for(auto str:info->getArchitecture())
	{
		log("TiledMapInfo::Architecture::%s",str.c_str());
	}
	log("\t**\tEND\t**\n");
}

void TiledMap2P5DFileParser::debugLogForTiledLayerInfo(TiledLayerInfo* info)
{
	// DEBUG
	log("\n**\tDEBUG OF parseTiledLayerInfoFile()\t**");
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
	log("\n**\tDEBUG OF parseTiledLayerBundlerInfoFile()\t**");
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
	log("\n**\tDEBUG OF parseTilesheetInfoFile()\t**");
	log("TilesheetInfo::SheetName::%s",info->getSheetName().c_str());
	log("TilesheetInfo::Resource::%s",info->getFileName().c_str());
	log("TilesheetInfo::TileSize::%.5f,%.5f",info->getTileSize().width,info->getTileSize().height);
	log("TilesheetInfo::NumOfTileTypes::%zu",info->getNumOfTileType());
	for(auto rct:info->getTexturectAsVector())
	{
		log("TilesheetInfo::TextureRect::%.5f,%.5f,%.5f,%.5f",rct.origin.x,rct.origin.y,rct.size.width,rct.size.height);
	}
	log("\t**\tEND\t**\n");
}

// bool TiledMap2P5DFileParser::parseOriginFile(
// 	const std::string& path,
// 	TiledMapInfo* tiledMapInfo,
// 	cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
// 	cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
// 	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
// {
// 	std::ifstream i_file_stream(path,std::ios::in);
// 	std::string line_buff;
// 	std::string current_tag("");
// 	char buff[SSCANF_BUFF_LEN];
// 	size_t tmp;
// 	//If any information file couldn't be loaded,this is false.
// 	bool flag = true;
//
//
// 	if(i_file_stream.eof())
// 	{
// 		log("WORNIG!! >> The file '%s' has nothing to be read.",path.c_str());
// 		return false;
// 	}
//
// 	while(!i_file_stream.eof())
// 	{
// 		//Parse the map-info file
// 		//Read a line
// 		std::getline(i_file_stream,line_buff);
// 		deleteHeadSpace(line_buff);
//
// 		//Skip a comment
// 		if(findStr(line_buff,"#") || line_buff == "\n")
// 			continue;
// 		//The line is end of a tag
// 		else if(matchingRegex(line_buff,REGEX_VALUE))
// 		{
// 			//Get a value for a tag.
// 			if(findStr(line_buff,VALUE_SIZE_T))
// 			{
// 				//A value type of size_t
// 				sscanf(line_buff.c_str(),FORMAT_SIZE_T.c_str(),&tmp);
//
// 				if(current_tag == TAG_GRID_WIDTH)
// 				{
// 					tiledMapInfo->setMapGridWidth(tmp);
// 				}
// 				else if(current_tag == TAG_GRID_HEIGHT)
// 				{
// 					tiledMapInfo->setMapGridHeight(tmp);
// 				}
// 			}
// 			else if(findStr(line_buff,VALUE_PATH))
// 			{
// 				//A value type of file-path
// 				sscanf(line_buff.c_str(),FORMAT_PATH.c_str(),buff);
//
// 				if(current_tag == TAG_TILED_LAYER_INFORMATION)
// 				{
// 					TiledLayerInfo* tli = parseTiledLayerInfoFile(std::string(buff));
// 					if(tli != nullptr)
// 						mapTiledLayerInfo.insert(tli->getLayerName(),tli);
// 					else
// 						flag = false;
// 				}
// 				else if(current_tag == TAG_TILED_LAYER_BUNDLER_INFORMATION)
// 				{
// 					TiledLayerBundlerInfo* bli =
// 						parseTiledLayerBundlerInfoFile(std::string(buff));
// 					if(bli != nullptr)
// 						mapTiledLayerBundlerInfo.insert(bli->getName(),bli);
// 					else
// 						flag = false;
// 				}
// 				else if(current_tag == TAG_TILESHEET_INFORMATION)
// 				{
// 					flag = parseTilesheetInfoFile(std::string(buff),mapTilesheetInfo);
// 				}
// 			}
// 			else if(findStr(line_buff,VALUE_NAME))
// 			{
// 				//A value type of layer-name
// 				sscanf(line_buff.c_str(),FORMAT_NAME.c_str(),buff);
//
// 				if(current_tag == TAG_ARCHITECTURE)
// 				{
// 					tiledMapInfo->addElementToArchitecture(std::string(buff));
// 				}
// 			}
// 		}
// 		//The line is end of a tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_END))
// 		{
// 			current_tag.erase(current_tag.begin(),current_tag.end());
// 		}
// 		//The line is begining of a tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_BEGIN))
// 		{
// 			current_tag = findTagInLine(line_buff);
// 		}
// 	}
//
// 	// DEBUG
// 	log("\n**\tDEBUG OF parseOriginFile()\t**");
// 	log("TiledMapInfo::GridWidth::%zu",tiledMapInfo->getMapGridWidth());
// 	log("TiledMapInfo::GridHeight::%zu",tiledMapInfo->getMapGridHeight());
// 	for(auto str:tiledMapInfo->getArchitecture())
// 	{
// 		log("TiledMapInfo::Architecture::%s",str.c_str());
// 	}
// 	log("\t**\tEND\t**\n");
//
// 	return flag;
// }

// TiledLayerBundlerInfo* TiledMap2P5DFileParser::parseTiledLayerBundlerInfoFile(
// 	const std::string& path)
// {
// 	std::ifstream i_file_stream(path,std::ios::in);
// 	std::string line_buff;
// 	std::string current_tag("");
// 	char buff[SSCANF_BUFF_LEN];
// 	size_t tmp;
//
//
// 	if(i_file_stream.eof())
// 	{
// 		log("WORNIG!! >> The file '%s' has nothing to be read.",path.c_str());
// 		return nullptr;
// 	}
//
// 	auto info = TiledLayerBundlerInfo::create();
//
// 	if(info == nullptr)
// 		return nullptr;
//
// 	while(!i_file_stream.eof())
// 	{
// 		//Read line
// 		std::getline(i_file_stream,line_buff);
// 		deleteHeadSpace(line_buff);
//
// 		//Skip a comment and empty line.
// 		if(findStr(line_buff,"#") || line_buff == "\n")
// 			continue;
// 		//Value tag
// 		else if(matchingRegex(line_buff,REGEX_VALUE))
// 		{
// 			if(findStr(line_buff,VALUE_NAME))
// 			{
// 				sscanf(line_buff.c_str(),FORMAT_NAME.c_str(),buff);
// 				if(current_tag == TAG_LAYER_NAME)
// 				{
// 					info->setName(std::string(buff));
// 				}
// 				else if(current_tag == TAG_ARCHITECTURE)
// 				{
// 					info->addElementToArchitecture(std::string(buff));
// 				}
// 			}
// 		}
// 		//End tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_END))
// 		{
// 			current_tag.erase(current_tag.begin(),current_tag.end());
// 		}
// 		//Begin tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_BEGIN))
// 		{
// 			current_tag = findTagInLine(line_buff);
// 		}
// 	}
//
// 	// DEBUG
// 	log("\n**\tDEBUG OF parseTiledLayerBundlerInfoFile()\t**");
// 	log("TiledLayerBundlerInfo::Name::%s",info->getName().c_str());
// 	for(auto str:info->getArchitecture())
// 	{
// 		log("TiledLayerBundlerInfo::Architecture::%s",str.c_str());
// 	}
// 	log("\t**\tEND\t**\n");
//
// 	return info;
// }
//
// TiledLayerInfo* TiledMap2P5DFileParser::parseTiledLayerInfoFile(const std::string& path)
// {
// 	std::ifstream i_file_stream(path,std::ios::in);
// 	std::string line_buff;
// 	std::string current_tag("");
// 	char buff[SSCANF_BUFF_LEN];
// 	size_t tmp;
//
//
// 	if(i_file_stream.eof())
// 	{
// 		log("WORNIG!! >> The file '%s' has nothing to be read.",path.c_str());
// 		return nullptr;
// 	}
//
// 	auto info = TiledLayerInfo::create();
//
// 	if(info == nullptr)
// 		return nullptr;
//
// 	while(!i_file_stream.eof())
// 	{
// 		//Read line
// 		std::getline(i_file_stream,line_buff);
// 		deleteHeadSpace(line_buff);
//
// 		//Skip a comment and empty line.
// 		if(findStr(line_buff,"#") || line_buff == "\n")
// 			continue;
// 		//Value tag
// 		else if(matchingRegex(line_buff,REGEX_VALUE))
// 		{
// 			if(findStr(line_buff,VALUE_NAME))
// 			{
// 				sscanf(line_buff.c_str(),FORMAT_NAME.c_str(),buff);
// 				if(current_tag == TAG_LAYER_NAME)
// 				{
// 					info->setLayerName(std::string(buff));
// 				}
// 				else if(current_tag == TAG_SHEET_NAME)
// 				{
// 					info->setTilesheetName(std::string(buff));
// 				}
// 			}
// 			else if(findStr(line_buff,VALUE_BOOL))
// 			{
// 				sscanf(line_buff.c_str(),FORMAT_BOOL.c_str(),buff);
// 				std::string boolean = buff;
// 				if(current_tag == TAG_VISIBLE)
// 				{
// 					info->setIsVisible((boolean == "true") ? true : false);
// 				}
// 			}
// 			else if(findStr(line_buff,VALUE_PATH))
// 			{
// 				sscanf(line_buff.c_str(),FORMAT_PATH.c_str(),buff);
// 				if(current_tag == TAG_TERRAIN)
// 				{
// 					info->setPathTerrainFile(std::string(buff));
// 				}
// 			}
// 		}
// 		//End tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_END))
// 		{
// 			current_tag.erase(current_tag.begin(),current_tag.end());
// 		}
// 		//Begin tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_BEGIN))
// 		{
// 			current_tag = findTagInLine(line_buff);
// 		}
// 	}
//
// 	// DEBUG
// 	log("\n**\tDEBUG OF parseTiledLayerInfoFile()\t**");
// 	log("TiledLayerInfo::LayerName::%s",info->getLayerName().c_str());
// 	log("TiledLayerInfo::Visible::%s",(info->getIsVisible()) ? "true" : "false");
// 	log("TiledLayerInfo::Terrain::%s",info->getPathTerrainFile().c_str());
// 	log("TiledLayerInfo::SheetName::%s",info->getTilesheetName().c_str());
// 	log("\t**\tEND\t**\n");
//
// 	return info;
// }
//
// bool TiledMap2P5DFileParser::parseTilesheetInfoFile(
// 	const std::string& path,
// 	cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo)
// {
// 	std::ifstream i_file_stream(path,std::ios::in);
// 	std::string line_buff;
// 	std::string current_tag("");
// 	char buff[SSCANF_BUFF_LEN];
// 	size_t sizeT;
// 	Size size;
// 	Rect rect;
//
// 	return true;
// 	if(i_file_stream.eof())
// 	{
// 		log("WORNIG!! >> The file '%s' has nothing to be read.",path.c_str());
// 		return nullptr;
// 	}
//
// 	auto info = TiledLayerBundlerInfo::create();
//
// 	if(info == nullptr)
// 		return nullptr;
//
// 	while(!i_file_stream.eof())
// 	{
// 		//Read line
// 		std::getline(i_file_stream,line_buff);
// 		deleteHeadSpace(line_buff);
//
// 		//Skip a comment and empty line.
// 		if(findStr(line_buff,"#") || line_buff == "\n")
// 			continue;
// 		//Value tag
// 		else if(matchingRegex(line_buff,REGEX_VALUE))
// 		{
//
// 		}
// 		//End tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_END))
// 		{
//
// 		}
// 		//Begin tag
// 		else if(matchingRegex(line_buff,REGEX_TAG_BEGIN))
// 		{
//
// 		}
// 	}
// 	return true;
// }

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
	else if(findStr(line,TAG_LAYER_NAME))
		return TAG_LAYER_NAME;
	else if(findStr(line,TAG_SHEET_NAME))
		return TAG_SHEET_NAME;
	else if(findStr(line,TAG_TERRAIN))
		return TAG_TERRAIN;
	else if(findStr(line,TAG_VISIBLE))
		return TAG_VISIBLE;
	else if(findStr(line,TAG_TILE_SIZE))
		return TAG_TILE_SIZE;
	else if(findStr(line,TAG_NUM_OF_TILE_TYPES))
		return TAG_NUM_OF_TILE_TYPES;
	else if(findStr(line,TAG_RESOURCE))
		return TAG_RESOURCE;
	else if(findStr(line,TAG_TEXTURE_RECTS))
		return TAG_TEXTURE_RECTS;
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

const std::vector<Rect>& TilesheetInfo::getTexturectAsVector()
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
