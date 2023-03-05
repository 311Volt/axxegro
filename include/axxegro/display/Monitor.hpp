#ifndef INCLUDE_AXXEGRO_DISPLAY_MONITOR
#define INCLUDE_AXXEGRO_DISPLAY_MONITOR

#include <allegro5/allegro.h>
#include <axxegro/math/math.hpp>
#include <axxegro/Exception.hpp>

#include <stdexcept>

namespace al {
	
	class AdapterInfo {
		int idx;
		explicit AdapterInfo(int idx)
			: idx(idx) {}
		friend class CAdapters;
	public:

		[[nodiscard]] Rect<int> rect() const {
			ALLEGRO_MONITOR_INFO m;
			al_get_monitor_info(idx, &m);
			return {m.x1, m.y1, m.x2, m.y2};
		}
		[[nodiscard]] int dpi() const {
			return al_get_monitor_dpi(idx);
		}
	};

	class CAdapters {
	public:
		[[nodiscard]] int size() const {
			return al_get_num_video_adapters();
		}

		[[nodiscard]] int getDefaultIndex() const {
			return al_get_new_display_adapter();
		}

		void setDefault(int adapter) {
			return al_set_new_display_adapter(adapter);
		}

		AdapterInfo operator[](int idx) const {
			if(idx < 0 || idx > size()) {
				throw OutOfRangeError("Invalid video adapter index: " + std::to_string(idx));
			}
			return AdapterInfo(idx);
		}

		[[nodiscard]] AdapterInfo getDefault() const {
			return operator[](getDefaultIndex());
		}
	};

	inline CAdapters Adapters;
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_MONITOR */
