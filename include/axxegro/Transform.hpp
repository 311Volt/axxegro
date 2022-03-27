#ifndef INCLUDE_AXXEGRO_TRANSFORM
#define INCLUDE_AXXEGRO_TRANSFORM

#include <allegro5/allegro.h>

namespace al {
	class ScopedTransform {
		const ALLEGRO_TRANSFORM* originalTransform;
	public:
		ScopedTransform(ALLEGRO_TRANSFORM* t)
			: originalTransform(al_get_current_transform())
		{
			al_use_transform(t);
		}

		~ScopedTransform()
		{
			al_use_transform(const_cast<ALLEGRO_TRANSFORM*>(originalTransform));
		}
	};
}

#endif /* INCLUDE_AXXEGRO_TRANSFORM */
