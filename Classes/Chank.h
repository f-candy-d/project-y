#ifndef CHANK_H
#define CHANK_H

#include "../cocos2d/cocos/cocos2d.h"

/**
 * Chank class is a member of the namespace TM25Component.
 */
namespace TM25Component {

//Forward declaration
class TilesheetInfo;

class Chank :public cocos2d::Ref
{
public:
	/**
	* The width and height of the grid of the chank.
	* (= The number of tiles of horizontal and vertical direction.)
	* These are constant value.
	*/
	static const size_t GRID_WIDTH = 16;
	static const size_t GRID_HEIGHT = 256;

	/**
	 * Create Chank class object.
	 * @method createWithParam
	 * @param  size            [The size of a tile(px)]
	 * @param  index           [The index of the chank on a layer]
	 * @return                 [A pointer of a Chank class object]
	 */
	static Chank* createWithParam(cocos2d::Size size,size_t index);

	/**
	 * Make sprites of the tile and add them to the parent SpriteBatchNode.
	 * @method makeTiles
	 * @param  parent        [Parent SpriteBatchNode]
	 * @param  tilesheetInfo [A pointer of TilesheetInfo class object]
	 * @param  storeSprites  [If this is true,store sprite objects in the vector]
	 */
	void makeTiles(cocos2d::SpriteBatchNode* parent,TilesheetInfo* tilesheetInfo,bool storeSprites);

	/**
	 * Remove all sprites of the tile from parent SpriteBatchNode.
	 * @method eraseTiles
	 * @param  parent     [Parent SpriteBatchNode]
	 */
	void eraseTiles(cocos2d::SpriteBatchNode* parent);

protected:
	Chank();
	~Chank();

	/**
	 * Initialize the object. If initialization fail,return false.
	 * @method initWithParam
	 * @param  size             [The size of a tile(px)]
	 * @param  index            [The index of the chank on a layer]
	 * @return                  [true/false]
	 */
	bool initWithParam(cocos2d::Size size,size_t index);

private:
	/**
	 * The index of the chank.
	 */
	CC_SYNTHESIZE(size_t,_index,Index);

	/**
	 * The size of the chank.
	 */
	CC_SYNTHESIZE(cocos2d::Size,_size,Size);

	/**
	 * The vector which locate the origin point on a map.
	 */
	CC_SYNTHESIZE(cocos2d::Vec2,_origin,Origin);

	/**
	 * The array contains tile types.
	 * Use this array as 2D array.
	 */
	int _tiles[GRID_WIDTH * GRID_HEIGHT] = {-1};

	/**
	 * The array contains sprites of the tile.
	 */
	cocos2d::Vector<cocos2d::Sprite*> _sprites;
};

} /* namespace TM25Component */

#endif
