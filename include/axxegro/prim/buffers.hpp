#ifndef INCLUDE_AXXEGRO_PRIM_BUFFERS
#define INCLUDE_AXXEGRO_PRIM_BUFFERS

#include <span>
#include "lldr.hpp"
#include <allegro5/allegro_primitives.h>
#include "axxegro/resources/Resource.hpp"


namespace al {
	
	AXXEGRO_DEFINE_DELETER(ALLEGRO_VERTEX_BUFFER, al_destroy_vertex_buffer);
	AXXEGRO_DEFINE_DELETER(ALLEGRO_INDEX_BUFFER, al_destroy_index_buffer);

	class VertexBuffer: public Resource<ALLEGRO_VERTEX_BUFFER> {
	public:
		explicit VertexBuffer(const std::span<Vertex> vertices, int flags = ALLEGRO_PRIM_BUFFER_STATIC)
				: Resource<ALLEGRO_VERTEX_BUFFER>(
				al_create_vertex_buffer(nullptr, vertices.data(), (int)vertices.size(), flags)
		)
		{
			if(ptr() == nullptr) {
				throw VertexBufferError("Cannot create vertex buffer of size %u. ", vertices.size());
			}
		}

		[[nodiscard]] int size() const {
			return al_get_vertex_buffer_size(ptr());
		}
		//TODO template ctor for different vertex types
	};
	//TODO locking

	class IndexBuffer: public Resource<ALLEGRO_INDEX_BUFFER> {
	public:
		explicit IndexBuffer(const std::span<int> indices, int flags = ALLEGRO_PRIM_BUFFER_STATIC)
				: Resource<ALLEGRO_INDEX_BUFFER>(
				al_create_index_buffer(sizeof(int), indices.data(), (int)indices.size(), flags)
		)
		{
			if(ptr() == nullptr) {
				throw IndexBufferError("Cannot create index buffer of size %u. ", indices.size());
			}
		}

		[[nodiscard]] int size() const {
			return al_get_index_buffer_size(ptr());
		}
		//TODO template ctor for different index sizes
	};
	
	inline int DrawVertexBuffer(
		const VertexBuffer& vBuf, 
		OptionalRef<Bitmap> texture = std::nullopt,
		int start=0, int end=-1, 
		int type=ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		if(end < 0) {
			end = vBuf.size();
		}
		return al_draw_vertex_buffer(
			vBuf.constPtr(), 
			texture ? texture->get().constPtr() : nullptr, 
			start, end, type
		); 
	}

	inline int DrawIndexedBuffer(
		const VertexBuffer& vBuf, 
		const IndexBuffer& iBuf,
		OptionalRef<Bitmap> texture = std::nullopt,
		int start=0, int end=-1, 
		int type=ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		if(end < 0) {
			end = iBuf.size();
		}
		return al_draw_indexed_buffer(
			vBuf.constPtr(), 
			texture ? texture->get().constPtr() : nullptr, 
			iBuf.constPtr(), 
			start, end, type
		);
	}

}

#endif /* INCLUDE_AXXEGRO_PRIM_BUFFERS */
