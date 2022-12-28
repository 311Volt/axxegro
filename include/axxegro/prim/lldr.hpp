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

	//TODO consider replacing these with a plain old pointer
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

	

	inline int DrawPrim(
		const tcb::span<Vertex> vertices, 
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
		int start = 0,
		int end = -1
	)
	{
		return al_draw_prim(
			vertices.data(), 
			nullptr, 
			texture ? texture->get().constPtr() : nullptr, 
			start, 
			end==-1 ? vertices.size() : end, 
			type
		);
	}

	inline int DrawIndexedPrim(
		const tcb::span<Vertex> vertices, 
		const tcb::span<int> indices, 
		const OptionalRef<Bitmap> texture = std::nullopt, 
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		return al_draw_indexed_prim(
			vertices.data(), 
			nullptr, 
			texture ? texture->get().constPtr() : nullptr, 
			indices.data(), 
			indices.size(), 
			type
		);
	}

	template<typename VType>
	void DrawPrim(
		const tcb::span<VType> vertices,
		const OptionalRef<Bitmap> texture = std::nullopt,
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST,
		int start = 0,
		int end = -1
	)
	{
		static VertexDecl vd = VertexDecl::Init<VertexDeclFor<VType>>();
		al_draw_prim(
			vertices.data(), 
			vd.ptr(), 
			texture ? texture->get().constPtr() : nullptr, 
			start, 
			end==-1 ? vertices.size() : end, 
			type
		);
	}

	template<typename VType>
	void DrawIndexedPrim(
		const tcb::span<VType> vertices,
		const tcb::span<int> indices,
		const OptionalRef<Bitmap> texture = std::nullopt,
		ALLEGRO_PRIM_TYPE type = ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		static VertexDecl vd = VertexDecl::Init<VertexDeclFor<VType>>();
		al_draw_indexed_prim(
			vertices.data(), 
			vd.ptr(), 
			texture ? texture->get().constPtr() : nullptr, 
			indices.data(), 
			indices.size(), 
			type
		);
	}

	

}

#endif //INCLUDE_AXXEGRO_PRIM_LLDR
