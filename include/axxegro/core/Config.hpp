#ifndef INCLUDE_AXXEGRO_RESOURCES_CONFIG
#define INCLUDE_AXXEGRO_RESOURCES_CONFIG

#include "../common.hpp"

#include <string>
#include <utility>
#include <vector>
#include <concepts>
#include <optional>
#include <functional>
#include <string_view>
#include <iterator>
#include <charconv>

#include <unordered_map>
#include <unordered_set>

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_CONFIG, al_destroy_config);


	template<typename T>
	std::string ToStr(T val) {
		AXXEGRO_STATIC_ASSERT_FALSE(T, "Cannot use this type as a config value");
		return {};
	}

	template<Number T>
	std::string ToStr(T val) {
		return std::to_string(val);
	}

	template<std::same_as<bool> T>
	std::string ToStr(T val) {
		return val ? "true" : "false";
	}

	template<std::convertible_to<std::string> T>
	std::string ToStr(T&& val) {
		return std::forward<T>(val);
	}


	template<typename T>
	std::optional<T> FromStr(const std::string_view str) {
		AXXEGRO_STATIC_ASSERT_FALSE(T, "This type is not convertible from a config value string");
	}

	template<Number T>
	std::optional<T> FromStr(const std::string_view str) {
		T val;
		std::from_chars_result result = std::from_chars(str.data(), str.data()+str.size(), val);
		if(result.ec != std::errc()) {
			return std::nullopt;
		}
		return val;
	}


	template<std::same_as<bool> T>
	std::optional<T> FromStr(const std::string_view str) {
		if(str == "true" || str == "on" || str == "0") {
			return true;
		} else if(str == "false" || str == "off" || str == "1") {
			return false;
		} else {
			return std::nullopt;
		}
	}

	template<std::same_as<std::string> T>
	std::optional<T> FromStr(const std::string_view str) {
		return T(str);
	}

	template<typename T>
	std::optional<T> FromStrOpt(const std::string_view str) {
		try {
			return FromStr<T>(str);
		} catch (std::invalid_argument& ex) {
			return std::nullopt;
		}
	}



	struct ConfigKey {
		std::string section;
		std::string name;

		static inline ConfigKey ParsePath(const std::string& path) {
			auto dot = path.find('.');
			if(dot == path.size()) {
				return {.section = "", .name = path};
			}
			return {
					.section = path.substr(0, dot),
					.name = path.substr(dot+1)
			};
		}

		[[nodiscard]] std::string path() const {
			return Format("%s%s%s", section.c_str(), section.empty() ? "" : ".", name.c_str());
		}
	};

	struct ConfigValue {
		std::string value;

		template<typename T>
		std::optional<T> as() const {
			return FromStrOpt<T>(std::string_view(value));
		}

		template<typename T>
		operator T() const {
			if(auto vOpt = this->as<T>()) {
				return *vOpt;
			} else {
				throw ConfigEntryTypeError(
					R"(expected type "%s" but got an invalid value of "%s")",
					typeid(T).name(), value.c_str()
				);
			}
		}
	};

	struct ConfigEntry {
		ConfigKey key;
		ConfigValue value;
	};



	struct ConfigEntryIterator {

		using value_type = ConfigEntry;
		using difference_type = std::ptrdiff_t;

		ConfigEntryIterator& operator++() {
			const char* keyName = al_get_next_config_entry(&it);
			if(keyName) {
				lastEntry->key.name = keyName;
				lastEntry->value = {al_get_config_value(cfg, lastEntry->key.section.c_str(), keyName)};
			} else {
				lastEntry = std::nullopt;
			}
			return *this;
		}

		ConfigEntryIterator operator++(int) {
			ConfigEntryIterator tmp = *this;
			++(*this);
			return tmp;
		}

		const value_type& operator*() const {
			return lastEntry.value();
		}

		bool operator==(const ConfigEntryIterator& other) const {
			return it == other.it && lastEntry.has_value() == other.lastEntry.has_value();
		}

		ConfigEntryIterator() = default;
		ConfigEntryIterator(const ConfigEntryIterator&) = default;
	private:
		friend struct ConfigSectionEntriesView;
		explicit ConfigEntryIterator(ALLEGRO_CONFIG* cfg, const char* section) {
			this->cfg = cfg;
			const char* keyName = al_get_first_config_entry(cfg, section, &it);
			if(keyName) {
				lastEntry = ConfigEntry {
					.key = {section, keyName},
					.value = {al_get_config_value(cfg, section, keyName)}
				};
			}
		}

		std::optional<ConfigEntry> lastEntry;
		ALLEGRO_CONFIG* cfg = nullptr;
		ALLEGRO_CONFIG_ENTRY* it = nullptr;
	};
	static_assert(std::forward_iterator<ConfigEntryIterator>);



	struct ConfigSectionEntriesView {
		[[nodiscard]] ConfigEntryIterator begin() const {
			return ConfigEntryIterator {cfg, section};
		}

		[[nodiscard]] ConfigEntryIterator end() const {
			return {};
		}

		void addComment(const std::string& comment) {
			al_add_config_comment(cfg, section, comment.c_str());
		}

		template<typename T>
		std::optional<T> get(const std::string& key) const {
			const char* val = al_get_config_value(cfg, section, key.c_str());
			if(!val) {
				return std::nullopt;
			}
			return FromStr<T>(val);
		}

		[[nodiscard]] bool contains(const std::string& key) const {
			return al_get_config_value(cfg, section, key.c_str()) != nullptr;
		}

		template<typename T>
		bool set(const std::string& key, T&& value) {
			bool ret = !contains(key);
			al_set_config_value(cfg, section, key.c_str(), ToStr(std::forward<T>(value)).c_str());
			return ret;
		}

		[[nodiscard]] const char* name() const {
			return section;
		}

	private:
		friend class Config;
		friend struct ConfigSectionIterator;
		explicit ConfigSectionEntriesView(ALLEGRO_CONFIG* cfg, const char* section) : cfg(cfg), section(section) {}

		ALLEGRO_CONFIG* cfg;
		const char* section;
	};


	struct ConfigSectionIterator {
		using value_type = ConfigSectionEntriesView;
		using difference_type = std::ptrdiff_t;

		ConfigSectionIterator& operator++() {
			const char* sectionName = al_get_next_config_section(&it);
			if(sectionName) {
				lastSection = value_type {cfg, sectionName};
			} else {
				lastSection = std::nullopt;
			}
			return *this;
		}

		ConfigSectionIterator operator++(int) {
			ConfigSectionIterator tmp = *this;
			++(*this);
			return tmp;
		}

		const value_type& operator*() const {
			return lastSection.value();
		}

		bool operator==(const ConfigSectionIterator& other) const {
			return it == other.it && lastSection.has_value() == other.lastSection.has_value();
		}

		ConfigSectionIterator() = default;
		ConfigSectionIterator(const ConfigSectionIterator&) = default;
	private:
		friend struct ConfigSectionView;
		explicit ConfigSectionIterator(ALLEGRO_CONFIG* cfg) {
			this->cfg = cfg;
			const char* sectionName = al_get_first_config_section(cfg, &it);
			if(sectionName) {
				lastSection = value_type {cfg, sectionName};
			}
		}

		std::optional<value_type> lastSection;
		ALLEGRO_CONFIG* cfg = nullptr;
		ALLEGRO_CONFIG_SECTION* it = nullptr;
	};
	static_assert(std::forward_iterator<ConfigSectionIterator>);

	struct ConfigSectionView {
		[[nodiscard]] ConfigSectionIterator begin() const {
			return ConfigSectionIterator {cfg};
		}

		[[nodiscard]] ConfigSectionIterator end() const {
			return {};
		}
	private:
		friend class Config;
		explicit ConfigSectionView(ALLEGRO_CONFIG* cfg) : cfg(cfg) {}
		ALLEGRO_CONFIG* cfg;
	};



	class Config:
			RequiresInitializables<CoreAllegro>,
			public Resource<ALLEGRO_CONFIG> {
	public:
		using Resource::Resource;

		explicit Config(const std::string& filename)
			: Resource<ALLEGRO_CONFIG>(al_load_config_file(filename.c_str()))
		{
			if(!ptr()) {
				throw ResourceLoadError("Cannot load config from file '%s'", filename.c_str());
			}
		}

		Config()
			: Resource(al_create_config())
		{

		}

		[[nodiscard]] bool contains(const ConfigKey& key) const {
			return al_get_config_value(ptr(), key.section.c_str(), key.name.c_str()) != nullptr;
		}

		[[nodiscard]] bool contains(const std::string& path) const {
			return contains(ConfigKey::ParsePath(path));
		}

		template<typename T>
		std::optional<T> get(const ConfigKey& key) {
			return section(key.section).get<T>(key.name);
		}

		template<typename T>
		bool set(const ConfigKey& key, T&& value) {
			return section(key.section).set<T>(key.name, std::forward<T>(value));
		}

		template<typename T>
		std::optional<T> get(const std::string& path) {
			return get<T>(ConfigKey::ParsePath(path));
		}

		template<typename T>
		bool set(const std::string& path, T&& value) {
			return set<T>(ConfigKey::ParsePath(path), std::forward<T>(value));
		}

		ConfigSectionView sections() {
			return ConfigSectionView(ptr());
		}

		ConfigSectionEntriesView section(const std::string& sectionName) {
			return ConfigSectionEntriesView(ptr(), sectionName.c_str());
		}

		bool saveToFile(const std::string& filename) {
			return al_save_config_file(filename.c_str(), ptr());
		}

		void saveToFileOrThrow(const std::string& filename) {
			if(!saveToFile(filename)) {
				throw ConfigError("cannot save config to file '%s'", filename.c_str());
			}
		}

		bool removeSection(const std::string& section) {
			return al_remove_config_section(ptr(), section.c_str());
		}

		/**
		 * @brief Merges two configs, with values in `other` overriding those in `*this`.
		 */
		void merge(const Config& other) {
			al_merge_config_into(ptr(), other.ptr());
		}


		/**
		 * @brief Merges two configs, with values in `source` overriding those in `master`.
		 */
		static Config Merge(const Config& master, const Config& source) {
			ALLEGRO_CONFIG* newCfg = al_merge_config(master.ptr(), source.ptr());
			return Config(newCfg);
		}

	private:

	};


}

#endif /* INCLUDE_AXXEGRO_RESOURCES_CONFIG */
