#ifndef F61BADF2_7D09_47EB_BE3B_4FDA2D3984F5
#define F61BADF2_7D09_47EB_BE3B_4FDA2D3984F5

#include "audio.hpp"
#include <allegro5/allegro_acodec.h>

namespace al {

	struct AudioCodecAddon {
		static constexpr char name[] = "Audio codec addon";
		[[nodiscard]] static bool isInitialized() {return al_is_acodec_addon_initialized();}
		[[nodiscard]] static bool init() {return al_init_acodec_addon();}
		using DependsOn = InitDependencies<AudioAddon>;
	};

	inline Sample LoadSample(const std::string& filename) {
		InternalRequire<AudioCodecAddon>();
		if(auto* ptr = al_load_sample(filename.c_str())) {
			return Sample(ptr);
		} else {
			throw ResourceLoadError("Cannot load sample from file \"%s\"", filename.c_str());
		}
	}

	namespace detail {
		class AudioStreamLoader {
		public:

			BaseAudioStream load(const std::string& filename, BufferConfig bufConfig) {
				return BaseAudioStream(loadRaw(filename, bufConfig), bufConfig);
			}

			template<ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
			AudioStream<TSample, TPChanConf> loadTyped(const std::string& filename, BufferConfig bufConfig) {
				return AudioStream<TSample, TPChanConf>(loadRaw(filename, bufConfig), bufConfig);
			}
		private:
			ALLEGRO_AUDIO_STREAM* loadRaw(const std::string& filename, BufferConfig bufConfig) {
				InternalRequire<AudioCodecAddon>();
				if(auto* p = al_load_audio_stream(filename.c_str(), bufConfig.numChunks, bufConfig.fragmentsPerChunk)) {
					return p;
				} else {
					throw AudioError("Cannot load audio stream from file %s", filename.c_str());
				}
			}
		};
	}

	inline BaseAudioStream LoadAudioStream(const std::string& filename, BufferConfig bufConfig = {.numChunks = 2, .fragmentsPerChunk = 2048}) {
		return detail::AudioStreamLoader{}.load(filename, bufConfig);
	};

	template<ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
	inline AudioStream<TSample, TPChanConf> LoadTypedAudioStream(const std::string& filename, BufferConfig bufConfig = {.numChunks = 2, .fragmentsPerChunk = 2048}) {
		return detail::AudioStreamLoader{}.loadTyped<TSample, TPChanConf>(filename, bufConfig);
	}
}

#endif /* F61BADF2_7D09_47EB_BE3B_4FDA2D3984F5 */
