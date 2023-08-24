//
// Created by volt on 2023-07-13.
//

#ifndef AXXEGRO_RINGBUFFER_HPP
#define AXXEGRO_RINGBUFFER_HPP

#include <span>
#include <vector>

#include "../../com/util/General.hpp"

namespace al {

	/**
	 * @brief A simple FIFO ring buffer. Provides utility for audio code.
	 * @tparam T The buffer's value type. Must be copyable.
	 */
	template<typename T>
	class RingBuffer {
	public:

		using SizeT = typename std::span<T>::size_type;
		using DiffT = typename std::span<T>::difference_type;

		/**
		 * Creates a ring buffer with the given capacity
		 * @param capacity Maximum number of elements that can be held by the buffer.
		 */
		explicit RingBuffer(SizeT capacity) {
			data.resize(capacity+1);
		}

		/// @brief Returns the maximum number of elements that can be held by the buffer.
		[[nodiscard]] SizeT capacity() const {
			return data.size() - 1;
		}

		/// @brief Returns the maximum number of elements that can be pushed into the buffer currently.
		[[nodiscard]] SizeT freeSpace() const {
			return capacity() - size();
		}

		/// @brief Returns the number of elements currently held by the buffer.
		[[nodiscard]] SizeT size() const {
			return (internalBufSize() + (head-tail)) % internalBufSize();
		}

		/// @brief Returns the size of the internal buffer.
		[[nodiscard]] DiffT internalBufSize() const {
			return std::ssize(data);
		}

		/**
		 * @brief Inserts the specified range of elements into the buffer in a FIFO manner.
		 * @param elements Elements to be inserted into the buffer.
		 * @return Whether there was enough space to perform the operation.
		 */
		bool pushData(const std::span<const T> elements) {
			if(freeSpace() < elements.size()) {
				return false;
			}

			auto [dpart1, dpart2] = makeRanges(head, elements.size());
			const auto [spart1, spart2] = partition(elements, dpart1.size());

			std::copy(spart1.begin(), spart1.end(), dpart1.begin());
			std::copy(spart2.begin(), spart2.end(), dpart2.begin());

			head = (head + elements.size()) % internalBufSize();

			return true;
		}

		/**
		 * @brief Copies the buffer's oldest elements into the specified range.
		 * @param output Output data. Specifies both the destination and the requested amount of data.
		 * @return Whether there was enough data in the buffer to perform the operation.
		 */
		bool popInto(std::span<T> output) {
			if(size() < output.size()) {
				return false;
			}

			const auto [spart1, spart2] = makeRanges(tail, output.size());
			auto [dpart1, dpart2] = partition(output, spart1.size());

			std::copy(spart1.begin(), spart1.end(), dpart1.begin());
			std::copy(spart2.begin(), spart2.end(), dpart2.begin());

			tail = (tail + output.size()) % internalBufSize();

			return true;
		}



	private:

		std::pair<std::span<T>, std::span<T>> makeRanges(DiffT begin, DiffT size) {
			std::span bufspan(data);
			return {
				Subspan(bufspan, begin, size),
				Subspan(bufspan, begin - internalBufSize(), size)
			};
		}

		std::pair<std::span<T>, std::span<T>> partition(std::span<T> original, DiffT size) const {
			return {original.subspan(0, size), original.subspan(size)};
		}
		std::pair<std::span<const T>, std::span<const T>> partition(const std::span<const T> original, DiffT size) const {
			return {original.subspan(0, size), original.subspan(size)};
		}

		DiffT head = 0; //index into one past the last element
		DiffT tail = 0; //index into the first element
		std::vector<T> data;
	};

}

#endif //AXXEGRO_RINGBUFFER_HPP

