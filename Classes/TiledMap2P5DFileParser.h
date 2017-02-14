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
	 * If parseing the file fail,return false.
	 * @method parseOriginFile
	 * @param  path                     [The path of the origin file]
	 * @param  tiledMapInfo             [A pointer of TiledMapInfo object]
	 * @param  mapTiledLayerBundlerInfo [Hashed map contains names of TiledLayerBundler
	 *                                   and pointers of TiledLayerBundlerInfo object]
	 * @param  mapTiledLayerInfo        [Hashed map contains names of TIledLayer
	 *                                   and pointers of TiledLayerInfo object]
	 * @param  mapTilesheetInfo         [Hashed map contains names of Tilesheet
	 *                                   and pointers of TilesheetInfo object]
	 * @return                          [true/false]
	 */
	bool parseOriginFile(
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
	 * @return                        [true/false]
	 */
	bool parseTilesheetInfoFile(
		const std::string& path,cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo);

	/**
	* Do matching text using the regex. This function will only be called
	* in matchingRegex(text,regex). If you want to do matching text,
	* call that function instead of this.If the text matches up with the regex,
	* return true, otherwise return false.
	*
	* Use an asterisk '*' to indicate what there are zero or more charactors on it.
	* Use an questionmark '?' to indicate what there is one charactor on it.
	* Use an dollar '&$' to indicate what there are one or more numbers on it.
	* A whitespace is treated as one charactor.
	*
	* @method matchingRegex
	* @param  text          [The target text]
	* @param  regex         [The regex used in matching]
	* @param  i_txt         [The iterator of the target text]
	* @param  i_rgx         [The iterator of the ragex]
	* @return               [true/false]
	*/
	bool matchingRegex(
		const std::string &text,
		const std::string &regex,
		std::string::const_iterator i_txt,
		std::string::const_iterator i_rgx);

	/**
	 * Do matching using the regex. Generally,use this function instead of
	 * matchingRegex(text,regex,i_txt,i_regex) function.If the text matches up with the regex,
	 * return true, otherwise return false.
	 *
	 * Use an asterisk '*' to indicate what there are zero or more charactors on it.
	 * Use an questionmark '?' to indicate what there is one charactor on it.
	 * Use an dollar '$' to indicate what there are one or more numbers on it.
	 * A whitespace is treated as one charactor.
	 *
	 * @method matchingRegex
	 * @param  text          [The target text]
	 * @param  regex         [Regex used in matching]
	 * @return               [ture/false]
	 */
	bool matchingRegex(const std::string &text,const std::string &regex);

	/**
	 * Delete whitespaces or tabs in front of the head of the string.
	 * @method deleteHeadSpace
	 * @param  buff            [Target string]
	 */
	void deleteHeadSpace(std::string& buff);

	/**
	 * Find a substring in the target string.
	 * If find it,return true,otherwise return false.
	 * @method findStr
	 * @param  string    [The target string]
	 * @param  substring [The substring searching for]
	 * @return           [true/false]
	 */
	bool findStr(const std::string &string,const std::string &substring);

	/**
	 * Find a tag in the line of the information file.,and return it.
	 * @method findTagInLine
	 * @param  line          [A line of the information file]
	 * @return               [A tag]
	 */
	std::string findTagInLine(const std::string &line);
};


/**
 * TIledMapInfo class declarations
 */

class TiledMapInfo :public cocos2d::Ref
{
public:
	static TiledMapInfo* create();

	/**
	 * Return the vector '_architecture' calling by reference.
	 * @method getArchitecture
	 * @return [An object of std::vector]
	 */
	const std::vector<std::string>& getArchitecture();

	/**
	 * Add a name of TiledLayerBundler to the map architecture.
	 * @method addElementToArchitecture
	 * @param  element                  [The name of TiledLayerBundler]
	 */
	void addElementToArchitecture(std::string element);

private:
	/**
	 * Map architecture. Each elements are a name of a bunched layer bundler.
	 */
	std::vector<std::string> _architecture;

	/**
	 * The grid size of the map.
	 */
	CC_SYNTHESIZE(size_t,_mapGridWidth,MapGridWidth);
	CC_SYNTHESIZE(size_t,_mapGridHeight,MapGridHeight);

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

	/**
	 * Return the vector '_architecture' calling by reference.
	 * @method getArchitecture
	 * @return [An object of std::vector.]
	 */
	const std::vector<std::string>& getArchitecture();

	/**
	 * Add a name of TiledLayer to the map architecture.
	 * @method addElementToArchitecture
	 * @param  element                  [The name of TiledLayer]
	 */
	void addElementToArchitecture(std::string element);

private:
	/**
	 * The name of the bunched layer bundler.
	 */
	CC_SYNTHESIZE(std::string,_name,Name);

	/**
	 * This vector contains the names of TiledLayers contained by the BunchedLayerBundler.
	 */
	std::vector<std::string> _architecture;

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

private:
	/**
	 * The name of the tiled layer.
	 */
	CC_SYNTHESIZE(std::string,_layerName,LayerName);

	/**
	 * The file path to the terrain data file.
	 */
	CC_SYNTHESIZE(std::string,_pathTerrainFile,PathTerrainFile);

	/**
	 * The name of a tilesheet that is used in this Tiled-Layer.
	 */
	CC_SYNTHESIZE(std::string,_tilesheetName,TilesheetName);

	/**
	 * The flag of what the Tiled-Layer is visible or not.
	 */
	CC_SYNTHESIZE(bool,_isVisible,IsVisible);

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
	/**
	 * Create TilesheetInfo class object.
	 * @method createWithTileTypeNum
	 * @param  num                   [The number of kinds of tiles]
	 * @return                       [A pointer of the TilesheetInfo class object]
	 */
	static TilesheetInfo* createWithTileTypeNum(const size_t num);

private:
	/**
	 * The name of a tilesheet resource file.
	 */
	CC_SYNTHESIZE(std::string,_fileName,FileName);

	/**
	 * The name of a tilesheet.
	 */
	CC_SYNTHESIZE(std::string,_sheetName,SheetName);

	/**
	 * The number of the tile types.
	 */
	CC_SYNTHESIZE(size_t,_numTileType,NumTileType);

	/**
	 * The size of a tile.
	 */
	CC_SYNTHESIZE(cocos2d::Size,_tileSize,TileSize);

	/**
	 * An array contains Rect objects of a tile texture on the tilesheet.
	 * An index is tile type.
	 */
	cocos2d::Rect* _textureRects;

protected:
	TilesheetInfo();
	~TilesheetInfo();

	/**
	 * Initialize class object and the array :: _textureRects using the parameter num.
	 * @method initWithTileTypeNum
	 * @param  num                 [The number of kinds of tiles]
	 * @return                     [true/false]
	 */
	bool initWithTileTypeNum(const size_t num);
};

#endif
