#ifndef CHANK_H
#define CHANK_H

#include "../cocos2d/cocos/cocos2d.h"

class Chank :public cocos2d::Ref
{
public:
	/**
	* The width and height of the chank.
	* (= The number of tiles of horizontal and vertical direction.)
	* These are constant value.
	*/
	static const size_t CHANK_WIDH = 16;
	static const size_t CHANK_HEIGHT = 256;

	static Chank* create();

protected:
	Chank();
	~Chank();
	bool init();

private:
	/**
	 * The index of the chank.
	 */
	CC_SYNTHESIZE(size_t,_index,Index);

	/**
	 * The vector which locate the origin point on a map.
	 */
	CC_SYNTHESIZE(cocos2d::Vec2,_origin,Origin);

	/**
	 * The array contains tile types.
	 * Use this array as 2D array.
	 */
	int _tiles[CHANK_WIDH * CHANK_HEIGHT] = {-1};
};

#endif
