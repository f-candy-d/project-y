#ifndef TILED_MAP_2P5D_H
#define TILED_MAP_2P5D_H

#include "../cocos2d/cocos/cocos2d.h"
#include "TiledMapInfoFileParser.h"

class TiledMap2P5D :public cocos2d::Node
{
protected:
	TiledMap2P5D();
	~TiledMap2P5D();
	bool init();

private:
	/**
	 * A hashed map which contains TiledLayerInfo class objects.
	 * A key is a name of a tiled layer.
	 */
	cocos2d::Map<std::string,TiledLayerInfo*> _tiledLayerInfoMap;

	/**
	 * A hashed map which contains BunchedLayerInfo class objects.
	 * A key is a name of a bunched layer.
	 */
	cocos2d::Map<std::string,BunchedLayerInfo*> _bunchedLayerInfoMap;

	/**
	 * A hashed map which contains TilesheetInfo class objects.
	 * A key is a name of a tilesheet.
	 */
	cocos2d::Map<std::string,TilesheetInfo*> _tilesheetInfoMap;

	/**
	 * A pointer of a TiledMapInfo class object.
	 */
	CC_SYNTHESIZE_RETAIN(TiledMapInfo*,_tiledMapInfo,TiledMapInfo);

	


public:
	static TiledMap2P5D* create();
};

#endif
