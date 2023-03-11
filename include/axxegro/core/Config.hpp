#ifndef INCLUDE_AXXEGRO_RESOURCES_CONFIG
#define INCLUDE_AXXEGRO_RESOURCES_CONFIG

#include "../common.hpp"

#include <string>
#include <vector>
#include <memory>

#include <unordered_map>
#include <unordered_set>

/**
 * @file
 * al::Config wraps around ALLEGRO_CONFIG and extends it with a
 * section selection mechanism.
 */

namespace al {

	AXXEGRO_DEFINE_DELETER(ALLEGRO_CONFIG, al_destroy_config);

	class Config: public Resource<ALLEGRO_CONFIG> {
	public:
		using Resource::Resource;
		Config()
				: Resource(al_create_config())
		{
			filename = "";
			currentSection = "";
		}
		explicit Config(const std::string& filename)
				: Resource(al_load_config_file(filename.c_str()))
		{
			if(!ptr()) {
				throw ResourceLoadError("Cannot load config from %s - file missing, corrupted or invalid", filename.c_str());
			}
			currentSection = "";
		}

		/**
		 * @brief Provides a RAII-style mechanism for selecting a section to prevent
		 * the user from forgetting to set it back to the old value.
		 */
		struct SectionSelector {
			///@brief Calls cfg.selectSection(sectionName).
			SectionSelector(Config& cfg, const std::string& sectionName)
					: cfg(cfg)
			{
				oldSectionName = cfg.getSelectedSectionName();
				cfg.selectSection(sectionName);
			}


			///@brief Restores the section selection from before the construction of the object.
			~SectionSelector() {
				cfg.selectSection(oldSectionName);
			}
		private:
			al::Config& cfg;
			std::string oldSectionName;
		};

		/**
		 * @brief Selects the section that will be used for isPresent,
		 * getValue and setValue. If the section doesn't exist, it is
		 * created automatically.
		 * 
		 * By default, the global section (i.e. "") is selected.
		 * 
		 * SectionSelector provides a RAII alternative for this method.
		 */
		void selectSection(const std::string& name) {
			currentSection = name;
		}

		///@returns Returns the name of the currently selected section.
		[[nodiscard]] const std::string& getSelectedSectionName() const {
			return currentSection;
		}

		///@brief Adds a section. If it already exists, nothing happens.
		void addSection(const std::string& name) {
			al_add_config_section(ptr(), name.c_str());
		}

		/**
		 * @brief Removes a section and all its associated key-value pairs.
		 * 
		 * @return true if the section existed.
		 * @return false if the section did not exist.
		 */
		bool removeSection(const std::string& name) {
			return al_remove_config_section(ptr(), name.c_str());
		}

		///@brief Checks whether a key exists in the currently selected section.
		[[nodiscard]] bool isPresent(const std::string& key) const {
			return isPresent(currentSection, key);
		}

		///@brief Checks whether a key exists.
		[[nodiscard]] bool isPresent(const std::string& section, const std::string& key) const {
			return al_get_config_value(ptr(), section.c_str(), key.c_str());
		}

		/**
		 * @brief Returns the value associated with a given key in the currently selected section.
		 * Throws if the key does not exist.
		 * 
		 * @throws std::runtime_error if the key did not exist.
		 */
		[[nodiscard]] std::string getValue(const std::string& key) const {
			return getValue(currentSection, key);
		}

		/**
		 * @brief Returns the value associated with a given key in the currently selected section.
		 * Throws if the key does not exist.
		 * 
		 * @throws std::runtime_error if the key did not exist.
		 */
		[[nodiscard]] std::string getValue(const std::string& section, const std::string& key) const {
			return {al_get_config_value(ptr(), section.c_str(), key.c_str())};
		}

		/**
		 * @brief Sets the value for a given key in the currently selected section. 
		 * If the key does not exist, it is created automatically.
		 */
		void setValue(const std::string& key, const std::string& value) {
			setValue(currentSection, key, value);
		}

		/**
		 * @brief Sets the value for a given key.
		 * If the key does not exist, it is created automatically.
		 */
		void setValue(const std::string& section, const std::string& key, const std::string& value) {
			al_set_config_value(ptr(), section.c_str(), key.c_str(), value.c_str());
		}

		/**
		 * @brief Adds a comment in the current section.
		 */
		void addComment(const std::string& comment) {
			addComment(currentSection, comment);
		}

		/**
		 * @brief Adds a comment in a given section.
		 */
		void addComment(const std::string& section, const std::string& comment) {
			al_add_config_comment(ptr(), section.c_str(), comment.c_str());
		}

		///@returns A list of all keys in the currently selected section.
		[[nodiscard]] std::vector<std::string> keys() const {
			return keys(currentSection);
		}

		///@returns A list of all keys in a given section.
		[[nodiscard]] std::vector<std::string> keys(const std::string& section) const {
			ALLEGRO_CONFIG_ENTRY* iterator;
			std::vector<std::string> ret;

			const char* entry = al_get_first_config_entry(ptr(), section.c_str(), &iterator);
			if(!entry) {
				/* if the first entry does not exist (NULL) then the iterator
				 * is not valid, nor will it be set to NULL
				 * (as I write this, this behavior is not documented) */
				return {};
			}
			while(iterator) {
				if(entry) {
					ret.emplace_back(entry);
				}
				entry = al_get_next_config_entry(&iterator);
			}
			return ret;
		}

		/**
		 * @returns A list of all section names in the configuration file, including the global section ("").
		 * The return of an empty string at the beginning is guaranteed, unlike in Allegro5.
		 */
		[[nodiscard]] std::vector<std::string> sections() const {
			ALLEGRO_CONFIG_SECTION* iterator;

			std::vector<std::string> ret = {""};
			std::unordered_set<std::string> existingValues;

			const char* entry = al_get_first_config_section(ptr(), &iterator);
			while(iterator) {
				if(entry) {
					std::string strEntry(entry);
					if(not strEntry.empty() && existingValues.count(strEntry)==0) {
						ret.push_back(strEntry);
						existingValues.insert(strEntry);
					}
				}
				entry = al_get_next_config_section(&iterator);
			}
			return ret;
		}

		/**
		 * @brief Saves the configuration file to the disk, with the filename
		 * that was given to the constructor. If the configuration file has
		 * no filename associated with it or the file cannot be saved, this throws.
		 * 
		 * @throws std::runtime_error on failure
		 */
		void saveToDisk() const {
			if(filename.empty()) {
				throw ConfigError("cannot save config to file: no filename given");
			}
			saveToDisk(filename);
		}

		/**
		 * @brief Saves the configuration file to the disk, with a given filename.
		 * If the file cannot be saved, this throws.
		 * 
		 * @throws std::runtime_error on failure
		 */
		void saveToDisk(const std::string& dstFilename) const {
			if(!al_save_config_file(dstFilename.c_str(), ptr())) {
				throw ConfigError("cannot save config file to %s", dstFilename.c_str());
			}
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

		/**
		 * @brief Clones the configuration structure.
		 * There is no `al_clone_config`, so this is done by merging
		 * with an empty config.
		 */
		[[nodiscard]] Config clone() const {
			return Merge(*this, Config());
		}
	private:
		std::string filename;
		std::string currentSection;
	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_CONFIG */
