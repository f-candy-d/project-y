#include "Chank.h"
#include "TiledMap2P5DFileParser.h"
#include "TiledLayer.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <thread>

USING_NS_CC;

namespace TM25Component {

/**
 * Public
 */
TiledLayer* TiledLayer::createWithParams(
	size_t capacity,TilesheetInfo* sheetInfo,TiledLayerInfo* layerInfo,TiledMapInfo* mapInfo)
{
	TiledLayer* ref = new TiledLayer();
	if(ref->initWithParams(capacity,sheetInfo,layerInfo,mapInfo))
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
		if((0 >= _chanks[_iteratorBegin]->getIndex() && direction == LoadDirection::LEFT)
		|| (_chanks[_iteratorEnd]->getIndex() >= _numOfChanks - 1 && direction == LoadDirection::RIGHT))
			break;

		if(direction == LoadDirection::RIGHT)
		{
			//Remove tiles in the chank from _batchNode node
			_chanks[_iteratorBegin]->eraseTiles(_batchNode,false);
			saveTerrain(_chanks[_iteratorBegin]);
			_chanks[_iteratorBegin]->recycleChank(_chanks[_iteratorEnd]->getIndex() + 1);
			loadTerrain(_chanks[_iteratorBegin]);
			//Donot store tile sprites for now...
			_chanks[_iteratorBegin]->makeTiles(_batchNode,_sheetInfo,false);
			_iteratorEnd = _iteratorBegin;
			_iteratorBegin = (_iteratorBegin + 1 < _capacity) ? _iteratorBegin + 1 : 0;
		}
		else if(direction == LoadDirection::LEFT)
		{
			//Remove tiles in the chank from _batchNode node
			_chanks[_iteratorEnd]->eraseTiles(_batchNode,false);
			saveTerrain(_chanks[_iteratorEnd]);
			_chanks[_iteratorEnd]->recycleChank(_chanks[_iteratorBegin]->getIndex() - 1);
			loadTerrain(_chanks[_iteratorEnd]);
			//Donot store tile sprites for now...
			_chanks[_iteratorEnd]->makeTiles(_batchNode,_sheetInfo,false);
			_iteratorBegin = _iteratorEnd;
			_iteratorEnd = (_iteratorEnd - 1 >= 0) ? _iteratorEnd - 1 : _capacity - 1;
		}
		log("Loaded new chank :: %s :: b=%d[index=%d] :: e=%d[index=%d]",
			(direction==LoadDirection::RIGHT?"RIGHT":"LEFT"),_iteratorBegin,_chanks[_iteratorBegin]->getIndex(),_iteratorEnd,_chanks[_iteratorEnd]->getIndex());
	}
}


/**
 * Protected
 */
TiledLayer::TiledLayer()
:_sheetInfo(nullptr)
,_layerInfo(nullptr)
,_batchNode(nullptr)
,_zOlder(0)
,_numOfChanks(0)
,_capacity(0)
,_iteratorBegin(0)
,_iteratorEnd(0)
,_chanks(nullptr)
,_indexesOfChank(nullptr)
{}

TiledLayer::~TiledLayer()
{
	saveAllStagedChank();
	for(size_t i = 0; i < _capacity; ++i)
	{
		_chanks[i]->release();
	}
	delete [] _chanks;
	delete [] _indexesOfChank;
}

bool TiledLayer::initWithParams(
	size_t capacity,TilesheetInfo* sheetInfo,TiledLayerInfo* layerInfo,TiledMapInfo* mapInfo)
{
	if(!Node::init())
		return false;

	_numOfChanks = mapInfo->getNumOfChanks();
	_capacity = std::min(_numOfChanks,capacity);
	_iteratorBegin = 0;
	_iteratorEnd = std::max(capacity - 1,static_cast<size_t>(0));
	_layerInfo = layerInfo;
	_sheetInfo = sheetInfo;
	_indexesOfChank = new int[_capacity];
	_chanks = new Chank*[_capacity];
	_batchNode = SpriteBatchNode::create(sheetInfo->getFileName());
	this->addChild(_batchNode);

	std::cout << "IN TILED LAYER" << '\n';
	TM25Component::TiledMap2P5DFileParser::debugLogForTilesheetInfo(_sheetInfo);
	TM25Component::TiledMap2P5DFileParser::debugLogForTiledLayerInfo(_layerInfo);

	//Load chanks for the first scene
	for(size_t i = 0; i < _capacity; ++i)
	{
		_chanks[i] = Chank::createWithParam(sheetInfo->getTileSize(),i);
		_chanks[i]->retain();
		loadTerrain(_chanks[i]);
		//Do not store tile sprites for now...
		if(_chanks[i])
			_chanks[i]->makeTiles(_batchNode,sheetInfo,false);
	}

	_chanks[0]->getIndex();

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
	std::string path = _layerInfo->getPathTerrainFile();
	size_t numType = _sheetInfo->getNumOfTileType();
	auto th = std::thread([chank,path,numType]() {
	std::ifstream i_stream(path.c_str(),std::ios::binary|std::ios::in);
	int buf;

	if(!i_stream)
	{
		std::cout << "IN::ERROR!! >> Cannot open " << path << '\n';
		return;
	}

	std::cout << "Load terrain" << '\n';

		i_stream.seekg(
				chank->getIndex() * Chank::GRID_WIDTH * Chank::GRID_HEIGHT * sizeof(int),
				std::ios::beg);

		for (size_t i = 0, num = Chank::GRID_WIDTH * Chank::GRID_HEIGHT; i < num; ++i)
		{
			i_stream.read((char*)&buf,sizeof(int));
			if(0 < buf && buf < numType)
			chank->insertTypeAt(i,buf);
		}
	});
	th.detach();
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

		std::cout << "Save terrain" << '\n';

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
