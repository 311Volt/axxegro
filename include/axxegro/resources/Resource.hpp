#ifndef __AXXEGRO_RESOURCE_H__
#define __AXXEGRO_RESOURCE_H__

/**
 @file
Resource is the base class for classes that correspond to Allegro structures 
that have a create/destroy pair.

Classes that derive from Resource shall provide:
	- RAII for underlying Allegro objects (ctor -> create, dtor -> delete)
	- error checking (exceptions in ctors)
	- methods that correspond to appropriate Allegro functions 
	   (instead of having the user write al_do_something(res.alPtr(), ...))

A Resource must point to a valid Allegro object for its entire lifetime.
*/

namespace al {
	class Resource {
	public:
		Resource(){}
		Resource(Resource&) = delete;
		~Resource(){}//virtual ~Resource() = 0;
	};
}

#endif // __AXXEGRO_RESOURCE_H__