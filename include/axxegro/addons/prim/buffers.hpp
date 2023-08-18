#ifndef INCLUDE_AXXEGRO_PRIM_BUFFERS
#define INCLUDE_AXXEGRO_PRIM_BUFFERS

#include "PrimitivesAddon.hpp"
#include "lldr.hpp"
#include "Vertex.hpp"
#include "common.hpp"

#include <span>


namespace al {
	
	AXXEGRO_DEFINE_DELETER(ALLEGRO_VERTEX_BUFFER, al_destroy_vertex_buffer);
	AXXEGRO_DEFINE_DELETER(ALLEGRO_INDEX_BUFFER, al_destroy_index_buffer);

	template<VertexType VertexT>
	class VertexBufferLockedData;

	//class IndexBufferLockedData;

	template<VertexType VertexT>
	class VertexBuffer:
			RequiresInitializables<PrimitivesAddon>,
			public Resource<ALLEGRO_VERTEX_BUFFER> {
	public:
		explicit VertexBuffer(const std::span<VertexT> vertices, int flags = ALLEGRO_PRIM_BUFFER_STATIC)
				: Resource<ALLEGRO_VERTEX_BUFFER>(
				al_create_vertex_buffer(
					detail::VertexDeclGetter<VertexT>::GetVertexDeclPtr(),
					vertices.data(), (int)vertices.size(), flags
				)
		)
		{
			if(ptr() == nullptr) {
				throw VertexBufferError("Cannot create vertex buffer of size %u. ", (unsigned)vertices.size());
			}
		}

		[[nodiscard]] int size() const {
			return al_get_vertex_buffer_size(ptr());
		}
		//TODO template ctor for different vertex types

		VertexBufferLockedData<VertexT> lock(int start = 0, int len = -1, int flags = ALLEGRO_LOCK_WRITEONLY);
	};

	template<VertexType VertexT>
	class VertexBufferLockedData {
	public:

		VertexBufferLockedData(const VertexBufferLockedData&) = delete;
		VertexBufferLockedData& operator=(const VertexBufferLockedData&) = delete;
		VertexBufferLockedData(VertexBufferLockedData&&) = delete;
		VertexBufferLockedData& operator=(VertexBufferLockedData&&) = delete;

		std::span<VertexT> data() {
			return data_;
		}

		~VertexBufferLockedData() {
			al_unlock_vertex_buffer(vb_);
		}
	private:
		std::span<VertexT> data_;

		friend class VertexBuffer<VertexT>;
		ALLEGRO_VERTEX_BUFFER* vb_;
		VertexBufferLockedData(ALLEGRO_VERTEX_BUFFER* vb, int start, int len, int flags) 
			: vb_(vb)
		{
			int vbsize = al_get_vertex_buffer_size(vb);
			
			if(len < 0) {
				len = vbsize - start;
			}

			void* dat = al_lock_vertex_buffer(vb, start, len, flags);
			if(dat == nullptr) {
				throw al::VertexBufferError("Cannot lock vertex buffer vtxs [%d-%d) out of %d", start, start+len, vbsize);
			}

			VertexT* vertices = static_cast<VertexT*>(dat);
			data_ = std::span {vertices, vertices + len};
		}
	};

	template<VertexType VertexT>
	inline VertexBufferLockedData<VertexT> VertexBuffer<VertexT>::lock(int start, int len, int flags) {
		return VertexBufferLockedData(ptr(), start, len, flags);
	}

	class IndexBuffer:
			RequiresInitializables<PrimitivesAddon>,
			public Resource<ALLEGRO_INDEX_BUFFER> {
	public:
		explicit IndexBuffer(const std::span<int> indices, int flags = ALLEGRO_PRIM_BUFFER_STATIC)
				: Resource<ALLEGRO_INDEX_BUFFER>(
				al_create_index_buffer(sizeof(int), indices.data(), (int)indices.size(), flags)
		)
		{
			if(ptr() == nullptr) {
				throw IndexBufferError("Cannot create index buffer of size %u. ", (unsigned)indices.size());
			}
		}

		[[nodiscard]] int size() const {
			return al_get_index_buffer_size(ptr());
		}
		//TODO template ctor for different index sizes
	};

	template<VertexType VertexT>
	inline int DrawVertexBuffer(
		const VertexBuffer<VertexT>& vBuf,
		OptionalRef<Bitmap> texture = std::nullopt,
		int start=0, int end=-1, 
		int type=ALLEGRO_PRIM_TRIANGLE_LIST
	)
	{
		if(end < 0) {
			end = vBuf.size();
		}
		return al_draw_vertex_buffer(
			vBuf.ptr(),
			texture ? texture->get().ptr() : nullptr,
			start, end, type
		); 
	}

	template<VertexType VertexT>
	inline int DrawIndexedBuffer(
		const VertexBuffer<VertexT>& vBuf,
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
			vBuf.ptr(),
			texture ? texture->get().ptr() : nullptr,
			iBuf.ptr(),
			start, end, type
		);
	}

}

#endif /* INCLUDE_AXXEGRO_PRIM_BUFFERS */
