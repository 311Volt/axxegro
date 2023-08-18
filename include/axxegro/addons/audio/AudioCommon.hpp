#ifndef AXXEGRO_AUDIOCOMMON_HPP
#define AXXEGRO_AUDIOCOMMON_HPP

#include "../../core.hpp"
#include "AudioAddon.hpp"

#include <allegro5/allegro_audio.h>

#include <span>
#include <string>

namespace al
{

	constexpr size_t GetChannelCount(ALLEGRO_CHANNEL_CONF conf)
	{
		return (conf>>4) + (conf&0xF);
	}

	constexpr size_t GetAudioDepthBytes(ALLEGRO_AUDIO_DEPTH depth)
	{
		return (depth & 0x07) + 1;
	}

	struct BufferConfig {
		unsigned numChunks;
		unsigned fragmentsPerChunk;
	};

	struct PlaybackParams {
		float gain = 1.0;
		float pan = 0.0;
		float speed = 1.0;
		ALLEGRO_PLAYMODE loop = ALLEGRO_PLAYMODE_ONCE;

		[[nodiscard]] std::string str() const
		{
			std::string loopStr = [](ALLEGRO_PLAYMODE mode){switch(mode){
				case ALLEGRO_PLAYMODE_ONCE: return "once";
				case ALLEGRO_PLAYMODE_LOOP: return "loop-once";
				case ALLEGRO_PLAYMODE_BIDIR: return "bidir";
				case ALLEGRO_PLAYMODE_LOOP_ONCE: return "loop-once";
				default: return "invalid";
			}}(loop);

			return Format("gain=%.3f, pan=%.3f, speed=%3f, playmode=%s", gain, pan, speed, loopStr.c_str());
		}
	};

	struct AudioFragmentFormat {
		ALLEGRO_AUDIO_DEPTH depth = ALLEGRO_AUDIO_DEPTH_FLOAT32;
		ALLEGRO_CHANNEL_CONF chanConf = ALLEGRO_CHANNEL_CONF_2;

		[[nodiscard]] std::string str() const
		{
			using namespace std::string_literals;

			std::string depthStr = [](ALLEGRO_AUDIO_DEPTH mode){switch(mode){
				case ALLEGRO_AUDIO_DEPTH_INT8: return "int8"s;
				case ALLEGRO_AUDIO_DEPTH_INT16: return "int16"s;
				case ALLEGRO_AUDIO_DEPTH_INT24: return "int24"s;
				case ALLEGRO_AUDIO_DEPTH_UINT8: return "uint8"s;
				case ALLEGRO_AUDIO_DEPTH_UINT16: return "uint16"s;
				case ALLEGRO_AUDIO_DEPTH_UINT24: return "uint24"s;
				case ALLEGRO_AUDIO_DEPTH_FLOAT32: return "float32"s;
				default: return "invalid"s;
			}}(depth);

			std::string chanStr = [](ALLEGRO_CHANNEL_CONF conf){switch(conf){
				case ALLEGRO_CHANNEL_CONF_1: return "mono"s;
				case ALLEGRO_CHANNEL_CONF_2: return "stereo"s;
				case ALLEGRO_CHANNEL_CONF_3: return "3 channels"s;
				case ALLEGRO_CHANNEL_CONF_4: return "4 channels"s;
				default: return Format("%d.1", (int)al::GetChannelCount(conf)-1);
			}}(chanConf);

			return Format("%s, %s", depthStr.c_str(), chanStr.c_str());
		}
	};


	struct AudioFormat {
		unsigned frequency = 44100;
		ALLEGRO_AUDIO_DEPTH depth = ALLEGRO_AUDIO_DEPTH_FLOAT32;
		ALLEGRO_CHANNEL_CONF chanConf = ALLEGRO_CHANNEL_CONF_2;

		[[nodiscard]] std::string str() const
		{
			return Format("%d Hz, %s", frequency, AudioFragmentFormat{.depth=depth, .chanConf=chanConf}.str().c_str());
		}
	};

	namespace detail {

		template<typename Derived>
		struct AddPlaybackParamsQuery
		{
			PlaybackParams getPlaybackParams() {
				auto& self = static_cast<Derived&>(*this);
				return {
					.gain = self.getGain(),
					.pan = self.getPan(),
					.speed = self.getSpeed(),
					.loop = self.getPlayMode(),
				};
			}

			bool setPlaybackParams(PlaybackParams params) {
				auto& self = static_cast<Derived&>(*this);
				if(!self.setGain(params.gain)) return false;
				if(!self.setPan(params.pan)) return false;
				if(!self.setSpeed(params.speed)) return false;
				if(!self.setPlayMode(params.loop)) return false;
				return true;
			}
		};

		template<typename Derived>
		struct AddAudioFormatQuery
		{
			AudioFormat getAudioFormat() {
				auto& self = static_cast<Derived&>(*this);
				return {
					.frequency = self.getFrequency(),
					.depth = self.getDepth(),
					.chanConf = self.getChannelConf()
				};
			}
		};

		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioSampleType {
			using Type=void;
		};
		template<>
		struct AudioSampleType<ALLEGRO_AUDIO_DEPTH_INT8> {
			using Type = int8_t;
		};
		template<>
		struct AudioSampleType<ALLEGRO_AUDIO_DEPTH_INT16> {
			using Type = int16_t;
		};
		template<>
		struct AudioSampleType<ALLEGRO_AUDIO_DEPTH_UINT8> {
			using Type = uint8_t;
		};
		template<>
		struct AudioSampleType<ALLEGRO_AUDIO_DEPTH_UINT16> {
			using Type = uint16_t;
		};
		template<>
		struct AudioSampleType<ALLEGRO_AUDIO_DEPTH_FLOAT32> {
			using Type = float;
		};
		template<ALLEGRO_CHANNEL_CONF ChanConf, ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType {
			using Type = void;
		};


		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_1, AudioDepth> {
			using Type = typename AudioSampleType<AudioDepth>::Type;
		};
		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_2, AudioDepth> {
			using Type = Vec<typename AudioSampleType<AudioDepth>::Type, 2>;
		};
		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_3, AudioDepth> {
			using Type = Vec<typename AudioSampleType<AudioDepth>::Type, 3>;
		};
		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_4, AudioDepth> {
			using Type = Vec<typename AudioSampleType<AudioDepth>::Type, 4>;
		};
		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_5_1, AudioDepth> {
			using Type = Vec<typename AudioSampleType<AudioDepth>::Type, 6>;
		};
		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_6_1, AudioDepth> {
			using Type = Vec<typename AudioSampleType<AudioDepth>::Type, 7>;
		};
		template<ALLEGRO_AUDIO_DEPTH AudioDepth>
		struct AudioFragmentType<ALLEGRO_CHANNEL_CONF_7_1, AudioDepth> {
			using Type = Vec<typename AudioSampleType<AudioDepth>::Type, 8>;
		};

		template<int N>
		concept ValidChannelCount = N > 0 && N <= 9 && N != 5;

		template<typename T>
		concept ValidSampleType = std::disjunction_v<
				std::is_same<T, int8_t>,
				std::is_same<T, int16_t>,
				std::is_same<T, uint8_t>,
				std::is_same<T, uint16_t>,
				std::is_same<T, float>>;

		template<typename T>
		concept ValidMultiChannelFragmentType = detail::ValidSampleType<typename T::ElementType> && requires {
			{T::NumElements} -> std::convertible_to<int>;
		};

		template<typename T>
		concept ValidFragmentType = detail::ValidSampleType<T> || detail::ValidMultiChannelFragmentType<T>;

		template<al::detail::ValidSampleType T>
		inline constexpr ALLEGRO_AUDIO_DEPTH AudioDepthOf = ALLEGRO_AUDIO_DEPTH_INT8;

		template<> inline constexpr ALLEGRO_AUDIO_DEPTH AudioDepthOf<int8_t> = ALLEGRO_AUDIO_DEPTH_INT8;
		template<> inline constexpr ALLEGRO_AUDIO_DEPTH AudioDepthOf<uint8_t> = ALLEGRO_AUDIO_DEPTH_UINT8;
		template<> inline constexpr ALLEGRO_AUDIO_DEPTH AudioDepthOf<int16_t> = ALLEGRO_AUDIO_DEPTH_INT16;
		template<> inline constexpr ALLEGRO_AUDIO_DEPTH AudioDepthOf<uint16_t> = ALLEGRO_AUDIO_DEPTH_UINT16;
		template<> inline constexpr ALLEGRO_AUDIO_DEPTH AudioDepthOf<float> = ALLEGRO_AUDIO_DEPTH_FLOAT32;

		template<al::detail::ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
		struct FragmentTraits {
			static constexpr ALLEGRO_AUDIO_DEPTH Depth = AudioDepthOf<TSample>;
			static constexpr ALLEGRO_CHANNEL_CONF ChanConf = TPChanConf;
			static constexpr int NumChannels = GetChannelCount(ChanConf);
			using SampleType = TSample;
			using FragmentType = typename AudioFragmentType<TPChanConf, Depth>::Type;
		};

		template<detail::ValidMultiChannelFragmentType TFrag, al::detail::ValidSampleType TNewSmp>
		using ConvertFragSampleType = Vec<TNewSmp, TFrag::NumElements>;
	}



	inline constexpr ALLEGRO_CHANNEL_CONF Mono = ALLEGRO_CHANNEL_CONF_1;
	inline constexpr ALLEGRO_CHANNEL_CONF Stereo = ALLEGRO_CHANNEL_CONF_2;
}

#endif //AXXEGRO_AUDIOCOMMON_HPP
