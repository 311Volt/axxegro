#ifndef INCLUDE_AXXEGRO_ADDONS_VIDEO_VIDEO
#define INCLUDE_AXXEGRO_ADDONS_VIDEO_VIDEO

#include "../../core.hpp"
#include "../audio.hpp"
#include "VideoAddon.hpp"

#include <allegro5/allegro_video.h>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_VIDEO, al_close_video);
	
	class Video;
	
	class VideoEventSource: public EventSource {
	public:
		explicit VideoEventSource(Video& vid)
			: vid(vid)
		{}
		virtual ~VideoEventSource() = default;
		
		[[nodiscard]] ALLEGRO_EVENT_SOURCE* ptr() const override;
	private:
		Video& vid;
	};
	
	class Video:
			RequiresInitializables<VideoAddon>,
			public Resource<ALLEGRO_VIDEO> {
	public:

		static std::string Identify(const std::string& filename) {
			return al_identify_video(filename.c_str());
		}

		explicit Video(const std::string& filename)
		 : Resource<ALLEGRO_VIDEO>(nullptr) {

			setPtr(al_open_video(filename.c_str()));
			if(!ptr()) {
				throw ResourceLoadError("Cannot open video: %s", filename.c_str());
			}
			
			eventSource = std::make_unique<VideoEventSource>(*this);
		}
		
		
		void rewind() {
			al_seek_video(ptr(), 0.0);
		}

		[[nodiscard]] double getFPS() const {
			return al_get_video_fps(ptr());
		}

		[[nodiscard]] double getPos(ALLEGRO_VIDEO_POSITION_TYPE which = ALLEGRO_VIDEO_POSITION_ACTUAL) const {
			return al_get_video_position(ptr(), which);
		}

		al::Vec2f getScaledDimensions() {
			return {
				al_get_video_scaled_width(ptr()),
				al_get_video_scaled_height(ptr())
			};
		}
		
		void start(Mixer& mixer = DefaultMixer) {
			al_start_video(ptr(), mixer.ptr());
		}
		
		void start(Voice& voice) {
			al_start_video_with_voice(ptr(), voice.ptr());
		}

		[[nodiscard]] bool isPlaying() const {
			return al_is_video_playing(ptr());
		}

		const Bitmap* getFrame() {
			static Bitmap currentFrame {nullptr, ResourceModel::NonOwning};

			currentFrame.setPtr(al_get_video_frame(ptr()));
			return currentFrame.ptr() ? &currentFrame : nullptr;
		}
		
		VideoEventSource& getEventSource() {
			return *eventSource;
		}
	private:
		
		std::unique_ptr<VideoEventSource> eventSource;

		static void ensureVideoAddonInitialized() {
			if(!al_is_video_addon_initialized()) {
				al_init_video_addon();
			}
		}
	};
	
	
	inline ALLEGRO_EVENT_SOURCE *VideoEventSource::ptr() const
	{
		return al_get_video_event_source(vid.ptr());
	}
	
}

#endif /* INCLUDE_AXXEGRO_ADDONS_VIDEO_VIDEO */
