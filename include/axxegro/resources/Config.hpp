#ifndef INCLUDE_AXXEGRO_RESOURCES_CONFIG
#define INCLUDE_AXXEGRO_RESOURCES_CONFIG

#include <allegro5/allegro5.h>
#include <string>
#include <vector>
#include <memory>

#include <axxegro/resources/Resource.hpp>

#include <unordered_map>

/**
 * @file
 * al::Config wraps around ALLEGRO_CONFIG and extends it with a
 * section selection mechanism.
 */

namespace al {

	class ConfigDeleter {
	public:
		void operator()(ALLEGRO_CONFIG* ptr){al_destroy_config(ptr);}
	};

	class Config: public Resource {
	public:
		Config();
		Config(ALLEGRO_CONFIG* cfg);
		Config(const std::string& filename);

		/**
		 * @brief Provides a RAII-style mechanism for selecting a section to prevent
		 * the user from forgetting to set it back to the old value.
		 */
		struct SectionSelector {
			///@brief Calls cfg.selectSection(sectionName).
			SectionSelector(Config& cfg, const std::string& sectionName);

			///@brief Restores the section selection from before the construction of the object.
			~SectionSelector();
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
		 * It's easy to forget to change the section back so using this
		 * method directly is a bug waiting to happen. Prefer using
		 * SectionSelector instead.
		 */
		void selectSection(const std::string& name);

		///@returns Returns the name of the currently selected section.
		const std::string& getSelectedSectionName() const;

		///@brief Adds a section. If it already exists, nothing happens.
		void addSection(const std::string& name);

		/**
		 * @brief Removes a section and all its associated key-value pairs.
		 * 
		 * @return true if the section existed.
		 * @return false if the section did not exist.
		 */
		bool removeSection(const std::string& name);

		///@brief Checks whether a key exists in the currently selected section.
		bool isPresent(const std::string& key) const;

		///@brief Checks whether a key exists.
		bool isPresent(const std::string& section, const std::string& key) const;

		/**
		 * @brief Returns the value associated with a given key in the currently selected section.
		 * Throws if the key does not exist.
		 * 
		 * @throws std::runtime_error if the key did not exist.
		 */
		std::string getValue(const std::string& key) const;

		/**
		 * @brief Returns the value associated with a given key in the currently selected section.
		 * Throws if the key does not exist.
		 * 
		 * @throws std::runtime_error if the key did not exist.
		 */
		std::string getValue(const std::string& section, const std::string& key) const;

		/**
		 * @brief Sets the value for a given key in the currently selected section. 
		 * If the key does not exist, it is created automatically.
		 */
		void setValue(const std::string& key, const std::string& value);

		/**
		 * @brief Sets the value for a given key.
		 * If the key does not exist, it is created automatically.
		 */
		void setValue(const std::string& section, const std::string& key, const std::string& value);

		/**
		 * @brief Adds a comment in the current section.
		 */
		void addComment(const std::string& comment);

		/**
		 * @brief Adds a comment in a given section.
		 */
		void addComment(const std::string& section, const std::string& comment);

		///@returns A list of all keys in the currently selected section.
		std::vector<std::string> keys() const;

		///@returns A list of all keys in a given section.
		std::vector<std::string> keys(const std::string& section) const;

		/**
		 * @returns A list of all section names in the configuration file, including the global section ("").
		 * The return of an empty string at the beginning is guaranteed, unlike in Allegro5.
		 */
		std::vector<std::string> sections() const;

		/**
		 * @brief Saves the configuration file to the disk, with the filename
		 * that was given to the constructor. If the configuration file has
		 * no filename associated with it or the file cannot be saved, this throws.
		 * 
		 * @throws std::runtime_error on failure
		 */
		void saveToDisk() const;

		/**
		 * @brief Saves the configuration file to the disk, with a given filename.
		 * If the file cannot be saved, this throws.
		 * 
		 * @throws std::runtime_error on failure
		 */
		void saveToDisk(const std::string& filename) const;

		/**
		 * @brief Merges two configs, with values in `other` overriding those in `*this`.
		 */
		void merge(const Config& other);

		
		/**
		 * @brief Merges two configs, with values in `source` overriding those in `master`.
		 */
		static Config Merge(const Config& master, const Config& source);

		/**
		 * @brief Clones the configuration structure.
		 * There is no `al_clone_config`, so this is done by merging
		 * with an empty config.
		 */
		Config clone();

		ALLEGRO_CONFIG* alPtr();

	#ifdef AXXEGRO_TRUSTED
		ALLEGRO_CONFIG* alPtr() const {return cfg.get();};
	#endif

	private:
		std::string filename;
		std::string currentSection;
		std::unique_ptr<ALLEGRO_CONFIG, ConfigDeleter> cfg;
	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_CONFIG */
