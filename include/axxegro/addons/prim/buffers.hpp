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

	template<typename T>
	concept IndexType = std::disjunction_v<
		std::is_same<T, uint16_t>,
		std::is_same<T, uint32_t>,
		std::is_same<T, int16_t>,
		std::is_same<T, int32_t>
	>;

	namespace detail {

		template<typename TPAPI>
		concept HardwareBufferAPI = requires(typename TPAPI::AllegBufType* bufPtr) {
			typename TPAPI::AllegBufType;
			typename TPAPI::ExceptionT;
			{TPAPI::template CreateFn<typename TPAPI::ExampleElementT>({}, 0)}
				-> std::same_as<typename TPAPI::AllegBufType*>;
			{TPAPI::DeleteFn(bufPtr)};
			{TPAPI::LockFn(bufPtr, 0, 0, 0)};
			{TPAPI::UnlockFn(bufPtr)};
			{TPAPI::SizeQueryFn(bufPtr)};
			{TPAPI::BufferTypeStr} -> std::convertible_to<const char*>;
		};
		template<HardwareBufferAPI TPAPI>
		struct HardwareBufferDeleter {
			void operator()(typename TPAPI::AllegBufType* ptr){
				TPAPI::DeleteFn(ptr);
			}
		};

		template<HardwareBufferAPI TPAPI, typename ElementT>
		class HardwareBufferLockedData;

		template<HardwareBufferAPI TPAPI, typename ElementT>
		class HardwareBuffer:
			RequiresInitializables<PrimitivesAddon>,
			public Resource<typename TPAPI::AllegBufType, HardwareBufferDeleter<TPAPI>>
		{
		public:
			using AllegBufT = typename TPAPI::AllegBufType;

			static constexpr int SuspectResourceExhaustionThreshold = 8'000'000;

			explicit HardwareBuffer(const std::span<ElementT> elements, int flags = ALLEGRO_PRIM_BUFFER_STATIC)
				: Resource<AllegBufT, HardwareBufferDeleter<TPAPI>>(nullptr)

			{
				if(auto* p = TPAPI::template CreateFn<ElementT>(elements, flags)) {
					this->setPtr(p);
				} else {
					throw typename TPAPI::ExceptionT(
						"Cannot create %s buffer of size %u.%s",
						TPAPI::BufferTypeStr,
						unsigned(elements.size()),
						elements.size() >= SuspectResourceExhaustionThreshold
						? ""
						: " Hardware buffers may not be supported on this platform."
					);
				}
			}

			[[nodiscard]] int size() const {
				return TPAPI::SizeQueryFn(this->ptr());
			}

			HardwareBufferLockedData<TPAPI, ElementT> lock(int start = 0, int len = -1, int flags = ALLEGRO_LOCK_WRITEONLY);

		};


		template<HardwareBufferAPI TPAPI, typename ElementT>
		class HardwareBufferLockedData {
		public:

			using AllegBufT = typename TPAPI::AllegBufType;

			HardwareBufferLockedData(const HardwareBufferLockedData&) = delete;
			HardwareBufferLockedData& operator=(const HardwareBufferLockedData&) = delete;
			HardwareBufferLockedData(HardwareBufferLockedData&&) = delete;
			HardwareBufferLockedData& operator=(HardwareBufferLockedData&&) = delete;

			std::span<ElementT> data() {
				return data_;
			}

			~HardwareBufferLockedData() {
				TPAPI::UnlockFn(buf_);
			}

		private:
			std::span<ElementT> data_;

			friend class HardwareBuffer<TPAPI, ElementT>;
			AllegBufT* buf_;

			using ExceptionT = typename TPAPI::ExceptionT;

			HardwareBufferLockedData(AllegBufT* buf, int start, int len, int flags)
			: buf_(buf)
			{
				int bufsize = TPAPI::SizeQueryFn(buf);

				if(len < 0) {
					len = bufsize - start;
				}
				int end = start+len;
				const char* bufType = std::same_as<AllegBufT, ALLEGRO_VERTEX_BUFFER> ? "vertex" : "index";

				if(start < 0 || start >= bufsize || end < 0 || end > bufsize || start >= end) {
					throw ExceptionT(
						"Cannot lock %s buffer of size %d: invalid range given: %d-%d",
						bufType, bufsize, start, end
					);
				}

				void* dat = TPAPI::LockFn(buf, start, len, flags);
				if(dat == nullptr) {
					throw ExceptionT(
						"Cannot lock %s buffer range [%d-%d) out of %d elements %s",
						bufType, start, end, bufsize,
						flags == ALLEGRO_LOCK_WRITEONLY ? "" : "(buffer reading may not be supported on this platform)"
					);
				}

				auto* vertices = static_cast<ElementT*>(dat);
				data_ = std::span {vertices, vertices + len};
			}
		};

		template<HardwareBufferAPI TPAPI, typename ElementT>
		inline HardwareBufferLockedData<TPAPI, ElementT> HardwareBuffer<TPAPI, ElementT>::lock(int start, int len, int flags) {
			return HardwareBufferLockedData<TPAPI, ElementT>(this->ptr(), start, len, flags);
		}






		struct IndexBufferAPI {
			using AllegBufType = ALLEGRO_INDEX_BUFFER;
			using ExceptionT = IndexBufferError;
			using ExampleElementT = int;
			static inline auto&& DeleteFn = al_destroy_index_buffer;
			static inline auto&& LockFn = al_lock_index_buffer;
			static inline auto&& UnlockFn = al_unlock_index_buffer;
			static inline auto&& SizeQueryFn = al_get_index_buffer_size;
			static constexpr char BufferTypeStr[] = "index buffer";

			template<IndexType ElementT>
			static inline AllegBufType* CreateFn(std::span<ElementT> elements, int flags) {
				return al_create_index_buffer(sizeof(ElementT), elements.data(), elements.size(), flags);
			}
		};
		static_assert(HardwareBufferAPI<IndexBufferAPI>);

		template<std::ranges::contiguous_range R>
			requires IndexType<std::ranges::range_value_t<R>>
		HardwareBuffer(R&& range, int flags) -> HardwareBuffer<IndexBufferAPI, std::ranges::range_value_t<R>>;


		struct VertexBufferAPI {

			using AllegBufType = ALLEGRO_VERTEX_BUFFER;
			using ExceptionT = VertexBufferError;
			using ExampleElementT = Vertex;
			static inline auto&& DeleteFn = al_destroy_vertex_buffer;
			static inline auto&& LockFn = al_lock_vertex_buffer;
			static inline auto&& UnlockFn = al_unlock_vertex_buffer;
			static inline auto&& SizeQueryFn = al_get_vertex_buffer_size;
			static constexpr char BufferTypeStr[] = "vertex buffer";

			template<VertexType ElementT>
			static inline AllegBufType* CreateFn(std::span<ElementT> elements, int flags) {
				return al_create_vertex_buffer(
					detail::VertexDeclGetter<ElementT>::GetVertexDeclPtr(),
					elements.data(), elements.size(), flags
				);
			}
		};
		static_assert(HardwareBufferAPI<VertexBufferAPI>);
	};


	template<VertexType VertexT>
	class VertexBuffer: public detail::HardwareBuffer<detail::VertexBufferAPI, VertexT> {
		using detail::HardwareBuffer<detail::VertexBufferAPI, VertexT>::HardwareBuffer;
	};

	template<std::ranges::contiguous_range R> requires VertexType<std::ranges::range_value_t<R>>
	VertexBuffer(R&& range, int flags) -> VertexBuffer<std::ranges::range_value_t<R>>;
	template<std::ranges::contiguous_range R> requires VertexType<std::ranges::range_value_t<R>>
	VertexBuffer(R&& range) -> VertexBuffer<std::ranges::range_value_t<R>>;



	template<IndexType IndexT>
	class IndexBuffer: public detail::HardwareBuffer<detail::IndexBufferAPI, IndexT> {
		using detail::HardwareBuffer<detail::IndexBufferAPI, IndexT>::HardwareBuffer;
	};

	template<std::ranges::contiguous_range R> requires IndexType<std::ranges::range_value_t<R>>
	IndexBuffer(R&& range, int flags) -> IndexBuffer<std::ranges::range_value_t<R>>;
	template<std::ranges::contiguous_range R> requires IndexType<std::ranges::range_value_t<R>>
	IndexBuffer(R&& range) -> IndexBuffer<std::ranges::range_value_t<R>>;


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

	template<VertexType VertexT, IndexType IndexT>
	inline int DrawIndexedBuffer(
		const VertexBuffer<VertexT>& vBuf,
		const IndexBuffer<IndexT>& iBuf,
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
