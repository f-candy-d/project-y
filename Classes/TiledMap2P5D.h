#ifndef TILED_MAP_2P5D_H
#define TILED_MAP_2P5D_H

#include "../cocos2d/cocos/cocos2d.h"
#include "TiledMap2P5DFileParser.h"

namespace TM25Component {

	class TiledMapInfo;
	class TiledLayerBundlerInfo;
	class TiledLayerInfo;
	class TilesheetInfo;
}

class TiledMap2P5D :public cocos2d::Node
{
public:
	/**
	 * Create a tiled map from origin information file.
	 * @param  path [File path to a origin informatino file]
	 * @return      [A pointer of a tiled map object]
	 */
	static TiledMap2P5D* createWithFile(std::string path);

protected:
	TiledMap2P5D();
	~TiledMap2P5D();

	/**
	 * Initialize a tiled map.Parse information files in this function.
	 * @param  path [File path to a origin informatino file]
	 * @return      [true/false]
	 */
	bool initWithFile(std::string path);

private:
	/**
	 * A hashed map which contains TiledLayerInfo class objects.
	 * A key is a name of a tiled layer.
	 */
	cocos2d::Map<std::string,TM25Component::TiledLayerInfo*> _tiledLayerInfoMap;

	/**
	 * A hashed map which contains TiledLayerBundlerInfo class objects.
	 * A key is a name of a bunched layer.
	 */
	cocos2d::Map<std::string,TM25Component::TiledLayerBundlerInfo*> _TiledLayerBundlerInfoMap;

	/**
	 * A hashed map which contains TilesheetInfo class objects.
	 * A key is a name of a tilesheet.
	 */
	cocos2d::Map<std::string,TM25Component::TilesheetInfo*> _tilesheetInfoMap;

	/**
	 * A pointer of a TiledMapInfo class object.
	 */
	TM25Component::TiledMapInfo* _tiledMapInfo;
};

#endif
