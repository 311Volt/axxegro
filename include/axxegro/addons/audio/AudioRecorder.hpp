#ifndef AXXEGRO_AUDIORECORDER_HPP
#define AXXEGRO_AUDIORECORDER_HPP

#ifdef ALLEGRO_UNSTABLE
#include "AudioCommon.hpp"
#include "AudioUtil.hpp"
#include "../../core/event/UserEvent.hpp"
#include "../../core/time/FreqPeriod.hpp"

namespace al {


	AXXEGRO_DEFINE_DELETER(ALLEGRO_AUDIO_RECORDER, al_destroy_audio_recorder);

	using RawAudioRecorderEvent = ALLEGRO_AUDIO_RECORDER_EVENT;

	template<>
	struct EventDataGetter<RawAudioRecorderEvent> {
		const RawAudioRecorderEvent& operator()(const Event& ev) {
			return *(al_get_audio_recorder_event(const_cast<Event*>(&ev)));
		}
	};

	class BaseAudioRecorder;
	struct AudioRecorderEventSource: public EventSource {
	public:
		explicit AudioRecorderEventSource(BaseAudioRecorder& recorder) : recorder(recorder) {}
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		BaseAudioRecorder& recorder;
	};

	class BaseAudioRecorder: public Resource<ALLEGRO_AUDIO_RECORDER> {
	public:

		explicit BaseAudioRecorder(
				AudioFormat fmt = {.depth = ALLEGRO_AUDIO_DEPTH_INT16},
				BufferConfig bufferConfig = {}
		)
		: Resource(nullptr), evSource(std::make_unique<AudioRecorderEventSource>(*this))
		{
			setPtr(al_create_audio_recorder(
				bufferConfig.numChunks,
				bufferConfig.fragmentsPerChunk,
				fmt.frequency, fmt.depth, fmt.chanConf
			));
			if(!ptr()) {
				throw AudioError(
					"Cannot create audio recorder (requested config: %s format with %d*%d buffers)",
					fmt.str().c_str(), bufferConfig.numChunks, bufferConfig.fragmentsPerChunk
				);
			}
		}

		bool start() {
			return al_start_audio_recorder(ptr());
		}

		void stop() {
			al_stop_audio_recorder(ptr());
		}

		bool isRecording() {
			return al_is_audio_recorder_recording(ptr());
		}

		EventSource& getEventSource() {
			return *evSource;
		}

	private:
		std::unique_ptr<AudioRecorderEventSource> evSource;
	};

	inline ALLEGRO_EVENT_SOURCE *al::AudioRecorderEventSource::ptr() const
	{
		return al_get_audio_recorder_event_source(recorder.ptr());
	}

	namespace internal {
		template<ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
		class TypedAudioRecorder: public BaseAudioRecorder {
		public:

			struct Traits {
				static constexpr ALLEGRO_AUDIO_DEPTH Depth = AudioDepthOf<TSample>;
				static constexpr ALLEGRO_CHANNEL_CONF ChanConf = TPChanConf;
				using SampleType = TSample;
				using FragmentType = AudioFragmentType<TPChanConf, Depth>::Type;
			};


			explicit TypedAudioRecorder(Freq frequency = al::Hz(44100), BufferConfig bufConfig = {})
				: BaseAudioRecorder(
					{
						.frequency = (unsigned)frequency.getFreqHz(),
						.depth = Traits::Depth,
						.chanConf = Traits::ChanConf
					},
					bufConfig
			)
			{

			}


			static_assert(
					sizeof(typename Traits::FragmentType) == GetChannelCount(TPChanConf)*sizeof(TSample),
					"Your implementation does not support contiguous representation of audio fragment vectors."
					"Use BaseAudioRecorder and do buffer type management manually."
			);

			EventHandler<RawAudioRecorderEvent> createHandler(
					std::function<void(const std::span<typename Traits::FragmentType>)> fn)
			{
				using FragT = typename Traits::FragmentType;

				return [fn, this](const RawAudioRecorderEvent& ev, [[maybe_unused]] const al::AnyEvent& meta) {
					if(ev.source != this->ptr()) {
						throw AudioError("Audio recorder event is being handled by a handler created by another recorder");
					}

					fn(std::span<FragT>((FragT*)ev.buffer, (FragT*)ev.buffer + ev.samples));
					return EventHandled;
				};
			}

		private:

		};
	}




	template<ValidSampleType TSample, ALLEGRO_CHANNEL_CONF TPChanConf>
	class AudioRecorder: public internal::TypedAudioRecorder<TSample, TPChanConf> {
		using internal::TypedAudioRecorder<TSample, TPChanConf>::TypedAudioRecorder;

		using Traits = internal::TypedAudioRecorder<TSample, TPChanConf>::Traits;
	};

#ifndef AXXEGRO_USE_NATIVE_FLOAT32_AUDIO_RECORDER

	template<ALLEGRO_CHANNEL_CONF TPChanConf>
	class AudioRecorder<float, TPChanConf>: public internal::TypedAudioRecorder<int16_t, TPChanConf> {
	public:
		using Super = internal::TypedAudioRecorder<int16_t, TPChanConf>;
		using internal::TypedAudioRecorder<int16_t, TPChanConf>::TypedAudioRecorder;
		using ImplTraits = typename Super::Traits;
		using Traits = ::al::internal::TypedAudioRecorder<float, TPChanConf>::Traits;

		EventHandler<RawAudioRecorderEvent> createHandler(std::function<void(const std::span<typename Traits::FragmentType>)> fn) {
			return [fn, this](const RawAudioRecorderEvent& ev, [[maybe_unused]] const al::AnyEvent& meta) {
				using ImplFragT = typename ImplTraits::FragmentType;

				if(ev.source != this->ptr()) {
					throw AudioError("Audio recorder event is being handled by a handler created by another recorder");
				}

				floatFragBuffer.resize(ev.samples);
				std::span<ImplFragT> inputBuffer((ImplFragT*)ev.buffer, (ImplFragT*)ev.buffer + ev.samples);
				if(!ConvertFragmentsToFloat(inputBuffer, floatFragBuffer)) {
					throw AudioError("internal error");
				}
				fn(floatFragBuffer);
				return EventHandled;
			};
		}
	private:
		std::vector<typename Traits::FragmentType> floatFragBuffer;
	};

#endif

}

#endif

#endif //AXXEGRO_AUDIORECORDER_HPP
