#ifndef CHANK_H
#define CHANK_H

#include "../cocos2d/cocos/cocos2d.h"

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
	 * @method createWithTileSize
	 * @param  size               [The size of a tile(px)]
	 * @return                    [A pointer of Chank class object]
	 */
	static Chank* createWithTileSize(cocos2d::Size size);

	/**
	 * Make sprites of the tile and add them to the parent SpriteBatchNode.
	 * @method makeTiles
	 * @param  parent        [Parent SpriteBatchNode]
	 * @param  tilesheetInfo [A pointer of TilesheetInfo class object]
	 */
	void makeTiles(cocos2d::SpriteBatchNode* parent,TilesheetInfo* tilesheetInfo);

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
	 * @method initWithTileSize
	 * @param  size             [The size of a tile(px)]
	 * @return                  [true/false]
	 */
	bool initWithTileSize(cocos2d::Size size);

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

#endif
