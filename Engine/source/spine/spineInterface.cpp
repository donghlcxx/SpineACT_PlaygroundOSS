#include "spine.h"
#include "spine.h"
#include "BlendMode.h"
#include "Atlas.h"
#include "RegionAttachment.h"
#include "Debug.h"
#include "CKLBAsset.h"
#include "TextureManagement.h"
#include "spineInterface.h"



namespace spine
{
    static DebugExtension *debugExtension = NULL;

    SpineExtension* getDefaultExtension() {
        // return a default spine extension that uses standard malloc for memory
        // management, and wrap it in a debug extension.

		return new DefaultSpineExtension();
    }
}


namespace oxspine
{
	using namespace spine;
	
	void init()
	{

	}

	void free()
	{
#ifdef OX_DEBUG
		if (debugExtension)
		{
			debugExtension->reportLeaks();
		}
#endif
	}
	class MyTextureLoader : public TextureLoader
	{
	public:
		MyTextureLoader() {}


		// Called when the atlas loads the texture of a page.
		virtual void load(AtlasPage& page, const String& path)
		{
			CKLBAssetManager& pAssetManager = CKLBAssetManager::getInstance();

			CKLBTextureAsset* pAsset =(CKLBTextureAsset*)pAssetManager.FindEntry("spineboy.png.imag");
		
			page.setRendererObject(pAsset->m_pTextureUsage->pTexture);

			page.width = pAsset->m_pTexture->getWidth();
			page.height = pAsset->m_pTexture->getHeight();
			
		}


		// Called when the atlas is disposed and itself disposes its atlas pages.
		virtual void unload(void* texture)
		{
		}
	};

	MyTextureLoader _loader;


	spine::TextureLoader * getTextureLoader()
	{
		return &_loader;
	}
	bool
	SpineActor::AddSpine(CKLBImageAsset* pImgAsset, const char *atlasStr, const char*jsonStr)
	{
		IPlatformRequest& pfif = CPFInterface::getInstance().platform();
		const char * covatlas = pfif.getFullPath(atlasStr);
		const char * covaJson = pfif.getFullPath(jsonStr);

		atlas = new spine::Atlas(covatlas, oxspine::getTextureLoader());
		assert(atlas != 0);

		spine::SkeletonJson json(atlas);
		json.setScale(1);

		skeletonData = json.readSkeletonDataFile(covaJson);

		assert(skeletonData != 0);

		for (int n = 0; n < skeletonData->getAnimations().size(); n++)
			DEBUG_PRINT("animationName= %s", skeletonData->getAnimations()[n]->getName().buffer());
		animationStateData = new spine::AnimationStateData(skeletonData);

		assert(animationStateData != 0);

		animationState = new spine::AnimationState(animationStateData);
		assert(animationState != 0);
		//animationState->get

		skeleton = new spine::Skeleton(skeletonData);
		//animationState->setAnimation(0, "idle", true);
		assert(skeleton != 0);
		//skeleton->setScaleY(-1);
		skeleton->setToSetupPose();

		animationState->update(0);
		animationState->apply(*skeleton);
		skeleton->updateWorldTransform();

		float fps = animationState->getData()->getSkeletonData()->getFps();
		float time = skeleton->getTime();
		DEBUG_PRINT("Spine fps=%d ", fps);
		DEBUG_PRINT("Spine time=%d ", time);

		uOffset = pImgAsset->m_pUVCoord[0];
		vOffset = pImgAsset->m_pUVCoord[1];
		pImgAsset->m_pXYCoord = KLBNEWA(float, (2000 * 3));
		pImgAsset->m_pUVCoord = &pImgAsset->m_pXYCoord[2000];
		pImgAsset->m_pIndex = (u16*)(&pImgAsset->m_pUVCoord[2000]);

	 	SKLBRect *rect= pImgAsset->getSize();
		
		//uMax = rect->getWidth() / 2048.0;
		//vMax = rect->getHeight() / 2048.0;
		this->pImgAsset = pImgAsset;
		return true;
	}
	void
	SpineActor::floatCopy(float* SpineDes, float* SpineOr, float* SpineUv,int SpinCount)
	{
		int fsize = sizeof(float);
		int count = SpinCount;

		if (isRegion)
			uvs = regionAttachment->getUVs();
		else
		{
			uvs = mesh->getUVs();
		}

		for (size_t i = 0; i < count; i++)
		{
			*(SpineDes + vertextIndex + i) = *(SpineOr +i);

			if (i % 2 == 0)
			{
				*(SpineDes + vertextIndex + i) = *(SpineOr +i) + 500;
				*(SpineUv + vertextIndex + i) = uOffset+ uvs[i]*uMax;
			}
			else
			{
				*(SpineDes + vertextIndex + i) = -(*(SpineOr +i)) + 500;
				*(SpineUv + vertextIndex + i) = vOffset + uvs[i] * vMax;
			}

			

		}
		vertextIndex += count;
		uvs.clear();
		//uvs =nullptr;
	}
	void
	SpineActor::shortCopy(unsigned short* SpineDes, unsigned short* SpineOr, int SpinCount)
	{
		int fsize = sizeof(unsigned short);
		int count = SpinCount;

		for (size_t i = 0; i < count; i++)
		{
			*(SpineDes + indexIndex + i) = *(SpineOr +i);
		}
		indexIndex += count;
	}
	int count = 0;
	void
	SpineActor::execute(u32  deltaT)
	{
		animationState->update(deltaT / 2000.0f);
		animationState->apply(*skeleton);
		skeleton->updateWorldTransform();
		doRender(this->pImgAsset);
	}

	void
	SpineActor::doRender(CKLBImageAsset* pImgAsset)
	{
		/*

		float *vertex = new float[45 * 2]{ 59,-91,42,-91,42,191,59,191,48.03764,-91,-31.87375,-91,-31.87374,191,48.03765,191,133.4041,-101,-25.07073,-101,-25.07073,201,133.4041,201,127.4288,-91,-19.09545,-91,-19.09544,191,127.4288,191,-1.153827,-42.2418,-1.153827,142.0182,3.137791,151.04,14.97923,169.06,20.1792,173.9574,91.75794,-69.60912,89.48837,-72.5,77.78202,-82.45998,66.75631,-88.48,54.09391,-90.58,44.69975,-89.27,33.94424,-84.81,22.27426,-77.38999,12.36049,-66.51999,3.137785,-51.94999,99.15273,-60.19,91.75794,-69.60912,20.1792,173.9575,27.9222,181.25,40.4495,188.14,54.21863,190.26,67.84227,187.4,80.36957,180.85,90.03393,171.42,100.1088,158.32,108.4118,142.86,114.075,128.9155,114.075,-28.8794,107.4558,-45.24999
		};
		float *uv = new float[45*2]{ 0.2773438,0.9980469,0.2773438,0.9814453,0.001953125,0.9814453,0.001953125,0.9980469,0.4423828,0.3876953,0.3017578,0.3876953,0.3017578,0.6630859,0.4423828,0.6630859,0.2998047,0.1103516,0.001953125,0.1103516,0.001953125,0.4052734,0.2998047,0.4052734,0.5771484,0.1103516,0.3017578,0.1103516,0.3017578,0.3857422,0.5771484,0.3857422,0.5791016,0.1733895,0.5791016,0.2858248,0.5817203,0.2913299,0.5889459,0.3023257,0.5921189,0.3053141,0.6357962,0.15669,0.6344113,0.154926,0.6272681,0.1488484,0.6205403,0.145175,0.6128137,0.1438936,0.6070814,0.1446929,0.6005183,0.1474144,0.5933974,0.1519421,0.587348,0.158575,0.5817203,0.1674656,0.6403086,0.1624375,0.6357962,0.15669,0.592119,0.3053141,0.5968437,0.309764,0.6044879,0.3139683,0.6128898,0.3152619,0.6212029,0.3135167,0.6288471,0.3095199,0.6347442,0.3037657,0.6408919,0.2957721,0.6459584,0.2863384,0.6494141,0.2778296,0.6494141,0.1815432,0.645375,0.1715539
		};
		unsigned short *index = new unsigned short[99]{ 0,1,2,2,3,0,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,14,15,12,16,17,18,16,18,19,16,19,20,16,20,21,16,21,22,16,22,23,16,23,24,16,24,25,16,25,26,16,26,27,16,27,28,16,28,29,16,29,30,31,32,33,31,33,34,31,34,35,31,35,36,31,36,37,31,37,38,31,38,39,31,39,40,31,40,41,31,41,42,31,42,43,31,43,44
		};
		pImgAsset->m_pXYCoord = vertex;
		pImgAsset->m_pIndex = index;
		pImgAsset->m_pUVCoord = uv;

		pImgAsset->m_uiVertexCount = 45*2;
		pImgAsset->m_uiIndexCount =99;

		float *vertex = new float[274 * 2]{ 541.6769,797.21,506.4713,876.7686,505.0983,781.0235,469.8927,860.582,561.5534,768.9683,532.4292,834.8149,510.3394,746.3161,481.2151,812.1627,729.1519,654.9328,683.1761,852.658,524.6086,607.3717,478.6329,805.0969,555.7819,550.6631,559.8417,546.0834,563.3365,542.1526,567.5789,540.9512,572.8401,539.4485,588.4539,534.9724,613.7604,514.1514,613.5309,498.1799,562.5018,498.99,535.7536,499.2365,500.5505,499.622,500.9437,534.7618,516.5967,559.4073,548.3351,559.0652,542.7437,662.5099,540.969,756.4932,487.7535,661.4716,485.9788,755.4548,564.9893,512.2419,566.4401,690.236,489.9918,512.8531,491.4425,690.8472,495.479,870.1084,483.2593,909.2451,461.1151,859.3789,448.8954,898.5156,475.6449,880.4147,515.9794,862.233,516.7265,857.7809,527.8547,791.8454,528.5276,787.8937,521.4158,784.8459,528.8195,757.4233,515.9374,727.8027,489.3895,723.3202,467.6794,719.6507,447.955,757.9494,441.4228,773.3241,434.4061,789.8468,425.1113,811.7451,421.5663,832.7545,417.267,858.2038,425.4115,895.0686,438.3096,897.2464,489.8613,855.1365,498.8174,823.093,513.8427,785.9338,496.3593,791.5848,475.1419,833.241,445.6666,839.7566,457.0303,799.6878,473.4915,758.0433,442.0421,866.8292,467.6815,868.1141,503.5036,768.3141,512.6864,776.6215,451.0404,820.8254,487.5384,808.8871,464.8126,779.9936,471.6214,849.6785,493.0728,776.5571,475.5957,871.9696,447.8256,768.269,430.3486,863.6815,455.7829,1178.088,502.1109,1162.972,524.5015,1131.146,518.5063,1165.757,542.0142,1154.897,563.8823,1113.834,560.7037,1095.951,557.8128,1079.779,546.0773,1066.308,544.9611,1024.756,548.3146,995.3662,574.4178,1008.213,584.1097,988.3459,580.746,977.0911,566.7245,967.804,574.6276,956.6265,563.1992,918.3594,497.3285,883.2914,429.6349,896.5196,409.6293,900.4218,376.9278,923.1268,347.5984,943.4827,323.0458,989.5914,314.9878,1053.922,343.9058,1090.217,405.1223,1123.788,396.4137,1142.013,365.4703,1150.642,403.4456,1170.852,519.5988,1110.871,501.747,1077.345,473.2065,1102.431,461.6617,1145.393,405.2378,1157.783,404.7444,1094.277,360.0235,1058.09,497.7413,1072.415,455.5614,1069.317,412.2198,1062.433,379.7939,1051.947,497.265,1070.597,455.7428,1067.136,413.1063,1059.833,380.6095,1047.256,548.7123,1078.828,441.1631,942.9649,420.8608,958.4445,403.0561,962.7729,387.6449,952.5155,383.5918,935.3596,391.9849,919.8364,411.154,907.7449,427.9907,909.3078,444.515,922.1523,452.4761,947.9855,400.7149,976.7498,371.8463,949.118,449.901,921.1924,563.2844,959.3091,554.8815,956.6334,535.4415,972.1633,554.1805,988.5315,484.3987,927.0737,425.5739,993.8477,495.3557,1055.305,489.7137,633.2589,514.4734,742.4878,445.8271,643.207,470.5868,752.4359,500.1294,545.4998,503.6953,540.6949,507.1687,536.0265,510.5085,535.0266,516.101,533.3618,521.3865,531.7859,551.0967,512.439,550.8576,492.0072,507.4237,492.5108,484.0273,492.813,424.9236,493.0905,425.1524,535.0432,447.7807,561.9626,488.0435,561.7453,494.4256,678.6619,502.6285,665.724,502.654,649.5479,494.6936,632.2655,485.2697,623.6335,484.7467,615.9233,484.062,605.7254,483.983,594.392,489.7736,571.9958,497.1865,551.4787,499.2029,545.9258,484.9651,527.9572,466.9691,505.2523,442.9809,505.2079,420.9453,505.1634,421.1633,512.3145,421.9882,540.0585,423.116,577.6512,427.5648,606.111,436.2048,630.5322,444.9217,645.0818,445.4219,661.7216,451.6591,678.3643,464.2677,689.2515,478.4207,689.2751,454.5349,556.804,454.1865,588.4855,454.4376,605.7675,459.4418,624.4925,448.0444,638.0145,461.8791,630.9789,474.3926,565.0566,470.5969,592.2584,469.9886,606.2261,472.1676,620.1895,474.6001,626.9885,443.5263,628.6242,436.2997,606.0151,434.3294,581.3901,435.8351,547.8462,462.0999,643.9166,469.4122,656.2001,469.9525,669.7356,462.739,681.283,479.8831,636.2014,488.169,652.0076,491.8081,668.3715,485.8578,680.7618,457.4212,660.6814,441.8082,512.1647,464.5185,515.4035,482.6412,536.1214,558.1378,926.2877,476.0417,882.5883,448.314,934.667,530.4101,978.3665,426.4637,1068.897,480.1477,1075.061,536.7333,1072.866,547.6568,1082.238,567.497,1061.357,564.9568,1042.57,553.6307,1025.349,561.3779,999.9858,545.6365,976.953,544.9637,951.6836,512.732,946.0664,482.6692,940.82,453.914,950.1669,429.3069,946.3584,409.1911,957.8911,388.6647,937.5181,397.9081,920.2631,386.8174,905.2824,324.8614,961.798,309.3907,990.8497,322.3762,1007.404,343.569,1012.311,363.9404,1017.032,371.2553,1042.989,379.8363,1051.802,452.6279,960.4707,430.4101,982.7494,415.2049,1011.386,423.446,1031.395,451.7398,1037.232,491.4081,1037.458,530.9239,1030.316,545.7939,1022.941,549.6339,1045.547,492.0596,1051.111,450.3204,1052.462,419.199,1051.576,391.0789,1047.455,389.3114,1023.263,396.9355,1003.556,424.637,974.496,447.2124,955.9133,412.4622,964.1752,372.2002,994.8122,345.1914,978.9446,464.7162,1014.861,441.7761,1014.918,458.2065,995.5709,500.3024,1004.43,528.6576,1002.093,502.3175,1035.498,493.85,733.3552,479.4359,812.0459,436.7992,722.9049,422.3851,801.5957,538.732,681.4633,523.9717,762.1239,464.9571,667.963,450.1967,748.6236
		};
		float *uv = new float[274 * 2]{ 0.1015625,0.01953125,0.05859375,0.01953125,0.1015625,0.09765625,0.05859375,0.09765625,0.8642578,0.4023438,0.8642578,0.2617188,0.8369141,0.4023438,0.8369141,0.2617188,0.8818359,0.9921875,0.8818359,0.59375,0.7792969,0.9921875,0.7792969,0.59375,0.6743845,0.3912539,0.6764013,0.4001063,0.6781318,0.4077035,0.6802126,0.4099406,0.6828015,0.4127238,0.6905127,0.421016,0.703125,0.4609535,0.703125,0.4921875,0.6779821,0.4920504,0.6648064,0.4921875,0.6474609,0.4921875,0.6474609,0.4235121,0.6550485,0.375,0.6706823,0.375,0.765625,0.4296875,0.765625,0.2460938,0.7382813,0.4296875,0.7382813,0.2460938,0.4912109,0.3359375,0.4042969,0.3359375,0.4912109,0.484375,0.4042969,0.484375,0.2617188,0.7929688,0.2617188,0.7109375,0.2441406,0.7929688,0.2441406,0.7109375,0.9137011,0.685175,0.9316406,0.7333004,0.9316406,0.7421246,0.9316406,0.8727301,0.9316406,0.8805488,0.9279675,0.8841137,0.9293035,0.9393301,0.9206941,0.9921875,0.9075497,0.9921875,0.8967956,0.9921875,0.8904127,0.9120172,0.8885196,0.8802851,0.8864855,0.8461871,0.8837891,0.8009867,0.8837891,0.7593758,0.8837891,0.7089547,0.8907098,0.640625,0.8970928,0.640625,0.9184934,0.7384824,0.9201998,0.8031031,0.9244093,0.8795574,0.9164511,0.8629848,0.9096222,0.7758711,0.8959639,0.7537227,0.8981747,0.8345961,0.9027158,0.9201418,0.8964166,0.7004153,0.9088681,0.7062758,0.9179991,0.9101152,0.9230996,0.8971109,0.8970085,0.7919375,0.9136135,0.8267844,0.9003218,0.8750434,0.9092667,0.7430703,0.2666016,0.9921875,0.2666016,0.8007813,0.2441406,0.9921875,0.2441406,0.8007813,0.102783,0.1410134,0.1193487,0.2096116,0.1217155,0.2850603,0.1270944,0.2198961,0.1347656,0.2601959,0.1347656,0.351086,0.1292725,0.3789827,0.1242973,0.4042428,0.1161148,0.4166809,0.1060622,0.4873103,0.1007444,0.5410493,0.1149978,0.5428711,0.1146153,0.586017,0.1105685,0.6023314,0.1023581,0.6054685,0.1032054,0.6320208,0.08944336,0.6875,0.05286945,0.6875,0.02662969,0.6025467,0.01887609,0.5774437,0.009955078,0.5082867,0.001953125,0.4462597,0.001953125,0.3442238,0.01328203,0.2259027,0.03415086,0.1898866,0.06835938,0.1881754,0.06878968,0.1487311,0.05738641,0.1054688,0.07847172,0.1054688,0.1149221,0.3155064,0.09947867,0.3569412,0.09290445,0.2874757,0.09779727,0.2027727,0.07623649,0.1296463,0.06140531,0.2387248,0.03372984,0.2600504,0.09661125,0.3617721,0.07764696,0.3283867,0.05730672,0.3005133,0.04086719,0.2887911,0.09598703,0.3644843,0.07722594,0.3323212,0.0570982,0.3057806,0.04014203,0.297638,0.1201403,0.3975146,0.04231352,0.5330464,0.03709531,0.4877178,0.03039359,0.4639069,0.02136102,0.4674573,0.01565406,0.4933111,0.01570984,0.5277731,0.02121758,0.566228,0.02886492,0.5789803,0.03897195,0.5720192,0.04836711,0.534775,0.03259695,0.4376515,0.01374422,0.4588025,0.04108102,0.578602,0.09891421,0.6169753,0.09466289,0.6138614,0.08982985,0.5692371,0.8876953,0.5859375,0.8417969,0.5859375,0.8417969,0.4101563,0.8876953,0.4101563,0.05664063,0.0078125,0.001953125,0.0078125,0.05664063,0.09765625,0.001953125,0.09765625,0.2416335,0.05545974,0.2433808,0.0649125,0.2450745,0.07407266,0.2466907,0.07594299,0.2494168,0.07909572,0.2519984,0.08208162,0.2666016,0.1196533,0.2666016,0.1601563,0.2453864,0.1601563,0.2339468,0.1601549,0.2050781,0.1601563,0.2050781,0.07702714,0.2160576,0.0234375,0.2357143,0.0234375,0.6546115,0.5159659,0.6609392,0.5,0.6688374,0.5,0.6772674,0.5156152,0.6814739,0.5340556,0.6852384,0.5351046,0.6902166,0.5364916,0.6957483,0.536679,0.7066904,0.5254744,0.716716,0.5110588,0.7194293,0.5071574,0.7281882,0.5350326,0.7392578,0.5702605,0.7392578,0.6171223,0.7392578,0.6601563,0.7357647,0.6597095,0.7222216,0.6579813,0.7038639,0.6556367,0.6899713,0.6468248,0.678058,0.6298739,0.6709586,0.6127868,0.6628376,0.6117314,0.6547167,0.5994971,0.6494141,0.5748186,0.6494141,0.5471852,0.7140728,0.5943416,0.6986044,0.5949038,0.6901672,0.5943416,0.6810265,0.5844984,0.6744167,0.6067169,0.6778622,0.5797162,0.7100649,0.5555294,0.6967761,0.5628421,0.6899561,0.5639664,0.6831441,0.559663,0.6798262,0.5548855,0.6789942,0.6155639,0.6900261,0.6297795,0.7020499,0.633716,0.718432,0.6309037,0.6715462,0.5792261,0.6655563,0.5649065,0.6589474,0.5638046,0.6533016,0.5778488,0.6753331,0.5445266,0.6676218,0.5282788,0.6596356,0.5211182,0.6535774,0.5326847,0.6633533,0.5883133,0.7358608,0.6193708,0.7342985,0.5750028,0.7242,0.5395426,0.6914063,0.3671875,0.6455078,0.3671875,0.6455078,0.25,0.6914063,0.25,0.7180522,0.6813071,0.742703,0.7199605,0.7666834,0.7756839,0.7735647,0.7695432,0.7773438,0.8237656,0.7719144,0.8538433,0.7630476,0.8731344,0.7605568,0.9239783,0.7484367,0.9492285,0.7423269,0.9921875,0.7270853,0.9722902,0.7128685,0.9537287,0.7025791,0.9112204,0.6910514,0.8951942,0.6850041,0.8568488,0.6714308,0.8731376,0.6714551,0.911363,0.6632062,0.927013,0.6494141,0.7727044,0.6494141,0.7084183,0.6588463,0.6917794,0.6691446,0.7027704,0.679049,0.7133399,0.6881934,0.6752831,0.6939362,0.6679688,0.704397,0.892273,0.6999156,0.8334565,0.699931,0.7701366,0.7081056,0.743181,0.7216956,0.7591034,0.738915,0.7951144,0.7543663,0.8437083,0.7590984,0.8700964,0.765967,0.8346399,0.7423409,0.7721953,0.7245907,0.7315448,0.7109163,0.704505,0.6978009,0.685804,0.6914645,0.7258937,0.6902236,0.7668555,0.6955148,0.8423952,0.7010043,0.8951553,0.6878685,0.849006,0.6775036,0.7592363,0.6621597,0.7618074,0.7221587,0.8095875,0.7122403,0.7884355,0.7148949,0.836864,0.73515,0.8602434,0.7468837,0.8903018,0.7431814,0.8085338,0.8349609,0.4140625,0.8349609,0.2578125,0.8066406,0.4140625,0.8066406,0.2578125,0.8046875,0.4140625,0.8046875,0.2539063,0.7675781,0.4140625,0.7675781,0.2539063
		};

		//float *uv = new float[16 * 2]{ 0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,1,1,0,1
		//};

		//unsigned short *index = new unsigned short[24]{ 0,1,2,2,3,0,4,5,6,6,7,4,8,9,10,10,11,8,12,13,14,14,15,12
		//};
		unsigned short *index = new unsigned short[1017]{ 0,2,1,2,3,1,4,6,5,6,7,5,8,10,9,10,11,9,20,15,16,20,16,17,20,17,18,20,18,19,23,13,22,15,21,14,14,22,13,24,25,12,12,23,24,13,23,12,14,21,22,15,20,21,26,28,27,28,29,27,30,32,31,32,33,31,34,36,35,36,37,35,57,56,40,51,52,61,61,71,60,60,71,56,52,53,61,71,64,65,71,61,64,61,53,64,71,65,56,56,39,40,65,38,56,56,38,39,53,54,64,38,65,55,55,65,54,65,64,54,49,62,70,49,50,62,41,58,57,70,69,59,70,62,69,57,40,41,59,69,57,50,68,62,50,51,68,62,60,69,62,68,60,69,60,57,60,56,57,51,61,68,68,61,60,46,66,45,45,67,44,45,66,67,47,63,46,46,63,66,47,48,63,67,43,44,48,70,63,63,59,66,63,70,59,48,49,70,66,59,67,67,58,43,67,59,58,42,43,41,43,58,41,58,59,57,72,74,73,74,75,73,110,101,107,113,114,110,107,108,105,107,113,110,113,117,114,106,107,105,112,113,107,109,103,104,102,103,109,76,109,104,108,109,76,108,76,77,109,101,102,109,108,101,107,101,108,78,108,77,78,79,80,78,105,108,78,80,81,105,78,81,82,105,81,106,112,107,106,105,82,120,106,82,112,106,120,110,100,101,111,99,100,111,100,110,115,111,110,115,98,111,114,115,110,118,115,114,119,115,118,117,118,114,98,99,111,119,98,115,116,113,112,117,113,116,83,120,82,84,112,120,116,112,84,84,120,83,131,98,119,132,97,98,131,132,98,131,124,132,123,124,131,85,116,84,131,130,122,118,131,119,123,131,122,125,132,124,96,97,132,96,132,125,126,125,124,96,125,126,122,130,121,130,131,117,131,118,117,85,136,116,122,127,126,136,117,116,86,136,85,130,117,136,122,128,127,95,126,127,126,124,123,123,122,126,122,121,128,96,126,95,133,129,121,133,121,130,129,128,121,88,86,87,89,86,88,94,127,128,95,127,94,94,128,129,94,129,133,90,86,89,136,86,90,135,136,90,134,135,90,134,90,91,93,130,136,92,93,136,133,130,93,94,133,93,135,92,136,92,135,134,92,134,91,138,140,137,138,139,140,141,143,142,143,144,142,153,154,148,149,153,148,150,153,149,151,153,150,153,151,152,156,146,155,145,157,158,145,156,157,145,146,156,154,147,148,146,147,155,154,155,147,183,159,206,202,182,183,206,202,183,202,181,182,201,202,206,205,206,159,201,206,205,205,159,160,207,181,202,207,202,201,180,181,207,200,207,201,204,201,205,200,201,204,205,160,161,204,205,161,199,207,200,207,179,180,188,207,199,188,179,207,203,200,204,199,200,203,162,204,161,203,204,162,189,188,199,194,189,199,195,178,179,195,179,188,203,194,199,187,188,189,163,203,162,194,203,163,193,189,194,164,194,163,193,187,189,192,187,193,196,178,195,177,178,196,186,188,187,186,187,192,195,188,186,196,195,186,164,193,194,165,193,164,192,193,165,165,191,192,166,191,165,185,196,186,197,177,196,197,196,185,176,177,197,190,191,166,190,166,167,191,184,185,197,185,184,186,192,191,191,185,186,198,197,184,176,197,198,175,176,198,210,190,167,210,167,168,170,210,168,170,168,169,190,209,184,190,184,191,209,190,210,208,198,184,208,184,209,174,175,198,208,174,198,172,208,209,173,174,208,172,173,208,171,209,210,171,210,170,172,209,171,212,213,214,212,214,211,264,223,263,224,263,223,227,240,226,263,224,225,262,263,225,262,225,240,240,225,226,223,264,222,232,230,231,232,233,230,264,247,222,222,247,221,256,257,255,227,256,240,256,227,257,228,229,257,227,228,257,256,255,240,255,241,240,240,241,262,264,246,247,246,264,265,233,259,230,257,229,259,229,230,259,220,221,248,221,247,248,247,246,248,262,260,263,264,263,265,265,260,245,265,263,260,257,259,258,220,248,219,257,254,255,257,258,254,246,265,248,255,254,241,260,262,261,248,217,219,217,248,249,262,241,261,241,242,261,241,254,242,217,218,219,245,260,244,245,249,265,248,265,249,260,261,244,245,244,249,242,243,261,261,243,244,233,234,259,244,250,249,217,249,216,249,250,216,243,242,253,242,254,253,254,258,253,259,234,236,259,236,258,236,234,235,258,237,253,258,236,237,244,243,250,243,251,250,243,253,251,251,215,250,250,215,216,237,238,253,253,252,251,252,238,239,252,253,238,251,252,215,252,239,215,266,268,267,268,269,267,270,272,271,272,273,271
		};

		pImgAsset->m_pXYCoord = vertex;
		pImgAsset->m_pIndex = index;
		pImgAsset->m_pUVCoord = uv;

		pImgAsset->m_uiVertexCount = 274*2;
		pImgAsset->m_uiIndexCount = 1017;
		return;
		*/
		//pAsset->m_indexBufferTotal = index;
		//pAsset->m_floatBufferTotal = uv;
		//pAsset->m_totalVertexCount = 16;
		//pAsset->m_totalIndexCount = 24;

		int baseIndex = 0;
		vertextIndex = 0;
		indexIndex = 0;
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
			if (slot->getColor().a == 0 || !slot->getBone().isActive() || slot->getData().getName() == "shine")
				continue;

			// Fill the vertices array, indices, and texture depending on the type of attachment
			if (attachment->getRTTI().isExactly(spine::RegionAttachment::rtti))
			{
				// Cast to an spRegionAttachment so we can get the rendererObject
				// and compute the world vertices
				regionAttachment = (spine::RegionAttachment*)attachment;

				if (regionAttachment->getColor().a == 0)
					continue;


				float* positions = KLBNEWA(float, 8);
				//(pAsset->m_pImages[0]->m_pUVCoor
				regionAttachment->computeWorldVertices(slot->getBone(), (float*)positions, 0, 2);
				//regionAttachment->getRendererObject()
				// copy color and UVs to the vertices
				//memmove(pAsset->m_pImages[0]->m_pXYCoord, positions, sizeof(float) * 8);
				isRegion = true;
				floatCopy(pImgAsset->m_pXYCoord, positions, pImgAsset->m_pUVCoord,8);
				//KLBDELETE(positions);
				unsigned short* indices = KLBNEWA(unsigned short, 6);

				indices[0] = 0 + baseIndex;
				indices[1] = 1 + baseIndex;
				indices[2] = 2 + baseIndex;
				indices[3] = 2 + baseIndex;
				indices[4] = 3 + baseIndex;
				indices[5] = 0 + baseIndex;

				//memmove(pAsset->m_pImages[0]->m_pIndex, &indices, sizeof(unsigned short) * 6);
				shortCopy(pImgAsset->m_pIndex, indices,6);
				//KLBDELETE(indices);
				baseIndex += 4;
			}
			else if (attachment->getRTTI().isExactly(spine::MeshAttachment::rtti))
			{
				mesh = (spine::MeshAttachment*)attachment;

				size_t numVertices = mesh->getWorldVerticesLength();
				//vertices.resize(numVertices);

				float* positions = KLBNEWA(float, numVertices);

				mesh->computeWorldVertices(*slot, 0, numVertices, positions, 0, 2);

				//memmove(pAsset->m_pImages[0]->m_pXYCoord, positions, sizeof(float) * 8);

				//KLBDELETE(positions);
				isRegion = false;
				unsigned short* sind = mesh->getTriangles().buffer();
				int numInd = mesh->getTriangles().size();
				floatCopy(pImgAsset->m_pXYCoord, positions, pImgAsset->m_pUVCoord, numVertices);

				unsigned short* indices = KLBNEWA(unsigned short, numInd);

				for (size_t i = 0; i < numInd; i++)
				{
					*(indices + i) = *(sind + i) + baseIndex;
				}
				//memmove(pAsset->m_pImages[0]->m_pIndex, &indices, sizeof(unsigned short) * 6);
				shortCopy(pImgAsset->m_pIndex, indices, numInd);
				//KLBDELETE(indices);
				baseIndex += (numVertices / 2);

			}
		}
		pImgAsset->m_uiVertexCount = vertextIndex;
		pImgAsset->m_uiIndexCount = indexIndex;
	}

	void 
	SpineActor::changerAnimation(const char *animation,bool isloop)
	{
		animationState->setAnimation(0, animation, isloop);
	}
	

}