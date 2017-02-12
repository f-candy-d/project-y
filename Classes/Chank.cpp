#include "Chank.h"

USING_NS_CC;

/**
 * Public functions
 */
Chank* Chank::create()
{
	Chank* ref = new Chank();
	if(ref->init())
	{
		ref->autorelease();
		return ref;
	}

	CC_SAFE_DELETE(ref);
	return nullptr;
}

/**
 * Protected functions
 */
Chank::Chank()
:_index(-1)
,_origin(0,0)
{}

Chank::~Chank()
{}

bool Chank::init()
{
	return true;
}

/**
 * Private functions
 */
