/* 
   Copyright 2013 KLab Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
//#include "spine/spine.h"
#include "CKLBUISimpleItem.h"
#include "CKLBUtility.h"
//#include "spineInterface/spine.h"
//#include "spine/Slot.h"

//using namespace oxspine;
/*
//add spine
spine::AnimationStateData* animationStateData = 0;
spine::AnimationState* animationState = 0;
spine::SkeletonData *skeletonData = 0;
spine::Atlas* atlas = 0;
spine::Skeleton *skeleton = 0;

const char* jsonStr = "D:/Temp/spine-runtimes-3.8/examples/coin/export/coin-pro.json";
const char* atlasStr = "D:/Temp/spine-runtimes-3.8/examples/coin/export/coin.atlas";
*/
// Command Values
enum {
	UI_SIMPLE_ = 0,
};

static IFactory::DEFCMD cmd[] = {
	{ "UI_SIMPLE_", UI_SIMPLE_ },
	{0, 0}
};
static CKLBTaskFactory<CKLBUISimpleItem> factory("UI_SimpleItem", CLS_KLBUISIMPLEITEM, cmd);

// Allowed Property Keys
CKLBLuaPropTask::PROP_V2 CKLBUISimpleItem::ms_propItems[] = {
	UI_BASE_PROP,
	{	"order",	UINTEGER,	(setBoolT)&CKLBUISimpleItem::setOrder,	(getBoolT)&CKLBUISimpleItem::getOrder,	0 },
	{	"asset",	R_STRING,	NULL,									(getBoolT)&CKLBUISimpleItem::getAsset,	0 }
};

enum {
	ARG_PARENT = 1,

	ARG_ORDER,
	ARG_X,
	ARG_Y,

	ARG_ASSET,

	ARG_REQUIRE = ARG_ASSET,
	ARG_NUMS = ARG_ASSET
};

CKLBUISimpleItem::CKLBUISimpleItem() 
: CKLBUITask()
, m_pNode   (NULL)
, m_asset   (NULL) 
{
	setNotAlwaysActive();
	m_newScriptModel = true;
}

CKLBUISimpleItem::~CKLBUISimpleItem() 
{
	KLBDELETEA(m_asset);
}

u32 
CKLBUISimpleItem::getClassID() 
{ 
    return CLS_KLBUISIMPLEITEM; 
}

CKLBUISimpleItem* 
CKLBUISimpleItem::create(CKLBUITask* pParent, CKLBNode* pNode, u32 order, float x, float y, const char* asset) 
{
	CKLBUISimpleItem* pTask = KLBNEW(CKLBUISimpleItem);
    if(!pTask) { return NULL; }
	if(!pTask->init(pParent, pNode, order,x,y,asset)) {
		KLBDELETE(pTask);
		return NULL;
	}
	return pTask;
}

bool
CKLBUISimpleItem::init(CKLBUITask* pParent, CKLBNode* pNode, u32 order, float x, float y, const char* asset) 
{
    if(!setupNode()) { return false; }
	bool bResult = initCore(order,x,y,asset);
	bResult = registUI(pParent, bResult);
	if(pNode) {
		pParent->getNode()->removeNode(getNode());
		pNode->addNode(getNode());
	}

	return bResult;
}

//int vertextIndex = 0;
//int indexIndex = 0;

//CKLBTextureAsset* pAsset;
bool
CKLBUISimpleItem::initCore(u32 order, float x, float y, const char* asset)
{
	if(!setupPropertyList((const char**)ms_propItems,SizeOfArray(ms_propItems))) {
		return false;
	}

	setInitPos(x, y);

	klb_assert((((s32)order) >= 0), "Order Problem");

	m_order = order;

	setStrC(m_asset, asset);

	m_pNode = CKLBUtility::createNodeScript( m_asset, m_order, &m_handle);
	if(!m_pNode) {
		return false;
	}
	getNode()->addNode(m_pNode);

	/*

	atlas = new spine::Atlas(atlasStr, oxspine::getTextureLoader());
	assert(atlas != 0);

	spine::SkeletonJson json(atlas);
	json.setScale(1);

	skeletonData = json.readSkeletonDataFile(jsonStr);

	assert(skeletonData != 0);

	for (int n = 0; n < skeletonData->getAnimations().size(); n++)
		DEBUG_PRINT("animationName= %s",skeletonData->getAnimations()[n]->getName().buffer());
	animationStateData = new spine::AnimationStateData(skeletonData);

	assert(animationStateData != 0);

	animationState = new spine::AnimationState(animationStateData);
	assert(animationState != 0);
	//animationState->get
	animationState->setAnimation(0, "animation", true);

	skeleton = new spine::Skeleton(skeletonData);

	assert(skeleton != 0);
	skeleton->setScaleY(-1);
	skeleton->setToSetupPose();


	float fps = animationState->getData()->getSkeletonData()->getFps();
	float time = skeleton->getTime();
	DEBUG_PRINT("Spine fps=%d ", fps);
	DEBUG_PRINT("Spine time=%d ", time);


	CKLBAssetManager& pAssetManager = CKLBAssetManager::getInstance();

	 pAsset = (CKLBTextureAsset*)pAssetManager.FindEntry("itemimage.png.imag");

	//pAsset->m_pImages[0]->m_pXYCoord = NULL;
	
	doRender(pAsset);
	/*
	pAsset->m_totalVertexCount = vertextIndex + 3;
	pAsset->m_totalIndexCount = indexIndex + 1;
	pAsset->m_pImages[0]->m_uiVertexCount = vertextIndex + 1;
	pAsset->m_pImages[0]->m_uiIndexCount = indexIndex + 1;	//KLBDELETEA(pAsset->m_pImages[0]->m_pXYCoord);
	*/
	return true;
}
/*
spine::RegionAttachment* regionAttachment;
void
CKLBUISimpleItem::doRender(CKLBTextureAsset* pAsset)
{
	/*
	float *vertex = new float[32]{ 59,-91,42,-91,42,191,59,191,52.56999,-91,-91.42999,-91,-91.42999,191,52.57,191,202.5,-101,-102.5,-101,-102.5,201,202.5,201,191,-91,-91,-91,-91,191,191,191
	};
	float *uv = new float[32]{ 0.2773438,0.9980469,0.2773438,0.9814453,0.001953125,0.9814453,0.001953125,0.9980469,0.4423828,0.3876953,0.3017578,0.3876953,0.3017578,0.6630859,0.4423828,0.6630859,0.2998047,0.1103516,0.001953125,0.1103516,0.001953125,0.4052734,0.2998047,0.4052734,0.5771484,0.1103516,0.3017578,0.1103516,0.3017578,0.3857422,0.5771484,0.3857422
	};
	unsigned short *index = new unsigned short[24]{ 0,1,2,2,3,0,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,14,15,12 };

	pAsset->m_pImages[0]->m_pXYCoord = vertex;
	pAsset->m_pImages[0]->m_pIndex = index;
	pAsset->m_pImages[0]->m_pUVCoord = uv;

	pAsset->m_pImages[0]->m_uiVertexCount =32;
	pAsset->m_pImages[0]->m_uiIndexCount =24;
	
	//pAsset->m_indexBufferTotal = index;
	//pAsset->m_floatBufferTotal = uv;
	//pAsset->m_totalVertexCount = 16;
	//pAsset->m_totalIndexCount = 24;

	int baseIndex = 0;
	for (size_t i = 0, n = skeleton->getSlots().size(); i < n; ++i)
	{
		spine::Slot* slot = skeleton->getDrawOrder()[i];

		// Fetch the currently active attachment, continue
		// with the next slot in the draw order if no
		// attachment is active on the slot
		spine::Attachment* attachment = slot->getAttachment();
		if (!attachment)
		{
			continue;
		}
		if (slot->getColor().a == 0)
			continue;

		// Fill the vertices array, indices, and texture depending on the type of attachment
		if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
		{
			// Cast to an spRegionAttachment so we can get the rendererObject
			// and compute the world vertices
			regionAttachment = (spine::RegionAttachment*)attachment;

			float* positions = KLBNEWA(float, 8);
			//(pAsset->m_pImages[0]->m_pUVCoor
			regionAttachment->computeWorldVertices(slot->getBone(), (float*)positions, 0, 2);
			//regionAttachment->getRendererObject()
			// copy color and UVs to the vertices
			//memmove(pAsset->m_pImages[0]->m_pXYCoord, positions, sizeof(float) * 8);
				
			floatCopy(pAsset->m_pImages[0]->m_pXYCoord, positions, pAsset->m_pImages[0]->m_pUVCoord);
			//KLBDELETE(positions);
			regionAttachment->getUVs();
			unsigned short* indices = KLBNEWA(unsigned short, 6);
			
			indices[0] = 0 + baseIndex;
			indices[1] = 1 + baseIndex;
			indices[2] = 2 + baseIndex;
			indices[3] = 2 + baseIndex;
			indices[4] = 3 + baseIndex;
			indices[5] = 0 + baseIndex ;

			//memmove(pAsset->m_pImages[0]->m_pIndex, &indices, sizeof(unsigned short) * 6);
			shortCopy(pAsset->m_pImages[0]->m_pIndex, indices);
			//KLBDELETE(indices);
			baseIndex += 4;	
		}
		else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
		{
			spine::MeshAttachment* mesh = (spine::MeshAttachment*)attachment;


			size_t numVertices = mesh->getWorldVerticesLength();
			//vertices.resize(numVertices);

			float* positions = KLBNEWA(float, numVertices);

			mesh->computeWorldVertices(*slot, 0, mesh->getWorldVerticesLength(), positions, 0, 2);

			//memmove(pAsset->m_pImages[0]->m_pXYCoord, positions, sizeof(float) * 8);
			floatCopy(pAsset->m_pImages[0]->m_pXYCoord, positions, pAsset->m_pImages[0]->m_pUVCoord);
			//KLBDELETE(positions);

			unsigned short* sind = mesh->getTriangles().buffer();
			int numInd = mesh->getTriangles().size();
			unsigned short* indices = KLBNEWA(unsigned short, numInd);

			for (int i = 0; i < numInd; ++i)
			{
				indices[i]=(sind[i] + baseIndex);
			}
			//memmove(pAsset->m_pImages[0]->m_pIndex, &indices, sizeof(unsigned short) * 6);
			shortCopy(pAsset->m_pImages[0]->m_pIndex, indices);
			//KLBDELETE(indices);
			baseIndex += numVertices;

		}
	}
	pAsset->m_pImages[0]->m_uiVertexCount = vertextIndex;
	pAsset->m_pImages[0]->m_uiIndexCount = indexIndex;
}

void 
CKLBUISimpleItem::floatCopy(float* des, float* or ,float* uv)
{
	int fsize = sizeof(float);
	int count = _msize(or ) / fsize;
	for (size_t i = 0; i < _msize(or ) / fsize; i++)
	{
		des[vertextIndex+i] = or [i];
		
		//if (attachment!=NULL)
		  //uv[vertextIndex + i] = attachment->getUVs()[i];
		if (regionAttachment != NULL)
			uv[vertextIndex + i] = regionAttachment->getUVs()[i];
	
	}
	vertextIndex += count;
}
void 
CKLBUISimpleItem::shortCopy(unsigned short* des, unsigned short* or )
{
	int fsize = sizeof(unsigned short);
	int count = _msize(or ) / fsize;

	for (size_t i = 0; i < _msize(or ) / fsize; i++)
	{
		des[indexIndex+i] = or [i];
	}
	indexIndex += count;
}
*/
bool
CKLBUISimpleItem::initUI(CLuaState& lua)
{
	int argc = lua.numArgs();
    if(argc < ARG_REQUIRE || argc > ARG_NUMS) { return false; }

	float x = lua.getFloat(ARG_X);
	float y = lua.getFloat(ARG_Y);

	return initCore(lua.getInt(ARG_ORDER),x,y,lua.getString(ARG_ASSET));
} 

void
CKLBUISimpleItem::execute(u32 deltaT)
{
	/*
	//DEBUG_PRINT("execute= %d", deltaT);
	animationState->update(deltaT / 1000.0f);
	animationState->apply(*skeleton);
	skeleton->updateWorldTransform();
	//doRender(pAsset);
	*/
}

void
CKLBUISimpleItem::dieUI()
{
	/*
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
	*/
	CKLBUtility::deleteNode(m_pNode, m_handle);
}
