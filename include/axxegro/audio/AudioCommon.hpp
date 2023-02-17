#ifndef AXXEGRO_AUDIOCOMMON_HPP
#define AXXEGRO_AUDIOCOMMON_HPP

#include <axxegro/math/math.hpp>

#include <allegro5/allegro_audio.h>

#include <span>
#include <string>

namespace al
{
	struct PlaybackParams {
		float gain = 1.0;
		float pan = 0.0;
		float speed = 1.0;
		ALLEGRO_PLAYMODE loop = ALLEGRO_PLAYMODE_ONCE;

		[[nodiscard]] std::string str() const;
	};

	struct AudioFormat {
		unsigned frequency = 44100;
		ALLEGRO_AUDIO_DEPTH depth = ALLEGRO_AUDIO_DEPTH_FLOAT32;
		ALLEGRO_CHANNEL_CONF chanConf = ALLEGRO_CHANNEL_CONF_2;

		[[nodiscard]] std::string str() const;
	};

	template<typename Derived>
	struct AddPlaybackParams
	{
		PlaybackParams getPlaybackParams()
		{
			return {
				.gain = static_cast<Derived*>(this)->getGain(),
				.pan = static_cast<Derived*>(this)->getPan(),
				.speed = static_cast<Derived*>(this)->getSpeed(),
				.loop = static_cast<Derived*>(this)->getPlayMode(),
			};
		}

		bool setPlaybackParams(PlaybackParams params)
		{
			if(!static_cast<Derived*>(this)->setGain(params.gain)) return false;
			if(!static_cast<Derived*>(this)->setPan(params.pan)) return false;
			if(!static_cast<Derived*>(this)->setSpeed(params.speed)) return false;
			if(!static_cast<Derived*>(this)->setPlayMode(params.loop)) return false;
			return true;
		}
	};

	template<typename Derived>
	struct AddAudioFormatQuery
	{
		AudioFormat getAudioFormat()
		{
			return {
				.frequency = static_cast<Derived*>(this)->getFrequency(),
				.depth = static_cast<Derived*>(this)->getDepth(),
				.chanConf = static_cast<Derived*>(this)->getChannelConf()
			};
		}
	};

	constexpr size_t GetChannelCount(ALLEGRO_CHANNEL_CONF conf)
	{
		return (conf>>4) + (conf&0xF);
	}

	constexpr size_t GetAudioDepthBytes(ALLEGRO_AUDIO_DEPTH depth)
	{
		return (depth & 0x07) + 1;
	}

	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct AudioFragType {
		using Type=void;
	};

	template<>
	struct AudioFragType<ALLEGRO_AUDIO_DEPTH_INT8> {
		using Type = int8_t;
	};
	template<>
	struct AudioFragType<ALLEGRO_AUDIO_DEPTH_INT16> {
		using Type = int16_t;
	};
	template<>
	struct AudioFragType<ALLEGRO_AUDIO_DEPTH_UINT8> {
		using Type = uint8_t;
	};
	template<>
	struct AudioFragType<ALLEGRO_AUDIO_DEPTH_UINT16> {
		using Type = uint16_t;
	};
	template<>
	struct AudioFragType<ALLEGRO_AUDIO_DEPTH_FLOAT32> {
		using Type = float;
	};

	template<ALLEGRO_CHANNEL_CONF ChanConf, ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType {
		using Type = void;
	};

	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_1, AudioDepth> {
		using Type = typename AudioFragType<AudioDepth>::Type;
	};
	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_2, AudioDepth> {
		using Type = Vec2<typename AudioFragType<AudioDepth>::Type>;
	};
	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_3, AudioDepth> {
		using Type = Vec3<typename AudioFragType<AudioDepth>::Type>;
	};
	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_4, AudioDepth> {
		using Type = Vec4<typename AudioFragType<AudioDepth>::Type>;
	};
	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_5_1, AudioDepth> {
		using Type = std::array<typename AudioFragType<AudioDepth>::Type, 6>;
	};
	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_6_1, AudioDepth> {
		using Type = std::array<typename AudioFragType<AudioDepth>::Type, 7>;
	};
	template<ALLEGRO_AUDIO_DEPTH AudioDepth>
	struct SampleType<ALLEGRO_CHANNEL_CONF_7_1, AudioDepth> {
		using Type = std::array<typename AudioFragType<AudioDepth>::Type, 8>;
	};

}

#endif //AXXEGRO_AUDIOCOMMON_HPP
