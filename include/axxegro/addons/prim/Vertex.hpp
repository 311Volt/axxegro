#ifndef INCLUDE_AXXEGRO_PRIM_CUSTOMVERTEX
#define INCLUDE_AXXEGRO_PRIM_CUSTOMVERTEX

#include "common.hpp"

#include <type_traits>
#include <utility>
#include <array>

#include "../../com/util/General.hpp"
#include "../../com/util/Metaprogramming.hpp"

namespace al {

	using BasicVertex = ALLEGRO_VERTEX;

	inline BasicVertex CreateBasicVertex(const Vec3f pos = {0, 0, 0}, const Vec2f uv = {0, 0}, Color color = al::White) {
		return BasicVertex {
			.x = pos.x,
			.y = pos.y,
			.z = pos.z,
			.u = uv.x,
			.v = uv.y,
			.color = color
		};
	}

	//max value of a vertex attribute
	constexpr int MaxCustomVertexElements = 10 + ALLEGRO_PRIM_USER_ATTR + 1;

	using VertexAttrArr = std::array<ALLEGRO_VERTEX_ELEMENT, MaxCustomVertexElements + 1>;



	template<typename T>
	struct VertexAttrArrGetter {
		AXXEGRO_STATIC_ASSERT_FALSE(T, "Not a valid vertex type");
	};


	/**
	 * @brief Wraps a vector of an integral value (such as Vec2i) for use in custom vertices.
	 * This type carries the information that the vector is to be converted to a floating point value
	 * in the range (-1; 1) or (0; 1) (depending on signedness) before being sent to the GPU.
	 *
	 * @tparam VecT An instantiation of al::Vec with an integral value type.
	 */
	template<VectorType VecT> requires std::integral<typename VecT::ValueType>
	class NormalizedVec: public VecT {
		auto vec() {
			return VecT {*this};
		}

		auto toNormalizedFloat() {
			return this->f32() / std::numeric_limits<typename VecT::ValueType>::max();
		}
	};


	namespace detail {

		template<typename T>
		struct VertexDeclGetter {
			AXXEGRO_STATIC_ASSERT_FALSE(T, "Not a valid vertex type");
		};

		template<>
		struct VertexDeclGetter<BasicVertex> {
			static inline ALLEGRO_VERTEX_DECL* GetVertexDeclPtr() {
				return nullptr;
			}
		};

		template<typename T>
		struct VtxElemStorageType {
			AXXEGRO_STATIC_ASSERT_FALSE(T, "Vector type storage not supported");
		};

		template<> struct VtxElemStorageType<float> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_FLOAT_1;}};
		template<> struct VtxElemStorageType<Vec2f> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_FLOAT_2;}};
		template<> struct VtxElemStorageType<Vec3f> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_FLOAT_3;}};
		template<> struct VtxElemStorageType<Vec4f> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_FLOAT_4;}};
		template<> struct VtxElemStorageType<Vec2<short>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_SHORT_2;}};
		template<> struct VtxElemStorageType<Vec4<short>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_SHORT_4;}};
		template<> struct VtxElemStorageType<Vec4b> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_UBYTE_4;}};

		template<> struct VtxElemStorageType<NormalizedVec<Vec2<short>>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_NORMALIZED_SHORT_2;}};
		template<> struct VtxElemStorageType<NormalizedVec<Vec4<short>>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_NORMALIZED_SHORT_4;}};
		template<> struct VtxElemStorageType<NormalizedVec<Vec2<unsigned short>>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_NORMALIZED_USHORT_2;}};
		template<> struct VtxElemStorageType<NormalizedVec<Vec4<unsigned short>>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_NORMALIZED_USHORT_4;}};
		template<> struct VtxElemStorageType<NormalizedVec<Vec4b>> {constexpr ALLEGRO_PRIM_STORAGE operator()(){return ALLEGRO_PRIM_NORMALIZED_UBYTE_4;}};

		template<typename ElemT>
		concept VtxElem = requires {
			{VtxElemStorageType<ElemT>{}()} -> std::convertible_to<ALLEGRO_PRIM_STORAGE>;
		};

		void ConstevalFail(const char*);

		template<RangeOf<ALLEGRO_VERTEX_ELEMENT> TRange>
		inline consteval bool HasDuplicateAttrs(TRange&& range) {
			std::array<int, ALLEGRO_PRIM_ATTR_NUM+1> count {};
			for(const auto& elem: range) {
				if((++count[elem.attribute]) > 1) {
					return true;
				}
			}
			return false;
		}

		inline consteval bool IsVtxElemTerminator(ALLEGRO_VERTEX_ELEMENT elem) {
			return elem.offset == 0 && elem.storage == 0 && elem.attribute == 0;
		}

		inline consteval VertexAttrArr VertexElemVecToArr(const std::vector<ALLEGRO_VERTEX_ELEMENT>& elements) {
			if(elements.size() > MaxCustomVertexElements) {
				ConstevalFail("Too many vertex elements");
			}
			if(HasDuplicateAttrs(elements)) {
				ConstevalFail("Duplicate attributes");
			}
			VertexAttrArr result;
			result.fill({0, 0, 0});
			std::copy(elements.begin(), elements.end(), result.begin());
			if(!IsVtxElemTerminator(result.back())) {
				ConstevalFail("sanity check failed: no terminator at end (this should never happen)");
			}
			return result;
		}

		template<typename VertexT>
		inline consteval std::vector<ALLEGRO_VERTEX_ELEMENT> CreateSimpleCustomVertexDescriptionVec() {
////////////////////////MACROS BEGIN////////////////////////////////////
#define AXX_HAS_COMPATIBLE_ELEMENT_MEMBER(membername) \
requires(VertexT vtx){ \
	{vtx.membername};                                    \
    requires VtxElem<std::remove_cvref_t<decltype(vtx.membername)>>; \
}

#define AXX_TYPE_OF_VTX_MEMBER(membername) \
	std::remove_cvref_t<decltype(VertexT{}.membername)>

#define AXX_CREATE_SIMPLE_VTX_ELEM(attributeval, membername) \
ALLEGRO_VERTEX_ELEMENT { \
	.attribute = attributeval, \
	.storage = VtxElemStorageType<AXX_TYPE_OF_VTX_MEMBER(membername)>{}(), \
	.offset = offsetof(VertexT, membername) \
}
////////////////////////MACROS END////////////////////////////////////

			std::vector<ALLEGRO_VERTEX_ELEMENT> elements;
			int userAttrCounter = 0;

			auto isValidPositionStorage = [](int s) {
				return IsOneOf(s, std::array{ALLEGRO_PRIM_FLOAT_2, ALLEGRO_PRIM_FLOAT_3, ALLEGRO_PRIM_SHORT_2});
			};
			auto isValidUVStorage = [](int s) {
				return IsOneOf(s, std::array{ALLEGRO_PRIM_FLOAT_2, ALLEGRO_PRIM_SHORT_2});
			};
			auto isValidRGBAStorage = [](int s) {
				return IsOneOf(s, std::array{
					ALLEGRO_PRIM_FLOAT_4,
					ALLEGRO_PRIM_SHORT_4,
					ALLEGRO_PRIM_UBYTE_4,
					ALLEGRO_PRIM_NORMALIZED_SHORT_4,
					ALLEGRO_PRIM_NORMALIZED_USHORT_4,
					ALLEGRO_PRIM_NORMALIZED_UBYTE_4
				});
			};

			if constexpr(AXX_HAS_COMPATIBLE_ELEMENT_MEMBER(pos)) {
				auto elem = AXX_CREATE_SIMPLE_VTX_ELEM(ALLEGRO_PRIM_POSITION, pos);
				if(not isValidPositionStorage(elem.storage)) {
					ConstevalFail("Unsupported storage for position member");
				}
				elements.push_back(elem);
			}
			if constexpr(AXX_HAS_COMPATIBLE_ELEMENT_MEMBER(uvPx)) {
				auto elem = AXX_CREATE_SIMPLE_VTX_ELEM(ALLEGRO_PRIM_TEX_COORD_PIXEL, uvPx);
				if(not isValidUVStorage(elem.storage)) {
					ConstevalFail("Unsupported storage for texture coordinate member");
				}
				elements.push_back(elem);
			}
			if constexpr(AXX_HAS_COMPATIBLE_ELEMENT_MEMBER(uvNorm)) {
				auto elem = AXX_CREATE_SIMPLE_VTX_ELEM(ALLEGRO_PRIM_TEX_COORD_PIXEL, uvNorm);
				if(not isValidUVStorage(elem.storage)) {
					ConstevalFail("Unsupported storage for texture coordinate member");
				}
				elements.push_back(elem);
			}
			if constexpr(AXX_HAS_COMPATIBLE_ELEMENT_MEMBER(normal)) {
				auto elem = AXX_CREATE_SIMPLE_VTX_ELEM(ALLEGRO_PRIM_USER_ATTR + (userAttrCounter++), normal);
				elements.push_back(elem);
			}
			if constexpr(AXX_HAS_COMPATIBLE_ELEMENT_MEMBER(rgba)) {
				auto elem = AXX_CREATE_SIMPLE_VTX_ELEM(ALLEGRO_PRIM_USER_ATTR + (userAttrCounter++), rgba);
				if(not isValidRGBAStorage(elem.storage)) {
					ConstevalFail("Unsupported storage for RGBA member");
				}
				elements.push_back(elem);
			}

			if constexpr(requires(VertexT v){{v.color};})
			{
				if(std::is_same_v<AXX_TYPE_OF_VTX_MEMBER(color), ALLEGRO_COLOR>
				   		|| std::is_same_v<AXX_TYPE_OF_VTX_MEMBER(color), Color>) {
					ALLEGRO_VERTEX_ELEMENT elem {
						.attribute = ALLEGRO_PRIM_COLOR_ATTR,
						.storage = 0,
						.offset = offsetof(VertexT, color)
					};
					elements.push_back(elem);
				} else {
					ConstevalFail("Incompatible type for COLOR_ATTR storage");
				}
			}

			if(elements.size() == 0) {
				ConstevalFail("Vertex has no attributes");
			}

			return elements;

#undef AXX_HAS_COMPATIBLE_ELEMENT_MEMBER
#undef AXX_CREATE_SIMPLE_VTX_ELEM
		}

		template<typename VertexT>
		inline consteval VertexAttrArr CreateSimpleCustomVertexDescription() {
			return VertexElemVecToArr(CreateSimpleCustomVertexDescriptionVec<VertexT>());
		}

	}

	inline consteval VertexAttrArr CreateVertexAttrArr(const std::vector<ALLEGRO_VERTEX_ELEMENT>& elements) {
		return detail::VertexElemVecToArr(elements);
	}

	template<std::same_as<BasicVertex> VertexT>
	struct VertexAttrArrGetter<VertexT> {
		static consteval VertexAttrArr GetElements() {
			return CreateVertexAttrArr({
				{ALLEGRO_PRIM_POSITION, ALLEGRO_PRIM_FLOAT_3, offsetof(ALLEGRO_VERTEX, x)},
				{ALLEGRO_PRIM_TEX_COORD_PIXEL, ALLEGRO_PRIM_FLOAT_2, offsetof(ALLEGRO_VERTEX, u)},
				{ALLEGRO_PRIM_COLOR_ATTR, 0, offsetof(ALLEGRO_VERTEX, color)}
			});
		}
	};


	template<typename T>
	concept SimpleCustomVertex = requires {
		{detail::CreateSimpleCustomVertexDescription<T>()};
		requires not std::same_as<T, BasicVertex>;
	};



	template<SimpleCustomVertex VertexT>
	struct VertexAttrArrGetter<VertexT> {
		static consteval VertexAttrArr GetElements() {
			return detail::CreateSimpleCustomVertexDescription<VertexT>();
		}
	};

	template<typename T>
	concept VertexType = requires {
		{VertexAttrArrGetter<T>::GetElements()} -> std::convertible_to<VertexAttrArr>;
	};


	AXXEGRO_DEFINE_DELETER(ALLEGRO_VERTEX_DECL, al_destroy_vertex_decl);
	class VertexDecl: public Resource<ALLEGRO_VERTEX_DECL> {
	public:
		VertexDecl(const ALLEGRO_VERTEX_ELEMENT* elements, int stride)
			: Resource(al_create_vertex_decl(elements, stride))
		{
			if(ptr() == nullptr) {
				throw VertexDeclError("Cannot create vertex declaration. Check Allegro's log for details.");
			}
		}

		const ALLEGRO_VERTEX_DECL* get() 
		{
			return ptr();
		}
	};


	namespace detail {
		template<VertexType VertexT>
		struct VertexDeclGetter<VertexT> {
			static inline ALLEGRO_VERTEX_DECL* GetVertexDeclPtr() {
				static constexpr VertexAttrArr attrs = VertexAttrArrGetter<VertexT>::GetElements();
				static VertexDecl vDecl(attrs.data(), sizeof(VertexT));
				return vDecl.ptr();
			}
		};
	}

	struct Vertex {
		Vec3f pos;
		Vec2f uvPx;
		Color color;
	};

}

#endif /* INCLUDE_AXXEGRO_PRIM_CUSTOMVERTEX */
