#ifndef INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE
#define INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE

#include <allegro5/allegro.h>

#include <memory>

namespace al {

	class IResourceHandle {
	public:
		virtual void load() = 0;
		virtual void unload() = 0;
		virtual bool isLoaded() = 0;
		virtual bool isUsed() = 0;
		virtual double timeSinceLastUse() = 0;
	};
	
	template<typename T>
	class ResourceHandle: public IResourceHandle {
	public:
		ResourceHandle() : lastUsed(-1437.0f) {}
		virtual ~ResourceHandle() {}

		virtual T* loader() = 0;

		virtual void load() override
		{
			resource = std::shared_ptr<T>(loader());
		}

		virtual void unload() override
		{
			resource.reset();
		}

		virtual bool isLoaded() override
		{
			return resource.get();
		}

		std::shared_ptr<T> ptr()
		{
			if(!isLoaded()) {
				load();
			}
			lastUsed = al_get_time();
			return resource;
		}

		T& operator*()
		{
			return *(ptr().get());
		}

		T* operator->()
		{
			return ptr().get();
		}

		virtual double timeSinceLastUse() override
		{
			return al_get_time() - lastUsed;
		}

		virtual bool isUsed() override
		{
			return !resource.unique();
		}

		long refCount()
		{
			return resource.use_count();
		}
	protected:
		std::shared_ptr<T> resource;
		double lastUsed;
	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE */
