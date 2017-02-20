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

void TiledLayer::loadNewChank(size_t num,LoadDirection direction)
{
	for(size_t i = 0; i < num; ++i)
	{
		//Cannot load any more!
		if(0 >= _chanks[_iteratorBegin]->getIndex()
		|| _chanks[_iteratorEnd]->getIndex() >= _grigWidth - 1)
			break;

		if(direction == LoadDirection::RIGHT)
		{
			//Remove tiles in the chank from this node
			_chanks[_iteratorBegin]->eraseTiles(this,false);
			saveTerrain(_chanks[_iteratorBegin]);
			_chanks[_iteratorBegin]->recycleChank(_chanks[_iteratorEnd]->getIndex() + 1);
			loadTerrain(_chanks[_iteratorBegin]);
			//Donot store tile sprites for now...
			_chanks[_iteratorBegin]->makeTiles(this,_sheetInfo,false);
			_iteratorEnd = _iteratorBegin;
			_iteratorBegin = (_iteratorBegin + 1 < _capacity) ? _iteratorBegin + 1 : 0;
		}
		else if(direction == LoadDirection::LEFT)
		{
			//Remove tiles in the chank from this node
			_chanks[_iteratorEnd]->eraseTiles(this,false);
			saveTerrain(_chanks[_iteratorEnd]);
			_chanks[_iteratorEnd]->recycleChank(_chanks[_iteratorBegin]->getIndex() - 1);
			loadTerrain(_chanks[_iteratorEnd]);
			//Donot store tile sprites for now...
			_chanks[_iteratorEnd]->makeTiles(this,_sheetInfo,false);
			_iteratorBegin = _iteratorEnd;
			_iteratorEnd = (_iteratorEnd - 1 >= 0) ? _iteratorEnd - 1 : _capacity - 1;
		}
	}
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
		//Do not store tile sprites for now...
		_chanks[i]->makeTiles(this,sheetInfo,false);
	}

	return true;
}

/**
 * Private
 */

void TiledLayer::saveAllStagedChank()
{
	for(size_t i = _capacity; i < _capacity; ++i)
		saveTerrain(_chanks[i]);
}

void TiledLayer::loadTerrain(TM25Component::Chank* chank)
{
	std::ifstream i_stream(_layerInfo->getPathTerrainFile().c_str(),std::ios::binary|std::ios::in);
	int buf;

	if(!i_stream)
	{
		std::cout << "IN::ERROR!! >> Cannot open " << _layerInfo->getPathTerrainFile() << '\n';
		return;
	}

	i_stream.seekg(
		chank->getIndex() * Chank::GRID_WIDTH * Chank::GRID_HEIGHT * sizeof(int),
		std::ios::beg);

	for (size_t i = 0, num = Chank::GRID_WIDTH * Chank::GRID_HEIGHT; i < num; ++i)
	{
		i_stream.read((char*)&buf,sizeof(int));
		if(0 < buf && buf < _sheetInfo->getNumOfTileType())
			chank->insertTypeAt(i,buf);
	}
}

void TiledLayer::saveTerrain(TM25Component::Chank *chank)
{
	if(chank->getIsModified())
	{
		std::ofstream o_stream(
			_layerInfo->getPathTerrainFile().c_str(),
			std::ios::in|std::ios::out|std::ios::binary|std::ios::app);

		if(!o_stream)
		{
			std::cout << "OUT::ERROR!! >> Cannot open " << _layerInfo->getPathTerrainFile() << '\n';
			return;
		}

		o_stream.seekp(
			chank->getIndex() * Chank::GRID_WIDTH * Chank::GRID_HEIGHT * sizeof(int),
			std::ios::beg);

		for (size_t i = 0, buf = -1, num = Chank::GRID_WIDTH * Chank::GRID_HEIGHT; i < num; ++i)
		{
			buf = chank->getTypeAt(i);
			o_stream.write((char*)&buf,sizeof(int));
		}
	}
}

} /* namespace TM25Component */
