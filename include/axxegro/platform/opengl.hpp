//
// Created by volt on 2023-06-16.
//

#ifndef AXXEGRO_OPENGL_HPP
#define AXXEGRO_OPENGL_HPP

#include "axxegro/core/display.hpp"
#include "axxegro/core/Shader.hpp"
#include <allegro5/allegro_opengl.h>
#include <optional>

namespace al::gl {

	inline ALLEGRO_OGL_EXT_LIST& GetExtensionList() {
		return *al_get_opengl_extension_list();
	}

	inline void* GetProcAddr(const char* name) {
		return al_get_opengl_proc_address(name);
	}

	template<class> struct Proc{};

	template<typename R, typename... Args>
	struct Proc<R(Args...)> {
		ALLEGRO_DEFINE_PROC_TYPE(R, ProcFnPtr, (Args...))

		static inline auto Get(const char* name) {
			return (ProcFnPtr)(GetProcAddr(name));
		}
	};

	inline GLuint GetTexture(al::Bitmap& bitmap) {
		return al_get_opengl_texture(bitmap.ptr());
	}

	inline std::optional<Vec2i> GetTextureSize(const al::Bitmap& bitmap) {
		int w, h;
		if(al_get_opengl_texture_size(bitmap.ptr(), &w, &h)) {
			return Vec2i {w, h};
		} else {
			return std::nullopt;
		}
	}

	inline Vec2i GetTexturePos(const al::Bitmap& bitmap) {
		int u, v;
		al_get_opengl_texture_position(bitmap.ptr(), &u, &v);
		return {u, v};
	}

	inline GLuint GetProgramObject(al::Shader& shader) {
		return al_get_opengl_program_object(shader.ptr());
	}

	inline GLuint GetFBO(al::Bitmap& bitmap) {
		return al_get_opengl_fbo(bitmap.ptr());
	}

	inline void RemoveFBO(al::Bitmap& bitmap) {
		al_remove_opengl_fbo(bitmap.ptr());
	}

	inline bool HaveExtension(const char* extension) {
		return al_have_opengl_extension(extension);
	}

	inline uint32_t GetVersion() {
		return al_get_opengl_version();
	}

	inline std::string GetVersionStr() {
		uint32_t v = GetVersion();
		std::string ret;
		for(int i=24; i>=0; i-=8) {
			uint8_t num = v>>i;
			ret += Format("%s%d", (i==24)?".":"", (int)num);
		}
		return ret;
	}

	inline int GetVariant() {
		return al_get_opengl_variant();
	}

	void SetCurrentContext(al::Display& display) {
		return al_set_current_opengl_context(display.ptr());
	}
}

#endif //AXXEGRO_OPENGL_HPP
