#ifndef TILED_MAP_INFO_FILE_PARSER_H
#define TILED_MAP_INFO_FILE_PARSER_H

#include "../cocos2d/cocos/cocos2d.h"
#include <string>

class TiledMapInfo;
class TiledLayerInfo;
class TilesheetInfo;

/**
 * TiledMapInfoFileParser class
 */

class TiledMapInfoFileParser :public cocos2d::Ref
{
protected:
	TiledMapInfoFileParser();
	~TiledMapInfoFileParser();
	bool init();

private:
	/**
	 * Parse a tiled-layer-info file.
	 * @method parseTiledLayerInfoFile
	 * @param  tiledLayerInfo          [A pointer of a TiledLayerInfo object.]
	 */
	void parseTiledLayerInfoFile(TiledLayerInfo* tiledLayerInfo);

	/**
	 * Parse a tilesheet-info file.
	 * @method parseTilesheetFile
	 * @param  tilesheetInfos      [A vector of TilesheetInfo objects.]
	 */
	void parseTilesheetFile(const cocos2d::Vector<TilesheetInfo*>& tilesheetInfos);

public:
	static TiledMapInfoFileParser* create();

	/**
	 * Parse a map-info file and create objects of TiledMapInfo,TiledLayerInfo,TilesheetInfo.
	 * Then, return these objects through the pointers that this function received.
	 * @method parseOriginFile
	 * @param  file           [The name of a target file which will be parsed.]
	 * @param  tiledMapInfo*   [A pointer of a TiledMapInfo object.]
	 * @param  tiledLayerInfos [A vector of TiledLayerInfo objects.]
	 * @param  tilesheetInfos  [A vector of TilesheetInfo objects.]
	 */
	void parseOriginFile(
		std::string file,
		TiledMapInfo* tiledMapInfo,
		const cocos2d::Vector<TiledLayerInfo*>& tiledLayerInfos,
		const cocos2d::Vector<TilesheetInfo*>& tilesheetInfos);
};

/**
 * TiledMapInfo class
 */

class TiledMapInfo :public cocos2d::Ref
{
protected:
	TiledMapInfo();
	~TiledMapInfo();
	bool init();

private:
	/**
	 * The number of chanks in a tiled layer.
	 */
	CC_SYNTHESIZE(size_t,_numChank,NumChank);

	/**
	 * The number of all tile layers.
	 */
	CC_SYNTHESIZE(size_t,_numTiledLayer,NumTiledLayer);

	/**
	 * The number of bunched layers.
	 */
	CC_SYNTHESIZE(size_t,_numBunchedLayer,NumBunchedLayer);

public:
	static TiledMapInfo* create();
};

/**
 * TiledLayerInfo class
 */

class TiledLayerInfo :public cocos2d::Ref
{
protected:
	TiledLayerInfo();
	~TiledLayerInfo();
	bool init();

public:
	static TiledLayerInfo* create();
};

/**
 * TilesheetInfo class
 */

class TilesheetInfo :public cocos2d::Ref
{
protected:
	TilesheetInfo();
	~TilesheetInfo();
	/**
	 * Initialize TilesheetInfo object with the number of tile types.
	 * @method initWithN
	 * @param  n           [The number of the tile types.]
	 * @return             [true / false.]
	 */
	bool initWithN(size_t n);

private:
	/**
	 * The name of a tilesheet resource file.
	 */
	CC_SYNTHESIZE(std::string,_name,Name);

	/**
	 * The number of the tile types.
	 */
	size_t _numTileType;

	/**
	 * An array contains Rect objects of a tile texture on the tilesheet.
	 * An index is tile type.
	 */
	cocos2d::Rect* _textureRects;

public:
	/**
	 * Create TilesheetInfo object with the number of tile types.
	 * @method createWithN
	 * @param  n           [The number of the tile types.]
	 * @return             [A pointer of TilesheetInfo object or nullptr.]
	 */
	static TilesheetInfo* createWithN(size_t n);
};

#endif
