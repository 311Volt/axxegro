#ifndef INCLUDE_AXXEGRO_DISPLAY_MONITOR
#define INCLUDE_AXXEGRO_DISPLAY_MONITOR

#include <allegro5/allegro.h>
#include <axxegro/math/math.hpp>

#include <stdexcept>

namespace al {
	
	class AdapterInfo {
		int idx;
		AdapterInfo(int idx)
			: idx(idx) {}
		friend class CAdapters;
	public:

		Rect<int> rect() const;
		inline int dpi() const
		{
			return al_get_monitor_dpi(idx);
		}
	};

	class CAdapters {
	public:
		inline int size() const
		{
			return al_get_num_video_adapters();
		}

		inline int getDefaultIndex() const
		{
			return al_get_new_display_adapter();
		}

		inline void setDefault(int adapter)
		{
			return al_set_new_display_adapter(adapter);
		}

		AdapterInfo operator[](int idx) const;

		inline AdapterInfo getDefault()
		{
			return operator[](getDefaultIndex());
		}
	};

	extern CAdapters Adapters;
}

#endif /* INCLUDE_AXXEGRO_DISPLAY_MONITOR */
