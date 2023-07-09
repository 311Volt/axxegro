//
// Created by volt on 2023-07-09.
//

#ifndef AXXEGRO_AUDIOUTIL_HPP
#define AXXEGRO_AUDIOUTIL_HPP

#include "AudioCommon.hpp"

namespace al {

	template<ValidMultiChannelFragmentType TFrag, typename... TArgs>
	requires (
			(IndirectlyConvertibleTo<TArgs, std::span<typename TFrag::ElementType>> && ...) &&
			sizeof...(TArgs) == TFrag::NumElements
			)
	bool UnzipChannels(const std::span<TFrag> inputFragments, TArgs&&... outputChannels) {
		using SampleType = typename TFrag::ElementType;
		static constexpr int NumChannels = TFrag::NumElements;

		std::array<std::span<SampleType>, NumChannels> outSpans = {outputChannels...};
		for(const auto& outCh: outSpans) {
			if(outCh.size() != inputFragments.size()) {
				return false;
			}
		}

		for(unsigned i=0; i<inputFragments.size(); i++) {
			for(int ch=0; ch<NumChannels; ch++) {
				outSpans[ch][i] = inputFragments[i][ch];
			}
		}
		return true;
	}


	template<std::signed_integral T>
	inline float AsFloatSample(T val)
	{
		return static_cast<float>(val) / std::numeric_limits<T>::max();
	}

	template<std::unsigned_integral T>
	inline float AsFloatSample(T val)
	{
		return 2.0f * (static_cast<float>(val) / std::numeric_limits<T>::max() - 0.5f);
	}

	template<typename T> requires (ValidFragmentType<T> && std::integral<T>)
	bool ConvertFragmentsToFloat(const std::span<T> src, std::span<float> dst)
	{
		if(src.size() != dst.size()) {
			return false;
		}
		for(unsigned i=0; i<src.size(); i++) {
			dst[i] = AsFloatSample(src[i]);
		}
		return true;
	}

	template<ValidMultiChannelFragmentType TFrag>
	bool ConvertFragmentsToFloat(const std::span<TFrag> src, std::span<ConvertFragSampleType<TFrag, float>> dst) {
		static constexpr int NumChannels = TFrag::NumElements;
		if(src.size() != dst.size()) {
			return false;
		}
		for(unsigned i=0; i<src.size(); i++) {
			for(int j=0; j<NumChannels; j++) {
				dst[i][j] = AsFloatSample(src[i][j]);
			}
		}
		return true;
	}
}

#endif //AXXEGRO_AUDIOUTIL_HPP
