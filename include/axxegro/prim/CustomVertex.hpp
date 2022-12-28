#ifndef INCLUDE_AXXEGRO_PRIM_CUSTOMVERTEX
#define INCLUDE_AXXEGRO_PRIM_CUSTOMVERTEX

#include "axxegro/resources/Resource.hpp"
#include "tcb/span.hpp"
#include <allegro5/allegro_primitives.h>
#include <type_traits>
#include <utility>

namespace al {

	//i have no idea if any of this metaprogramming mess is even a good idea
	//probably not lol

	//max value of a vertex attribute
	constexpr int MAX_PRIM_ATTR = ALLEGRO_PRIM_USER_ATTR + 10 + 1;

	/// @brief A compile-time representation of a vertex element.
	template<int ATTRID, int STORAGE, int OFFSET>
	struct VertexElementT {
		static constexpr bool IsActive = OFFSET >= 0;
		static constexpr ALLEGRO_VERTEX_ELEMENT Create()
		{
			return {ATTRID, STORAGE, OFFSET};
		}
	};

	class VertexDecl;

	/// @brief A compile-time representation of a custom vertex declaration.
	template<typename CustomVertexT>
	class BasicVertexDecl {
	public:
		using VertexT = CustomVertexT;
		static_assert(
			std::is_standard_layout_v<CustomVertexT>, 
			"Cannot use non-standard layout types for vertices"
		);

		#define AXXEGRO_VERTEX_ATTR_BEGIN() template<int attr, typename xd = void> \
			struct Attr {using Type = ::al::VertexElementT<attr, -1, -1>;};

		#define AXXEGRO_VERTEX_ATTR(m, attrid, storage) template<typename xd> struct Attr<attrid, xd> \
			{using Type = ::al::VertexElementT<attrid, storage, offsetof(VertexT, m)>;};
	};

	///@brief Maps a vertex type to its BasicVertexDecl
	template<typename T>
	class VertexDeclFor {
		static_assert(std::is_void_v<T>, "The type T does not have a vertex declaration associated with it. ");
	};

	#define AXXEGRO_VERTEX_DECL(vtype) template<> struct al::VertexDeclFor<vtype>: public ::al::BasicVertexDecl<vtype>

	/**
	 * @brief Create an array of vertex elements 
	 * 
	 * @tparam VDecl A subclass of CustomVertexDecl
	 * @return constexpr std::array<ALLEGRO_VERTEX_ELEMENT, MAX_PRIM_ATTR> 
	 */
	template<typename VDecl>
	constexpr std::array<ALLEGRO_VERTEX_ELEMENT, MAX_PRIM_ATTR+1> CreateCustomVertexAttrArr()
	{
		std::array<ALLEGRO_VERTEX_ELEMENT, MAX_PRIM_ATTR+1> ret {};
		for(auto& x: ret)
			x = {0,0,0};
		int k = 0;

		#define FINNA_KMS_XD(x) \
			using hu##x = typename VDecl::template Attr<x, void>; \
			if constexpr(hu##x::Type::IsActive) {\
				ret[k++] = hu##x::Type::Create(); \
			}
		
		static_assert(MAX_PRIM_ATTR == 16);
		FINNA_KMS_XD(0); FINNA_KMS_XD(1); FINNA_KMS_XD(2); FINNA_KMS_XD(3);
		FINNA_KMS_XD(4); FINNA_KMS_XD(5); FINNA_KMS_XD(6); FINNA_KMS_XD(7);
		FINNA_KMS_XD(8); FINNA_KMS_XD(9); FINNA_KMS_XD(10); FINNA_KMS_XD(11);
		FINNA_KMS_XD(12); FINNA_KMS_XD(13); FINNA_KMS_XD(14); FINNA_KMS_XD(15);
		#undef FINNA_KMS_XD

		return ret;
	}


	AXXEGRO_DEFINE_DELETER(ALLEGRO_VERTEX_DECL, al_destroy_vertex_decl);

	/// @brief Represents an ALLEGRO_VERTEX_DECL
	class VertexDecl: public Resource<ALLEGRO_VERTEX_DECL> {
	public:
		VertexDecl(const ALLEGRO_VERTEX_ELEMENT* elements, int stride)
			: Resource(al_create_vertex_decl(elements, stride))
		{

		}

		const ALLEGRO_VERTEX_DECL* get() 
		{
			return ptr();
		}

		template<typename VDecl>
		static VertexDecl Init()
		{
			static constexpr auto elements = CreateCustomVertexAttrArr<VDecl>();
			return VertexDecl(elements.data(), sizeof(typename VDecl::VertexT));
		}
	};

}

#endif /* INCLUDE_AXXEGRO_PRIM_CUSTOMVERTEX */
