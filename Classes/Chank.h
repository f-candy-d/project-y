#ifndef CHANK_H
#define CHANK_H

#include "../cocos2d/cocos/cocos2d.h"

typedef unsigned int u_int;
typedef unsigned long int u_l_int;

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
	static Chank* createWithParam(cocos2d::Size size,int index);

	/**
	 * Make sprites of the tile and add them to the parent SpriteBatchNode.
	 * @method makeTiles
	 * @param  parent        [Parent SpriteBatchNode]
	 * @param  tilesheetInfo [A pointer of TilesheetInfo class object]
	 * @param  storeSprites  [Store sprites created to the vector or not]
	 */
	void makeTiles(
		cocos2d::SpriteBatchNode* parent,TilesheetInfo* tilesheetInfo,bool storeSprites);

	/**
	 * Remove all sprites of the tile from parent SpriteBatchNode.
	 * @method eraseTiles
	 * @param  parent     [Parent SpriteBatchNode]
	 * @param  del_vec     [True if you want to remove all sprites from the vector too,false otherwise]
	 */
	void eraseTiles(cocos2d::SpriteBatchNode* parent,bool del_vec);

	/*
	 * Use this function when add tile types to the arrary.
	 * @param [Insert a tile type at this index of the array]
	 * @param [Tile type]
	 */
	void insertTypeAt(size_t index,int tpye);

	/**
	 * Return a tile type at a certain position.
	 * @param  index [A position of a certain tile]
	 * @return       [Tile type at the index]
	 */
	int getTypeAt(size_t index);

	/**
	 * Re-initialize this chank and reuse it.
	 * When load a new chank,use this function
	 *  instead of that create new Chank object.
	 * @param index [New index]
	 */
	void recycleChank(int index);

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
	bool initWithParam(cocos2d::Size size,int index);

private:
	/**
	 * The index of the chank.
	 */
	CC_SYNTHESIZE(int,_index,Index);

	/**
	 * The size of the chank (px).
	 */
	CC_SYNTHESIZE(cocos2d::Size,_size,Size);

	/**
	 * The vector which locate the origin point on a map.
	 */
	CC_SYNTHESIZE(cocos2d::Vec2,_origin,Origin);

	/**
	 * The flag indicate that terrain data of this chank is modified or not.
	 */
	CC_SYNTHESIZE(bool,_isModified,IsModified);

	/**
	 * The flag indicate that sprites of tiles are stored in the vector or not.
	 */
	bool _haveSprites;

	/**
	 * The array contains tile types.
	 * Use this array as 2D array.
	 */
	int _tiles[GRID_WIDTH * GRID_HEIGHT] = {};

	/**
	 * The array contains sprites of the tile.
	 */
	cocos2d::Vector<cocos2d::Sprite*> _sprites;

	/**
	 * Count the number of digits of n using binary search.
	 * @method countDigit
	 * @param  n         [A nutural number or zero]
	 * @return           [The number of the digits]
	 */
	int countDigit(u_int n);

	/**
	 * Return the 'e'th power of 10 (unsigned long int).
	 * The range of the a returned value(R) is ::
	 * 1 <= R <= 10,000,000,000,000,000,000
	 * @method pow10
	 * @param  e     [0 <= e <= 20]
	 * @return       [10^e]
	 */
	u_l_int pow10(u_int e);

	/**
	 * Make a hash value of the coordinate.
	 * For example...
	 * (x,y) = (256,60) --- comvert... ---> 256602 (<-hash)
	 * Put 'y' on the right of 'x',and then put the number
	 *  of digits of 'y' on the right of 'y'.
	 * @method makeHashOfCoordinate
	 * @param  x                    [A x coordinate]
	 * @param  y                    [A y coordinate]
	 * @return                      [hash value]
	 */
	u_l_int makeHashOfCoordinate(u_int x,u_int y);

	/**
	 * Decode the hash value created by makeHashOfCoordinate() function
	 *  to a coordinate (x,y) as Vec2.
	 * The format of the hash value is >> [x][y][the number of digits of y]
	 * @method decodeHashOfCoordinate
	 * @param  hash                   [A hash value created by makeHashOfCoordinate()]
	 * @return                        [A decoded coordinate]
	 */
	cocos2d::Vec2 decodeHashOfCoordinate(u_l_int hash);

};

} /* namespace TM25Component */

#endif
