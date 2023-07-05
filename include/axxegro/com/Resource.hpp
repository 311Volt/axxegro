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

#include <type_traits>
#include <memory>
#include <variant>
#include "axxegro/com/util/Metaprogramming.hpp"

namespace al {

	template<typename T>
	struct Deleter {
		AXXEGRO_STATIC_ASSERT_FALSE(T, "Deleter for T not defined. Use AXXEGRO_DEFINE_DELETER");
	};

	enum class ResourceModel: char {
		Owning,
		NonOwning
	};

	#define AXXEGRO_DEFINE_DELETER(type, delfn) \
		template<> struct Deleter<type>{ \
			inline void operator()(type* p){delfn(p);} \
		}


	template<typename T, typename Deleter = ::al::Deleter<T>>
	class Resource {
	private:
		T* ptr_;
		void (*dtor)(T*);
	public:
		using ManagedType = T;

		explicit Resource(T* p, ResourceModel model = ResourceModel::Owning)
		{
			if(model == ResourceModel::Owning) {
				dtor = [](T* p){
					Deleter{}(p);
				};
			} else {
				dtor = nullptr;
			}
			setPtr(p);
		}

		Resource(Resource&& rhs) noexcept {
			(*this) = std::move(rhs);
		}

		Resource& operator=(Resource&& rhs) noexcept {
			ptr_ = rhs.ptr_;
			dtor = rhs.dtor;
			rhs.ptr_ = nullptr;
			rhs.dtor = nullptr;
			return *this;
		};

		Resource(const Resource&) = delete;
		Resource& operator=(const Resource&) = delete;

		virtual ~Resource() {
			if(dtor) {
				dtor(ptr_);
			}
		}

		T* ptr()
		{
			return getPointer();
		}

		T* constPtr() const //TODO remove this
		{
			return getPointer();
		}

		T* ptr() const
		{
			return getPointer();
		}
	protected:
		void setPtr(T* p)
		{
			ptr_ = p;
		}

		ResourceModel getResourceModel() {
			if(dtor) {
				return ResourceModel::Owning;
			} else {
				return ResourceModel::NonOwning;
			}
		}
	private:
		virtual T* getPointer() const
		{
			return ptr_;
		}
	};

}

#endif // __AXXEGRO_RESOURCE_H__
