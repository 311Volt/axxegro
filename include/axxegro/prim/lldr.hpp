#ifndef INCLUDE_AXXEGRO_PRIM_LLDR
#define INCLUDE_AXXEGRO_PRIM_LLDR

#include "axxegro/resources/Bitmap.hpp"
#include "axxegro/resources/Resource.hpp"
#include <axxegro/math/math.hpp>
#include <axxegro/Color.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <tcb/span.hpp>
#include <allegro5/allegro_primitives.h>

#include "CustomVertex.hpp"


namespace al {
	template<typename T>
	using OptionalRef = std::optional<std::reference_wrapper<T>>;
	
	class Vertex: public ALLEGRO_VERTEX {
	public:
		Vertex(const Vec3<> pos, const Vec2<> uv = {0,0}, Color color = al::White);

		void setPos(const Vec3<> pos);
		void setUV(const Vec2<> uv);
		void setColor(const Color color);

		Vec3<> getPos() const;
		Vec2<> getUV() const;
		Color getColor() const;
	};

	

	inline void DrawPrim(
		const tcb::span<Vertex> vertices, 
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
		int start = 0,
		int end = -1
	)
	{
		al_draw_prim(
			vertices.data(), 
			nullptr, 
			texture ? texture->get().constPtr() : nullptr, 
			start, 
			end==-1 ? vertices.size() : end, 
			type
		);
	}

	inline void DrawIndexedPrim(
		const tcb::span<Vertex> vertices, 
		const tcb::span<int> indices, 
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		al_draw_indexed_prim(
			vertices.data(), 
			nullptr, 
			texture ? texture->get().constPtr() : nullptr, 
			indices.data(), 
			vertices.size(), 
			type
		);
	}

	template<typename VDecl>
	void DrawPrim(
		const tcb::span<typename VDecl::VertexT> vertices,
		const OptionalRef<Bitmap> texture = std::nullopt,
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
		int start = 0,
		int end = -1
	)
	{
		static VertexDecl vd = VertexDecl::Init<VDecl>();
		al_draw_prim(
			vertices.data(), 
			vd.ptr(), 
			texture ? texture->get().constPtr() : nullptr, 
			start, 
			end==-1 ? vertices.size() : end, 
			type
		);
	}

	template<typename VDecl>
	void DrawIndexedPrim(
		const tcb::span<typename VDecl::VertexT> vertices,
		const tcb::span<int> indices,
		const OptionalRef<Bitmap> texture = std::nullopt,
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		static VertexDecl vd = VertexDecl::Init<VDecl>();
		al_draw_indexed_prim(
			vertices.data(), 
			vd.ptr(), 
			texture ? texture->get().constPtr() : nullptr, 
			indices.data(), 
			vertices.size(), 
			type
		);
	}

	AXXEGRO_DEFINE_DELETER(ALLEGRO_VERTEX_BUFFER, al_destroy_vertex_buffer);
	AXXEGRO_DEFINE_DELETER(ALLEGRO_INDEX_BUFFER, al_destroy_index_buffer);

	class IndexBuffer: Resource<ALLEGRO_INDEX_BUFFER> {
	public:
		IndexBuffer(const tcb::span<int> indices, int flags = ALLEGRO_PRIM_BUFFER_STATIC);
	};

	class VertexBuffer: Resource<ALLEGRO_VERTEX_BUFFER> {
	public:
		VertexBuffer(const tcb::span<Vertex> vertices, int flags = ALLEGRO_PRIM_BUFFER_STATIC);

	};

}

#endif //INCLUDE_AXXEGRO_PRIM_LLDR