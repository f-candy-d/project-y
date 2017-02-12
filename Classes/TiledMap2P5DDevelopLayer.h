 #ifndef TILED_MAP_2P5D_DEVELOP_LAYER_H
#define TILED_MAP_2P5D_DEVELOP_LAYER_H

#include "../cocos2d/cocos/cocos2d.h"
#include "TiledMap2P5D.h"
#include "Chank.h"

class TiledMap2P5DDevelopLayer :public cocos2d::LayerColor
{
protected:
	TiledMap2P5DDevelopLayer();
	~TiledMap2P5DDevelopLayer();
	bool initWithTiledMap(TiledMap2P5D* map);

private:
	CC_SYNTHESIZE_RETAIN(TiledMap2P5D*,_tiledMap,TiledMap);

public:
	static TiledMap2P5DDevelopLayer* createWithTiledMap(TiledMap2P5D* map);
};

#endif
