#ifndef INCLUDE_AXXEGRO_RESOURCES_CONFIG
#define INCLUDE_AXXEGRO_RESOURCES_CONFIG

#include <allegro5/allegro5.h>
#include <string>
#include <vector>

#include <unordered_map>

/**
 * @file
 * al::Config wraps around ALLEGRO_CONFIG and extends it with a
 * section selection mechanism.
 */

namespace al {
	class Config {
	public:
		Config();
		Config(const std::string& filename);
		~Config();

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

		///@returns A list of all keys in the currently selected section.
		std::vector<std::string> keys() const;

		///@returns A list of all keys in a given section.
		std::vector<std::string> keys(const std::string& section) const;

		/**
		 * @returns A list of all section names in the configuration file, including the global section ("").
		 * The return of an empty string at the beginning is guaranteed, unlike in Allegro5
		 * ()
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
	private:
		std::string filename;
		std::string currentSection;
		ALLEGRO_CONFIG* ptrConfig;
	};
}

#endif /* INCLUDE_AXXEGRO_RESOURCES_CONFIG */
