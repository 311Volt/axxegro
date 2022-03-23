#ifndef INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE
#define INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE

#include <allegro5/allegro.h>

#include <memory>

namespace al {

	class IResourceHandle {
	public:
		virtual void load() = 0;
		virtual void unload() = 0;
		bool isLoaded();
		double timeSinceLastUse();
	};
	
	template<typename T>
	class ResourceHandle: public IResourceHandle {
	public:
		ResourceHandle() : lastUsed(-1437.0f) {}
		virtual ~ResourceHandle() {}

		virtual T* loader() = 0;

		void load()
		{
			resource = std::shared_ptr<T>(loader());
		}

		void unload()
		{
			resource.reset();
		}

		bool isLoaded()
		{
			return resource.get();
		}

		std::shared_ptr<T> getPtr()
		{
			if(!isLoaded()) {
				load();
			}
			lastUsed = al_get_time();
			return resource;
		}

		T& operator*()
		{
			return *(getPtr().get());
		}

		T* operator->()
		{
			return getPtr().get();
		}

		T& get()
		{
			return *getPtr();
		}

		double timeSinceLastUse()
		{
			return al_get_time() - lastUsed;
		}

		bool isUsed()
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
