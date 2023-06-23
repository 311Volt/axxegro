#ifndef INCLUDE_AXXEGRO_SHADER
#define INCLUDE_AXXEGRO_SHADER

#include "../common.hpp"
#include "gfx/Bitmap.hpp"

#include <span>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_SHADER, al_destroy_shader);

	class Shader: public Resource<ALLEGRO_SHADER> {
	public:
		explicit Shader(ALLEGRO_SHADER_PLATFORM platform = ALLEGRO_SHADER_AUTO)
				: Resource(al_create_shader(platform))
		{
			if(!ptr()) {
				throw ShaderError("Cannot create shader. This error might have occured because an incompatible shader platform was specified.");
			}
		}


		void attachSourceCode(const std::string& src, ALLEGRO_SHADER_TYPE type) {
			if(!al_attach_shader_source(ptr(), type, src.c_str())) {
				throw ShaderSourceError("Cannot attach shader source: %s\n", getLog());
			}
		}
		void attachSourceFile(const std::string& filename, ALLEGRO_SHADER_TYPE type) {
			if(!al_attach_shader_source_file(ptr(), type, filename.c_str())) {
				throw ShaderSourceError("Cannot attach shader source file %s: \n%s", filename.c_str(), getLog());
			}
		}

		void attachPixelShader(const std::string& src) {
			attachSourceCode(src, ALLEGRO_PIXEL_SHADER);
		}
		void attachVertexShader(const std::string& src) {
			attachSourceCode(src, ALLEGRO_VERTEX_SHADER);
		}

		void attachPixelShaderFile(const std::string& filename) {
			attachSourceFile(filename, ALLEGRO_PIXEL_SHADER);
		}
		void attachVertexShaderFile(const std::string& filename) {
			attachSourceFile(filename, ALLEGRO_VERTEX_SHADER);
		}

		void build() {
			if(!al_build_shader(ptr())) {
				throw ShaderBuildError("Cannot build shader: \n" + std::string(getLog()));
			}
		}

		[[nodiscard]] const char* getLog() const {
			return al_get_shader_log(ptr());
		}
		[[nodiscard]] ALLEGRO_SHADER_PLATFORM getPlatform() const {
			return al_get_shader_platform(ptr());
		}
		void use() {
			if(!al_use_shader(ptr())) {
				throw ShaderError("Cannot use shader: " + std::string(getLog()));
			}
		}

		static void Reset() {
			al_use_shader(nullptr);
		}

		static std::string GetDefaultSource(ALLEGRO_SHADER_PLATFORM platform, ALLEGRO_SHADER_TYPE type) {
			const char* src = al_get_default_shader_source(platform, type);
			if(!src) {
				throw ShaderError("Cannot get default shader source. Allegro might have been built without support for shaders for the specified platform.");
			}
			return src;
		}

		void attachDefaultVertexShader() {
			attachVertexShader(GetDefaultSource(getPlatform(), ALLEGRO_VERTEX_SHADER));
		}

		void attachDefaultPixelShader() {
			attachVertexShader(GetDefaultSource(getPlatform(), ALLEGRO_PIXEL_SHADER));
		}

		static bool SetBool(const std::string &name, bool value) {
			return al_set_shader_bool(name.c_str(), value);
		}
		
		static bool SetInt(const std::string &name, int value) {
			return al_set_shader_int(name.c_str(), value);
		}

		static bool SetFloat(const std::string &name, float value) {
			return al_set_shader_float(name.c_str(), value);
		}

		static bool SetSampler(const std::string& name, al::Bitmap& bitmap, int unit) {
			return al_set_shader_sampler(name.c_str(), bitmap.ptr(), unit);
		}

		template<typename Vec>
		static bool SetVector(const std::string& name, const Vec& vec)
		{
			if constexpr(std::is_same_v<typename Vec::ValueType, int>) {
				return al_set_shader_int_vector(name.c_str(), Vec::NumElements, (int*)&vec, 1);
			} else if constexpr(std::is_same_v<typename Vec::ValueType, float>) {
				return al_set_shader_float_vector(name.c_str(), Vec::NumElements, (float*)&vec, 1);
			} else {
				AXXEGRO_STATIC_ASSERT_FALSE(Vec, "Cannot set shader vector array uniform for this type");
			}
		}

		template<typename Vec>
		static bool SetVector(const std::string& name, const std::span<Vec> vec)
		{
			if constexpr(std::is_same_v<typename Vec::ValueType, int>) {
				return al_set_shader_int_vector(name.c_str(), Vec::NumElements, (int*)vec.data(), vec.size());
			} else if constexpr(std::is_same_v<typename Vec::ValueType, float>) {
				return al_set_shader_float_vector(name.c_str(), Vec::NumElements, (float*)vec.data(), vec.size());
			} else if constexpr(std::is_same_v<Vec, int>) {
				return al_set_shader_int_vector(name.c_str(), 1, vec.data(), vec.size());
			} else if constexpr(std::is_same_v<Vec, float>) {
				return al_set_shader_float_vector(name.c_str(), 1, vec.data(), vec.size());
			} else {
				AXXEGRO_STATIC_ASSERT_FALSE(Vec, "Cannot set shader vector array uniform for this type");
			}
		}
	};

}

#endif /* INCLUDE_AXXEGRO_SHADER */
