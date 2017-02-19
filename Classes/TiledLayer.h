#ifndef TILED_LAYER_H
#define TILED_LAYER_H

#include "../cocos2d/cocos/cocos2d.h"

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

class TiledLayer :public cocos2d::SpriteBatchNode
{
public:
	/**
	 * Create an object.
	 * @method createWithCapacity
	 * @param  capacity           [The number of chanks that will be loaded]
	 * @return                    [A pointer of the object]
	 */
	static TiledLayer* createWithCapacity(size_t capacity);

	/**
	 * Load new chank from the terrain file. Before that,save chanks that
	 *  is no longer needed to that file.
	 * @method loadNewChank
	 * @param  num          [The number of chanks that will be loaded]
	 * @param  direction    [If this is RIGHT,load the chank on the right side of
	 *                       currently staged chanks, otherwise load it on the
	 *                       opposit side.]
	 */
	void loadNewChank(int num,LoadDirection direction);

protected:
	TiledLayer();
	~TiledLayer();

	/**
	 * Initialize the object. Create arrays using 'capacity' in this function.
	 * @method initWithCapacity
	 * @param  capacity         [The number of chanks that will be loaded]
	 * @return                  [true/false]
	 */
	bool initWithCapacity(size_t capacity);

private:
	/**
	 * Information of the layer and tilesheet.
	 */
	CC_SYNTHESIZE(TilesheetInfo*,_sheetInfo,SheetInfo);
	CC_SYNTHESIZE(TiledLayerInfo*,_layerInfo,LayerInfo);

	//Z older of this layer in the parent TiledLayerBundler.
	CC_SYNTHESIZE(int,_zOlder,ZOlder);

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

	/**
	 * Save chanks to the terrain data file.
	 * @method saveChank
	 * @param  index     [The index of the chank that will be saved]
	 */
	void saveChank(int index);

	/**
	 * Save all staged chanks to the terrain file.
	 * @method saveAllStagedChank
	 */
	void saveAllStagedChank();
};

} /* namespace TM25Component */

#endif
