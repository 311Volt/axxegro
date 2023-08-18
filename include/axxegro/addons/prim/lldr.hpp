#ifndef INCLUDE_AXXEGRO_PRIM_LLDR
#define INCLUDE_AXXEGRO_PRIM_LLDR

#include "common.hpp"
#include "Vertex.hpp"

#include <cstddef>
#include <memory>
#include <optional>
#include <span>



namespace al {

	//TODO consider replacing these with a plain old pointer
	template<typename T>
	using OptionalRef = std::optional<std::reference_wrapper<T>>;

	template<VertexType VertexT>
	inline int DrawPrim(
		const std::span<VertexT> vertices,
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
		int start = 0,
		int end = -1
	)
	{
		InternalRequire<PrimitivesAddon>();
		return al_draw_prim(
			vertices.data(), 
			detail::VertexDeclGetter<VertexT>::GetVertexDeclPtr(),
			texture ? texture->get().ptr() : nullptr,
			start, 
			end==-1 ? vertices.size() : end, 
			type
		);
	}

	template<VertexType VertexT>
	inline int DrawIndexedPrim(
		const std::span<VertexT> vertices,
		const std::span<int> indices, 
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		InternalRequire<PrimitivesAddon>();
		return al_draw_indexed_prim(
			vertices.data(),
			detail::VertexDeclGetter<VertexT>::GetVertexDeclPtr(),
			texture ? texture->get().ptr() : nullptr,
			indices.data(), 
			indices.size(), 
			type
		);
	}

}

#endif //INCLUDE_AXXEGRO_PRIM_LLDR
