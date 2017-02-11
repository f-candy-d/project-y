#ifndef TILED_MAP_INFO_FILE_PARSER_H
#define TILED_MAP_INFO_FILE_PARSER_H

#include "../cocos2d/cocos/cocos2d.h"
#include <string>

class TiledMapInfo;
class TiledLayerInfo;
class BunchedLayerInfo;
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
	 * @param  file                    [The name of the target file.]
	 */
	void parseTiledLayerInfoFile(TiledLayerInfo* tiledLayerInfo,std::string file);

	/**
	 * Parse a bunched-layer-info file.
	 * @method parseBunchedLayerInfoFile
	 * @param  bunchedLayerInfo          [A pointer of a BunchedLayerInfo object.]
	 * @param  file                      [The name of the targe file.]
	 */
	void parseBunchedLayerInfoFile(BunchedLayerInfo* bunchedLayerInfo,std::string file);

	/**
	 * Parse a tilesheet-info file.
	 * @method parseTilesheetFile
	 * @param  tilesheetInfos      [A map of TilesheetInfo objects.
	 *                              A key is the name of a tilesheet file.]
	 * @param  file                [The name of the target file.]
	 */
	void parseTilesheetFile(
		const cocos2d::Map<std::string,TilesheetInfo*>& tilesheetInfos,std::string file);

public:
	static TiledMapInfoFileParser* create();

	/**
	 * Parse a map-info file and create objects of TiledMapInfo,TiledLayerInfo,
	 * BunchedLayerInfo,TilesheetInfo.
	 * Then, return these objects through the pointers that this function received.
	 * @method parseOriginFile
	 * @param  file              [The name of a target file which will be parsed.]
	 * @param  tiledMapInfo*     [A pointer of a TiledMapInfo object.]
	 * @param  tiledLayerInfos   [A map of TiledLayerInfo objects.
	 *                            A key is the name of a tiled-layer.]
	 * @param  bunchedLayerInfos [A map of BunchedLayerInfo objects.
	 *                            A key is the name of a bunched-layer.]
	 * @param  tilesheetInfos    [A map of TilesheetInfo objects.
	 *                            A key is the name of a tilesheet file.]
	 */
	void parseOriginFile(
		std::string file,
		TiledMapInfo* tiledMapInfo,
		cocos2d::Map<std::string,TiledLayerInfo*>& tiledLayerInfos,
		cocos2d::Map<std::string,BunchedLayerInfo*>& bunchedLayerInfos,
		cocos2d::Map<std::string,TilesheetInfo*>& tilesheetInfos);
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

	/**
	 * The name of a directory which contains information files for the map.
	 */
	CC_SYNTHESIZE(std::string,_directory,Directory);

	/**
	 * Map architecture. Each elements are a name of a bunched layer.
	 */
	std::vector<std::string> _architecture;

public:
	static TiledMapInfo* create();

	/**
	 * Return the vector '_architecture' calling by reference.
	 * @method getArchitecture
	 * @return [An object of std::vector]
	 */
	std::vector<std::string>& getArchitecture();
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

private:
	/**
	 * The name of the tiled layer.
	 */
	CC_SYNTHESIZE(std::string,_name,Name);

public:
	static TiledLayerInfo* create();
};

/**
 * BunchedLayerInfo class
 */

class BunchedLayerInfo :public cocos2d::Ref
{
protected:
	BunchedLayerInfo();
	~BunchedLayerInfo();
	bool init();

private:
	/**
	 * The name of the bunched layer.
	 */
	CC_SYNTHESIZE(std::string,_name,Name);

	/**
	 * The number of the tiled layers which is contained the bunched layer.
	 */
	CC_SYNTHESIZE(size_t,_numChildLayer,NumChildLayer);

public:
	static BunchedLayerInfo* create();
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
	CC_SYNTHESIZE(size_t,_numTileType,NumTileType);

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
