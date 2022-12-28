#define AXXEGRO_TRUSTED

#include <axxegro/prim/buffers.hpp>

al::VertexBuffer::VertexBuffer(const tcb::span<Vertex> vertices, int flags)
	: Resource<ALLEGRO_VERTEX_BUFFER>(
		al_create_vertex_buffer(nullptr, vertices.data(), vertices.size(), flags)
	  )
{

}

int al::VertexBuffer::size() const
{
	return al_get_vertex_buffer_size(ptr());
}

al::IndexBuffer::IndexBuffer(const tcb::span<int> indices, int flags)
	: Resource<ALLEGRO_INDEX_BUFFER>(
		al_create_index_buffer(sizeof(int), indices.data(), indices.size(), flags)
	)
{

}

int al::IndexBuffer::size() const
{
	return al_get_index_buffer_size(ptr());
}
