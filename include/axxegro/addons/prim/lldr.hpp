#ifndef INCLUDE_AXXEGRO_PRIM_LLDR
#define INCLUDE_AXXEGRO_PRIM_LLDR

#include "common.hpp"
#include "Vertex.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <ranges>
#include <span>



namespace al {

	//TODO consider replacing these with a plain old pointer
	template<typename T>
	using OptionalRef = std::optional<std::reference_wrapper<T>>;

	template<std::ranges::contiguous_range RangeT>
		requires VertexType<std::ranges::range_value_t<RangeT>>
	inline int DrawPrim(
		const RangeT vertices,
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
		int start = 0,
		int end = -1
	)
	{
		using VertexT = std::ranges::range_value_t<RangeT>;
		InternalRequire<PrimitivesAddon>();
		return al_draw_prim(
			std::ranges::data(vertices),
			detail::VertexDeclGetter<VertexT>::GetVertexDeclPtr(),
			texture ? texture->get().ptr() : nullptr,
			start, 
			end==-1 ? std::ranges::size(vertices) : end,
			type
		);
	}

	template<std::ranges::contiguous_range VtxRangeT, std::ranges::contiguous_range IdxRangeT>
		requires VertexType<std::ranges::range_value_t<VtxRangeT>>
		      && std::same_as<std::ranges::range_value_t<IdxRangeT>, int>
	inline int DrawIndexedPrim(
		const VtxRangeT vertices,
		const IdxRangeT indices,
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		using VertexT = std::ranges::range_value_t<VtxRangeT>;
		InternalRequire<PrimitivesAddon>();
		return al_draw_indexed_prim(
			std::ranges::data(vertices),
			detail::VertexDeclGetter<VertexT>::GetVertexDeclPtr(),
			texture ? texture->get().ptr() : nullptr,
			std::ranges::data(indices),
			std::ranges::size(indices),
			type
		);
	}

}

#endif //INCLUDE_AXXEGRO_PRIM_LLDR
