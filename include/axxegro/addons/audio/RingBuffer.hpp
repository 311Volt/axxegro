//
// Created by volt on 2023-07-13.
//

#ifndef AXXEGRO_RINGBUFFER_HPP
#define AXXEGRO_RINGBUFFER_HPP

#include <span>
#include <vector>

namespace al {

	template<typename T>
	class RingBuffer {
	public:

		using SizeT = std::span<T>::size_type;
		using DiffT = std::span<T>::difference_type;

		explicit RingBuffer(SizeT capacity) {
			data.resize(capacity+1);
		}

		[[nodiscard]] SizeT capacity() const {
			return data.size() - 1;
		}

		[[nodiscard]] SizeT freeSpace() {
			return capacity() - size();
		}

		[[nodiscard]] SizeT size() const {
			return (internalBufSize() + (head-tail)) % internalBufSize();
		}

		[[nodiscard]] DiffT internalBufSize() const {
			return std::ssize(data);
		}

		bool pushData(const std::span<T> elements) {
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
			std::span<T> dspan(data);
			DiffT surplus = (begin + size) - internalBufSize();
			if(surplus > 0) {
				return {dspan.subspan(begin), dspan.subspan(0, surplus)};
			} else {
				return {dspan.subspan(begin, size), dspan.subspan(0, 0)};
			}
		}

		std::pair<std::span<T>, std::span<T>> partition(std::span<T> original, DiffT size) const {
			return {original.subspan(0, size), original.subspan(size)};
		}

		DiffT head = 0; //index into one past the last element
		DiffT tail = 0; //index into the first element
		std::vector<T> data;
	};

}

#endif //AXXEGRO_RINGBUFFER_HPP

