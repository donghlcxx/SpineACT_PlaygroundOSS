#include "spine.h"
#include <spine/spine.h>
#include <spine/BlendMode.h>
#include <spine/Atlas.h>
#include <spine/RegionAttachment.h>
#include <spine/Debug.h>


namespace spine
{
    static DebugExtension *debugExtension = NULL;

    class OxSpineExt : public DefaultSpineExtension
    {
    public:
		char *_readFile(const String &path, int *length) override
		{
			return 0;

		}
    };


    SpineExtension* getDefaultExtension() {
        // return a default spine extension that uses standard malloc for memory
        // management, and wrap it in a debug extension.

        static OxSpineExt ext;
        return &ext;

        //default DebugExtension is NOT THREAD SAFE, not recommended!!!
        debugExtension = new DebugExtension(new OxSpineExt());
        return debugExtension;
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
			/*
			spNativeTexture texture = IVideoDriver::instance->createTexture();
			texture->addRef();
			//Texture* texture = engine_loadTexture(path);


			file::buffer buf;
			file::read(path.buffer(), buf);

			Image im;
			im.init(buf, true);


			texture->init(im.lock());

			// store the Texture on the rendererObject so we can
			// retrieve it later for rendering.
			page.setRendererObject(texture.get());

			// store the texture width and height on the spAtlasPage
			// so spine-c can calculate texture coordinates for
			// rendering.
			page.width = texture->getWidth();
			page.height = texture->getHeight();
			*/
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
	/*

	void SpineActor::setSkeleton(spine::Skeleton* sk)
	{
		_skeleton = sk;
	}


	void SpineActor::setAnimationState(spine::AnimationState *st)
	{
		_animationState = st;
	}

	void SpineActor::doUpdate(const UpdateState& us)
	{
		_animationState->update(us.dt / 1000.0f);
		_animationState->apply(*_skeleton);
		_skeleton->updateWorldTransform();
	}
	*/

}