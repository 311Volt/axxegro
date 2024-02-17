#ifndef INCLUDE_AXXEGRO_SHADER
#define INCLUDE_AXXEGRO_SHADER

#include "../common.hpp"
#include "gfx/Bitmap.hpp"

#include <span>

#include "Transform.hpp"

namespace al {

	template<typename T>
	concept ShaderUniformScalarElement = std::same_as<T, int> || std::same_as<T, float> || std::same_as<T, bool>;

	template<typename TVec>
	concept ShaderUniformVectorElement = requires {
		requires TVec::NumElements > 1;
		requires std::same_as<typename TVec::ElementType, int> || std::same_as<typename TVec::ElementType, float>;
	};

	template<typename T>
	concept ShaderUniformElement = ShaderUniformScalarElement<T> || ShaderUniformVectorElement<T>;


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
				throw ShaderError("Cannot use shader: \n" + std::string(getLog()));
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

		//scalar uniforms
		template<typename T>
			requires ShaderUniformScalarElement<T>
		static bool TrySetUniform(const std::string& name, T value) {
			if constexpr (std::is_same_v<T, bool>) {
				return al_set_shader_bool(name.c_str(), value);
			} else if constexpr (std::is_same_v<T, int>) {
				return al_set_shader_int(name.c_str(), value);
			} else if constexpr (std::is_same_v<T, float>) {
				return al_set_shader_float(name.c_str(), value);
			}
			return false;
		}

		//vector uniforms
		template<typename T, int N>
			requires ShaderUniformVectorElement<Vec<T, N>>
		static bool TrySetUniform(const std::string& name, const Vec<T, N>& vec)
		{
			if constexpr(std::is_same_v<T, int>) {
				return al_set_shader_int_vector(name.c_str(), N, vec.data(), 1);
			} else if constexpr(std::is_same_v<T, float>) {
				return al_set_shader_float_vector(name.c_str(), N, vec.data(), 1);
			}
			return false;
		}

		//array of vector uniforms
		template<std::ranges::contiguous_range TRange>
			requires ShaderUniformVectorElement<std::remove_cvref_t<std::ranges::range_value_t<TRange>>>
		static bool TrySetUniform(const std::string& name, TRange&& elements) {
			using TElement = typename std::remove_cvref_t<std::ranges::range_value_t<TRange>>::ElementType;
			constexpr int NumElements = TElement::NumElements;

			if constexpr(std::is_same_v<TElement, int>) {
				return al_set_shader_int_vector(name.c_str(), NumElements, std::ranges::data(elements), std::ranges::size(elements));
			} else if constexpr(std::is_same_v<TElement, float>) {
				return al_set_shader_float_vector(name.c_str(), NumElements, std::ranges::data(elements), std::ranges::size(elements));
			}
			return false;
		}

		//array of scalar uniforms
		template<std::ranges::contiguous_range TRange>
			requires requires {
				requires not VectorType<std::remove_cvref_t<TRange>>;
				requires ShaderUniformScalarElement<std::remove_cvref_t<std::ranges::range_value_t<TRange>>>;
			}
		static bool TrySetUniform(const std::string& name, TRange&& elements) {
			using TElement = std::remove_cvref_t<std::ranges::range_value_t<TRange>>;

			if constexpr(std::is_same_v<TElement, int>) {
				return al_set_shader_int_vector(name.c_str(), 1, std::ranges::data(elements), std::ranges::size(elements));
			} else if constexpr(std::is_same_v<TElement, float>) {
				return al_set_shader_float_vector(name.c_str(), 1, std::ranges::data(elements), std::ranges::size(elements));
			}
			return false;
		}

		static bool TrySetUniform(const std::string& name, const Transform& mtx) {
			return al_set_shader_matrix(name.c_str(), &mtx);
		}


		static bool TrySetSampler(const std::string& name, Bitmap& bitmap, int unit) {
			return al_set_shader_sampler(name.c_str(), bitmap.ptr(), unit);
		}

		template<typename T>
		static void SetUniform(const std::string& name, T&& value) {
			if(not TrySetUniform(name, std::forward<T>(value))) {
				throw ShaderError("Cannot set uniform \"%s\": no such uniform or type mismatch", name.c_str());
			}
		}

		static void SetSampler(const std::string& name, Bitmap& bitmap, int unit) {
			if(not TrySetSampler(name, bitmap, unit)) {
				throw ShaderError("Cannot set sampler %s at unit %d", name.c_str(), unit);
			}
		}
	};

}

#endif /* INCLUDE_AXXEGRO_SHADER */
