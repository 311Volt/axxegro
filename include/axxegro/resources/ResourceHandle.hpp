#ifndef INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE
#define INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE

#include <allegro5/allegro.h>

#include <memory>

namespace al {
	
	template<typename T>
	class ResourceHandle {
	public:
		ResourceHandle() : lastUsed(-1437.0f) {}
		virtual ~ResourceHandle() {unload();}

		virtual void load() = 0;

		void unload()
		{
			resource = std::unique_ptr<T>();
		}

		bool isLoaded()
		{
			return resource.get();
		}

		T* getPtr()
		{
			if(!isLoaded()) {
				load();
			}
			lastUsed = al_get_time();
			return resource.get();
		}

		T& get()
		{
			return *getPtr();
		}

		double timeSinceLastUse()
		{
			return al_get_time() - lastUsed;
		}
	protected:
		std::unique_ptr<T> resource;
		double lastUsed;
	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_RESOURCEHANDLE */
