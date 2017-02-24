#ifndef TILED_LAYER_H
#define TILED_LAYER_H

#include "../cocos2d/cocos/cocos2d.h"
#include <mutex>

/**
 * TiledLayer class is a member of the namespace 'TM25Component'.
 */
namespace TM25Component {

//Forward declarations
class Chank;
class TilesheetInfo;
class TiledLayerInfo;

enum class LoadDirection
{
	RIGHT,
	LEFT,
};

class TiledLayer :public cocos2d::Node
{
public:
	/**
	 * Create an object.
	 * @method createWithParams
	 * @param  capacity         [The number of chanks that will be loaded in the vector]
	 * @param  sheetInfo        [Tilesheet information]
	 * @param  layerInfo        [TiledLayer information]
	 * @param  mapInfo        [TiledMap information]
	 * @return                  [A pointer of an object]
	 */
	static TiledLayer* createWithParams(
		size_t capacity,TilesheetInfo* sheetInfo,TiledLayerInfo* layerInfo,TiledMapInfo* mapInfo);

	/**
	 * Load new chank from the terrain file. Before that,save chanks that
	 *  is no longer needed to that file.
	 * @method loadNewChank
	 * @param  num          [The number of chanks that will be loaded]
	 * @param  direction    [If this is RIGHT,load the chank on the right side of
	 *                       currently staged chanks, otherwise load it on the
	 *                       opposit side.]
	 */
	void loadNewChank(size_t num,LoadDirection direction);

protected:
	TiledLayer();
	~TiledLayer();

	/**
	 * Initialize an object.
	 * @method initWithParams
	 * @param  capacity         [The number of chanks that will be loaded in the vector]
	 * @param  sheetInfo        [Tilesheet information]
	 * @param  layerInfo        [TiledLayer information]
	 * @param  mapInfo          [TiledMap information]
	 * @return                  [true/false]
	 */
	bool initWithParams(
		size_t capacity,TilesheetInfo* sheetInfo,TiledLayerInfo* layerInfo,TiledMapInfo* mapInfo);

private:
	/**
	 * Information of the layer and tilesheet.
	 */
	CC_SYNTHESIZE(TilesheetInfo*,_sheetInfo,SheetInfo);
	CC_SYNTHESIZE(TiledLayerInfo*,_layerInfo,LayerInfo);

	/**
	 * A Batch Node. Draw tile sprites in this node.
	 */
	CC_SYNTHESIZE_RETAIN(cocos2d::SpriteBatchNode*,_batchNode,BatchNode);

	//Z older of this layer in the parent TiledLayerBundler.
	CC_SYNTHESIZE(int,_zOlder,ZOlder);

	/**
	 * The number of chanks in a layer.
	 * Cannot load chanks well without this value.
	 * So this value must be setup in intialization.
	 */
	size_t _numOfChanks;

	/**
	 * These iterator indicate the indexes of chanks at the left end and right end.
	 */
	int _iteratorBegin;
	int _iteratorEnd;

	/**
	 * The number of chanks that will be loaded.
	 */
	size_t _capacity;

	/**
	 * The array contains staged chanks.
	 */
	Chank** _chanks;

	/**
	 * The array contains indexes of staged chanks.
	 */
	int* _indexesOfChank;

	std::mutex _mtx;

	/**
	 * Save all staged chanks to the terrain file.
	 * @method saveAllStagedChank
	 */
	void saveAllStagedChank();

	/**
	 * Load terrain data from a binary data file in the chank.
	 * @method loadTerrain
	 * @param  chank       [A chank]
	 */
	void loadTerrain(Chank* chank);

	/**
	 * Save terrain data of a chank to a binary data file.
	 * @method saveTerrain
	 * @param  chank       [A chank]
	 */
	void saveTerrain(Chank* chank);
};

} /* namespace TM25Component */

#endif
