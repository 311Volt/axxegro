#ifndef __AXXEGRO_RESOURCE_H__
#define __AXXEGRO_RESOURCE_H__

/**
 * @file
 * Resource is the base for classes that wrap around an Allegro object
 * that has a create/destroy pair, for example bitmaps or fonts.
 * A Resource is characterized by the following:
 *     - it provides RAII, obviously
 *     - it is neither copy-constructible non copy-assignable (but it may
 *           provide a more explicit way of copying, namely a clone() method)
 *     - it may not own the resource (details below)
 *     - it duplicates the Allegro API where applicable (e.g. bmp.draw(...)
 *           instead of al_draw_bitmap(bmp.ptr(), ...))
 *     - the above includes convenient ctor overloads, e.g.:
 *           (al_create_bitmap(800,600) -> al::Bitmap::Bitmap(800,600))
 *     - it points to a single valid structure for its entire lifetime
 * 
 * 
 * Often, access is needed to objects whose lifetimes are managed by Allegro
 * rather than the user directly. Examples include target bitmaps, current 
 * displays or display backbuffers. The axxegro approach to dealing with
 * these is:
 *     1. derive from the original wrapper class (e.g. Bitmap -> TargetBitmap)
 *     2. construct it with alPtr := null and override getPointer() 
 *            and make it return al_get_* directly
 *     3. provide access to a Derived object globally
 *     4. allegro functions that implicitly use such objects can be implemented
 *            in the derived classes
 * 
 * See CCurrentDisplay or CDisplayBackbuffer in Display.cpp for examples.
 **/

#include <stdexcept>
#include <memory>

namespace al {

	class ResourceLoadError: public std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	template<typename T, typename Deleter>
	class Resource {
		std::unique_ptr<T, Deleter> alPtr;
	public:
		Resource(T* p)
		{
			setPtr(p);
		}

		T* ptr()
		{
			return getPointer();
		}

#ifndef AXXEGRO_TRUSTED
	private:
#endif
		T* ptr() const
		{
			return getPointer();
		}
	protected:
		void setPtr(T* p)
		{
			alPtr = decltype(alPtr)(p);
		}
	private:
		virtual T* getPointer() const
		{
			return alPtr.get();
		}
	};

}

#endif // __AXXEGRO_RESOURCE_H__