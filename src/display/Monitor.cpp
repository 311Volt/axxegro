#include <axxegro/display/Monitor.hpp>

#include <string>

al::CAdapters al::Adapters;

al::Rect<int> al::AdapterInfo::rect() const
{
	ALLEGRO_MONITOR_INFO m;
	al_get_monitor_info(idx, &m);
	return al::Rect<int>(m.x1, m.x2, m.y1, m.y2);
}

al::AdapterInfo al::CAdapters::operator[](int idx) const
{
	if(idx < 0 || idx > size()) {
		throw std::out_of_range("Invalid video adapter index: " + std::to_string(idx));
	}
	return AdapterInfo(idx);
}