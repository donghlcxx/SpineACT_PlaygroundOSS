#pragma once
#include <spine/spine.h>

namespace oxspine
{ 
    void init();
    void free();

    spine::TextureLoader *getTextureLoader();

	class SpineActor
	{
	public:
		void setSkeleton(spine::Skeleton*);
		void setAnimationState(spine::AnimationState *st);


	protected:

		spine::Skeleton *_skeleton;
		spine::AnimationState *_animationState;

	};
}