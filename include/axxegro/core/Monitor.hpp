#ifndef INCLUDE_AXXEGRO_DISPLAY_MONITOR
#define INCLUDE_AXXEGRO_DISPLAY_MONITOR

#include "../common.hpp"

namespace al {

	namespace detail {
		class CAdapters;
	}

	class AdapterInfo {
		int idx;
		explicit AdapterInfo(int idx)
			: idx(idx) {}
		friend class detail::CAdapters;
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

	namespace detail {
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
	}


	inline detail::CAdapters Adapters;
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_MONITOR */
