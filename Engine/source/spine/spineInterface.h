#pragma once
#include "spine.h"
#include "TextureManagement.h"

namespace oxspine
{ 
    void init();
    void free();

    spine::TextureLoader *getTextureLoader();

	class SpineActor
	{
	public:
		bool AddSpine(CKLBImageAsset* pImgAsset, const char *atlasStr, const char*jsonStr);
		void doRender(CKLBImageAsset* pImgAsset);
		void execute(u32 deltaT);
		void floatCopy(float* SpineDes, float* SpineOr , float* SpineUv,int SpinCount);
		void shortCopy(unsigned short* SpineDes, unsigned short* SpineOr, int SpinCount);
		void changerAnimation(const char *animation, bool isloop);
		SpineActor() {
		}
		~SpineActor() {
			delete skeleton;
			skeleton = 0;
			delete skeletonData;
			skeletonData = 0;
			delete animationState;
			animationState = 0;
			delete animationState;
			animationState = 0;
			delete animationStateData;
			animationStateData = 0;
			delete atlas;
			atlas = 0;

			//uvs.clear();
			//uvs.ensureCapacity();
		}
	protected:

		spine::AnimationStateData* animationStateData = 0;
		spine::AnimationState* animationState = 0;
		spine::SkeletonData *skeletonData = 0;
		spine::Atlas* atlas = 0;
		spine::Skeleton *skeleton = 0;
		int vertextIndex = 0;
		int indexIndex = 0;
		spine::RegionAttachment* regionAttachment;
		spine::MeshAttachment* mesh;
		bool isRegion = true;
		spine::Vector<float> *uvs;
		CKLBImageAsset* pImgAsset;
		float uOffset = 0;
		float vOffset = 0;
		float uMax = 1;
		float vMax = 1;
	};
}