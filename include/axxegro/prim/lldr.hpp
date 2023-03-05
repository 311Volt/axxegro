#ifndef INCLUDE_AXXEGRO_PRIM_LLDR
#define INCLUDE_AXXEGRO_PRIM_LLDR

#include "axxegro/resources/Bitmap.hpp"
#include "axxegro/resources/Resource.hpp"
#include <axxegro/math/math.hpp>
#include <axxegro/Color.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <span>
#include <allegro5/allegro_primitives.h>

#include "CustomVertex.hpp"


namespace al {

	//TODO consider replacing these with a plain old pointer
	template<typename T>
	using OptionalRef = std::optional<std::reference_wrapper<T>>;
	
	class Vertex: public ALLEGRO_VERTEX {
	public:
		Vertex(const Vec3<> pos = {0,0,0}, const Vec2<> uv = {0,0}, Color color = al::White) : ALLEGRO_VERTEX() {
			setPos(pos);
			setUV(uv);
			setColor(color);
		}

		void setPos(const Vec3<> pos) {
			x = pos.x;
			y = pos.y;
			z = pos.z;
		}
		void setUV(const Vec2<> uv) {
			u = uv.x;
			v = uv.y;
		}
		void setColor(const Color color) {
			this->color = color;
		}

		[[nodiscard]] Vec3<> getPos() const {
			return {x,y,z};
		}
		[[nodiscard]] Vec2<> getUV() const {
			return {u,v};
		}
		[[nodiscard]] Color getColor() const {
			return this->color;
		}
	};

	

	inline int DrawPrim(
		const std::span<Vertex> vertices, 
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
		const std::span<Vertex> vertices, 
		const std::span<int> indices, 
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
		const std::span<VType> vertices,
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
		const std::span<VType> vertices,
		const std::span<int> indices,
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
