#ifndef TILED_MAP_2P5D_FILE_PARSER_H
#define TILED_MAP_2P5D_FILE_PARSER_H

#include "../cocos2d/cocos/cocos2d.h"
#include <string>

/**
 * TiledMap2P5DFileParser,TiledMapInfo,TiledLayerInfo,TiledLayerBundlerInfo,
 * TilesheetInfo classes are member of the namespace TM25Component.
 */
namespace TM25Component {

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

// private:
public:
	/**
	 * Parse the information file.If @parsing fail, return false.
	 * @method parseWithArgs
	 * @param  path                     [description]
	 * @param  tiledMapInfo             []
	 * @param  mapTiledLayerBundlerInfo [description]
	 * @param  mapTiledLayerInfo        [description]
	 * @param  mapTilesheetInfo         [description]
	 * @return                          [description]
	 */
	static bool parseWithArgs(
		const std::string& path,
		TiledMapInfo* tiledMapInfo,
		cocos2d::Map<std::string,TiledLayerBundlerInfo*>& mapTiledLayerBundlerInfo,
		cocos2d::Map<std::string,TiledLayerInfo*>& mapTiledLayerInfo,
		cocos2d::Map<std::string,TilesheetInfo*>& mapTilesheetInfo);

	/**
	 * Delete all white-spaces and tabs in the text.
	 * @method deleteSpace
	 * @param  text        [Target]
	 */
	static void deleteSpace(std::string& text);

	/**
	 * Tokenize a line of the information file.
	 * @method tokenize
	 * @param  text     [A line will be tokenized]
	 * @param  tokens   [Add tokens in this vector]
	 */
	static void tokenize(std::string text,std::vector<std::string>& tokens);

	/**
	 * Make a string object from the token.
	 * @method tokenToString
	 * @param  token        [Resource token]
	 * @return              [String object]
	 */
	static std::string tokenToString(std::string token);

	/**
	 * Make a size_t value from the token.
	 * @method tokenToSizeT
	 * @param  token        [Resource]
	 * @return              [size_t value]
	 */
	static size_t tokenToSizeT(std::string token);

	/**
	 * Make a bool value from the token.
	 * @method tokenToBool
	 * @param  token       [Resource]
	 * @return             [bool value]
	 */
	static bool tokenToBool(std::string token);

	/**
	 * Make cocos2d::Size object from the token.
	 * @method tokenToCCSize
	 * @param  token         [Resource]
	 * @return               [cocos2d::Size object]
	 */
	static cocos2d::Size tokenToCCSize(std::string token);

	/**
	 * make cocos2d::Rect object from the token.
	 * @method tokenToCCRect
	 * @param  token         [Resource]
	 * @return               [cocos2d::Rect object]
	 */
	static cocos2d::Rect tokenToCCRect(std::string token);

	/**
	 * Make a TiledMapInfo object from tokens.
	 * Don't add or remove any tokens and also do not break the iterator.
	 * @method tokenToTiledMapInfo
	 * @param  tokens              [Resource tokens]
	 * @param  itr                 [Iterator of a token]
	 * @param  info                [TiledMapInfo object]
	 */
	static void tokenToTiledMapInfo(
		const std::vector<std::string>& tokens,
		std::vector<std::string>::iterator& itr,
		TiledMapInfo* info);

	/**
	 * Make a TileLayerInfo object from tokens.
	 * Don't add or remove any tokens and also do not break the iterator.
	 * @method tokenToTiledLayerInfo
	 * @param  tokens              [Resource tokens]
	 * @param  itr                 [Iterator of a token]
	 * @return                     [TiledLayerInfo object]
	 */
	static TiledLayerInfo* tokenToTiledLayerInfo(
		const std::vector<std::string>& tokens,
		std::vector<std::string>::iterator& itr);

	/**
	 * Make a TileLayerBundlerInfo object from tokens.
	 * Don't add or remove any tokens and also do not break the iterator.
	 * @method tokenToTiledLayerBundlerInfo
	 * @param  tokens              [Resource tokens]
	 * @param  itr                 [Iterator of a token]
	 * @return                     [TiledLayerBundlerInfo object]
	 */
	static TiledLayerBundlerInfo* tokenToTiledLayerBundlerInfo(
		const std::vector<std::string>& tokens,
		std::vector<std::string>::iterator& itr);

	/**
	 * Make a TilesheetInfo object from tokens.
	 * Don't add or remove any tokens and also do not break the iterator.
	 * @method tokenToTilesheetInfo
	 * @param  tokens              [Resource tokens]
	 * @param  itr                 [Iterator of a token]
	 * @return                     [TilesheetInfo object]
	 */
	static TilesheetInfo* tokenToTilesheetInfo(
		const std::vector<std::string>& tokens,
		std::vector<std::string>::iterator& itr);

	/**
	* For debug
	*/
	static void debugLogForTiledMapInfo(TiledMapInfo* info);
	static void debugLogForTiledLayerInfo(TiledLayerInfo* info);
	static void debugLogForTiledLayerBundlerInfo(TiledLayerBundlerInfo* info);
	static void debugLogForTilesheetInfo(TilesheetInfo* info);

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
	static bool matchingRegex(
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
	static bool matchingRegex(const std::string &text,const std::string &regex);
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
	static TilesheetInfo* create();

	/**
	 * Add a texture rect to _textureRects.
	 * Cannot get _textureRects directory,so use this function
	 * when you want to add a new texture rect.
	 * @method addTextureRect
	 * @param  rect           [Texture rect]
	 */
	void addTextureRect(cocos2d::Rect rect);

	/**
	 * Return the vector(_textureRects) by reference.
	 * @method getTextureRectAsVector
	 * @return [_textureRects that is passed by reference]
	 */
	const std::vector<cocos2d::Rect>& getTextureRectAsVector();

	/**
	 * Find a texture rect with the clue of a tile type,and return it.
	 * @param  type   [tile type]
	 * @return        [texture rect]
	 */
	const cocos2d::Rect getTextureRectForType(size_t type);

	/**
	 * Getter function of _numTileType.
	 * @method getNumOfTileType
	 * @return [_numTileType]
	 */
	size_t getNumOfTileType();

	/**
	 * Setter function of _numTileType.
	 * Set capacity to the vector (_textureRects) using it.
	 * @method setNumOfTileType
	 * @param  num              [The number of the tile types]
	 */
	void setNumOfTileType(size_t num);

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
	 * The size of a tile.
	 */
	CC_SYNTHESIZE(cocos2d::Size,_tileSize,TileSize);

	/**
	* The number of the tile types.
	*/
	size_t _numTileType;

	/**
	 * An array contains Rect objects of a tile texture on the tilesheet.
	 * An index is tile type.
	 */
	std::vector<cocos2d::Rect> _textureRects;

protected:
	TilesheetInfo();
	~TilesheetInfo();
	bool init();
};

} /* namespace TM25Compo */

#endif
