#ifndef TILED_MAP_2P5D_FILE_PARSER_H
#define TILED_MAP_2P5D_FILE_PARSER_H

#include "../cocos2d/cocos/cocos2d.h"
#include <string>

class TiledMapInfo;
class TiledLayerBundlerInfo;
class TiledLayerInfo;
class TilesheetInfo;

class TiledMap2P5DFileParser :public cocos2d::Ref
{
public:
	/**
	 * Create TiledMap2P5DDevelopLayer class object.
	 * @method createWithParams
	 * @param  origin                   [The path of the origin file]
	 * @param  tiledMapInfo             [A pointer of TiledMapInfo object]
	 * @param  mapTiledLayerBundlerInfo [Hashed map contains names of TiledLayerBundler
	 *                                   and pointers of TiledLayerBundlerInfo object]
	 * @param  mapTiledLayerInfo        [Hashed map contains names of TIledLayer
	 *                                   and pointers of TiledLayerInfo object]
	 * @param  mapTilesheetInfo         [Hashed map contains names of Tilesheet
	 *                                   and pointers of TilesheetInfo object]
	 * @return                          [A pointer of TiledMap2P5DDevelopLayer class object]
	 */
	static TiledMap2P5DFileParser* createWithParams(
		const std::string& origin,
		TiledMapInfo* tiledMapInfo,
		cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
		cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
		cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo);

protected:
	TiledMap2P5DFileParser();
	~TiledMap2P5DFileParser();

	/**
	 * Initialize object. If initialization fail,return false.
	 * @method initWithParams
	 * @param  origin                   [The path of the origin file]
	 * @param  tiledMapInfo             [A pointer of TiledMapInfo object]
	 * @param  mapTiledLayerBundlerInfo [Hashed map contains names of TiledLayerBundler
	 *                                   and pointers of TiledLayerBundlerInfo object]
	 * @param  mapTiledLayerInfo        [Hashed map contains names of TIledLayer
	 *                                   and pointers of TiledLayerInfo object]
	 * @param  mapTilesheetInfo         [Hashed map contains names of Tilesheet
	 *                                   and pointers of TilesheetInfo object]
	 * @return                          [true/false]
	 */
	bool initWithParams(
		const std::string& origin,
		TiledMapInfo* tiledMapInfo,
		cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
		cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
		cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo);

private:
	/**
	 * Parse origin file and setup TiledMapInfo class object.
	 * @method parseOriginFile
	 * @param  path                   [The path of the origin file]
	 * @param  tiledMapInfo             [A pointer of TiledMapInfo object]
	 * @param  mapTiledLayerBundlerInfo [Hashed map contains names of TiledLayerBundler
	 *                                   and pointers of TiledLayerBundlerInfo object]
	 * @param  mapTiledLayerInfo        [Hashed map contains names of TIledLayer
	 *                                   and pointers of TiledLayerInfo object]
	 * @param  mapTilesheetInfo         [Hashed map contains names of Tilesheet
	 *                                   and pointers of TilesheetInfo object]
	 */
	void parseOriginFile(
		const std::string& path,
		TiledMapInfo* tiledMapInfo,
		cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
		cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
		cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo);

	/**
	 * Parse information file of TiledLayerBundler class and
	 * create a TiledLayerBundlerInfo class object.
	 * @method parseTiledLayerBundlerInfoFile
	 * @param  path                           [The file path]
	 * @return                                [A pointer of TiledLayerBundlerInfo class]
	 */
	TiledLayerBundlerInfo* parseTiledLayerBundlerInfoFile(const std::string& path);

	/**
	 * Parse information file of TiledLayer class and create a TiledLayerInfo class object.
	 * @method parseTiledLayerInfoFile
	 * @param  path                    [The file path]
	 * @return                         [A pointer of TiledLayerInfo class object]
	 */
	TiledLayerInfo* parseTiledLayerInfoFile(const std::string& path);

	/**
	 * Parse information file of Tilesheets and create TilesheetInfo object,
	 * then add them to the hashed map with tilesheet names as keys.
	 * @method parseTilesheetInfoFile
	 * @param  path                   [The file path]
	 * @param  mapTilesheetInfo       [Hashed map contains names of Tilesheet
	 *                                 and pointers of TilesheetInfo object]
	 */
	void parseTilesheetInfoFile(
		const std::string& path,cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo);
};


/**
 * TIledMapInfo class declarations
 */

class TiledMapInfo :public cocos2d::Ref
{
public:
	static TiledMapInfo* create();

protected:
	TiledMapInfo();
	~TiledMapInfo();
	bool init();
};


/**
* TiledLayerBundlerInfo class declarations
*/

class TiledLayerBundlerInfo :public cocos2d::Ref
{
public:
	static TiledLayerBundlerInfo* create();

protected:
	TiledLayerBundlerInfo();
	~TiledLayerBundlerInfo();
	bool init();
};


/**
 * TiledLayerInfo class declarations
 */

class TiledLayerInfo :public cocos2d::Ref
{
public:
	static TiledLayerInfo* create();

protected:
	TiledLayerInfo();
	~TiledLayerInfo();
	bool init();
};


/**
 * TilesheetInfo class declarations
 */

class TilesheetInfo :public cocos2d::Ref
{
public:
	static TilesheetInfo* create();

protected:
	TilesheetInfo();
	~TilesheetInfo();
	bool init();
};

#endif
