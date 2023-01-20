#include "axxegro/Exception.hpp"
#define AXXEGRO_TRUSTED

#include <axxegro/prim/buffers.hpp>

al::VertexBuffer::VertexBuffer(const std::span<Vertex> vertices, int flags)
	: Resource<ALLEGRO_VERTEX_BUFFER>(
		al_create_vertex_buffer(nullptr, vertices.data(), vertices.size(), flags)
	  )
{
	if(ptr() == nullptr) {
		throw VertexBufferError("Cannot create vertex buffer of size {}. ", vertices.size());
	}
}

int al::VertexBuffer::size() const
{
	return al_get_vertex_buffer_size(ptr());
}

al::IndexBuffer::IndexBuffer(const std::span<int> indices, int flags)
	: Resource<ALLEGRO_INDEX_BUFFER>(
		al_create_index_buffer(sizeof(int), indices.data(), indices.size(), flags)
	)
{
	if(ptr() == nullptr) {
		throw IndexBufferError("Cannot create index buffer of size {}. ", indices.size());
	}
}

int al::IndexBuffer::size() const
{
	return al_get_index_buffer_size(ptr());
}
