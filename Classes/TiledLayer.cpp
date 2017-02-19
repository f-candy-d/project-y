#include "Chank.h"
#include "TiledMap2P5DFileParser.h"
#include "TiledLayer.h"
#include <fstream>
#include <iostream>

USING_NS_CC;

namespace TM25Component {

/**
 * Public
 */
TiledLayer* TiledLayer::createWithParams(
	size_t capacity,TilesheetInfo* sheetInfo,TiledLayerInfo* layerInfo)
{
	TiledLayer* ref = new TiledLayer();
	if(ref->initWithParams(capacity,sheetInfo,layerInfo))
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

void TiledLayer::loadNewChank(int num,LoadDirection direction)
{

}

void TiledLayer::setGridSize(size_t w,size_t h)
{
	_grigWidth = w;
	_gridHeight = h;
}

/**
 * Protected
 */
TiledLayer::TiledLayer()
:_sheetInfo(nullptr)
,_layerInfo(nullptr)
,_zOlder(0)
,_capacity(0)
,_iteratorBegin(0)
,_iteratorEnd(0)
,_chanks(nullptr)
,_indexesOfChank(nullptr)
{}

TiledLayer::~TiledLayer()
{
	saveAllStagedChank();
	delete [] _chanks;
	delete [] _indexesOfChank;
}

bool TiledLayer::initWithParams(
	size_t capacity,TilesheetInfo* sheetInfo,TiledLayerInfo* layerInfo)
{
	if(!SpriteBatchNode::init())
		return false;

	_capacity = capacity;
	_iteratorBegin = 0;
	_iteratorEnd = capacity - 1;
	_layerInfo = layerInfo;
	_sheetInfo = sheetInfo;
	_indexesOfChank = new int[capacity]{-1};
	_chanks = new Chank*[capacity];

	//Load chanks for the first scene
	for(size_t i = 0; i < capacity; ++i)
	{
		_chanks[i] = Chank::createWithParam(sheetInfo->getTileSize(),i);
		loadTerrain(_chanks[i]);
	}

	return true;
}

/**
 * Private
 */
void TiledLayer::saveChank(Chank* chank)
{
}

void TiledLayer::saveAllStagedChank()
{
}

void TiledLayer::loadTerrain(TM25Component::Chank* chank)
{
	std::ifstream i_stream(_layerInfo->getPathTerrainFile().c_str(),std::ios::binary|std::ios::in);
	int buf;

	if(!i_stream)
	{
		std::cout << "ERROR!! >> Cannot open " << _layerInfo->getPathTerrainFile() << '\n';
		return;
	}

	i_stream.seekg(chank->getIndex() * Chank::GRID_WIDTH * Chank::GRID_HEIGHT,std::ios::beg);
	for (size_t i = 0, num = Chank::GRID_WIDTH * Chank::GRID_HEIGHT; i < num; ++i)
	{
		i_stream.read((char*)&buf,sizeof(int));
		chank->insertTypeAt(i,buf);
	}
}

void TiledLayer::saveTerrain(TM25Component::Chank *chank)
{

}

} /* namespace TM25Component */
