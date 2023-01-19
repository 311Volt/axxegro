#include "axxegro/Exception.hpp"
#define AXXEGRO_TRUSTED

#include <axxegro/resources/Config.hpp>
#include <stdexcept>
#include <unordered_set>


al::Config::Config()
	: Resource(al_create_config())
{
	filename = "";
	currentSection = "";
}
al::Config::Config(const std::string& filename)
	: Resource(al_load_config_file(filename.c_str()))
{
	if(!ptr()) {
		throw ResourceLoadError("Cannot load config from \""+filename+"\" - file missing, corrupted or invalid");
	}
	currentSection = "";
}

al::Config::SectionSelector::SectionSelector(al::Config& cfg, const std::string& sectionName)
	: cfg(cfg)
{
	oldSectionName = cfg.getSelectedSectionName();
	cfg.selectSection(sectionName);
}

al::Config::SectionSelector::~SectionSelector()
{
	cfg.selectSection(oldSectionName);
}


void al::Config::selectSection(const std::string& name)
{
	currentSection = name;
}

const std::string& al::Config::getSelectedSectionName() const
{
	return currentSection;
}

void al::Config::addSection(const std::string& name)
{
	al_add_config_section(ptr(), name.c_str());
}

bool al::Config::removeSection(const std::string& name)
{
	return al_remove_config_section(ptr(), name.c_str());
}

bool al::Config::isPresent(const std::string& key) const
{
	return isPresent(currentSection, key);
}

bool al::Config::isPresent(const std::string& section, const std::string& key) const
{
	return al_get_config_value(ptr(), section.c_str(), key.c_str());
}

std::string al::Config::getValue(const std::string& key) const
{
	return getValue(currentSection, key);
}

std::string al::Config::getValue(const std::string& section, const std::string& key) const
{
	return {al_get_config_value(ptr(), section.c_str(), key.c_str())};
}

void al::Config::setValue(const std::string& key, const std::string& value)
{
	setValue(currentSection, key, value);
}

void al::Config::setValue(const std::string& section, const std::string& key, const std::string& value)
{
	al_set_config_value(ptr(), section.c_str(), key.c_str(), value.c_str());
}


void al::Config::addComment(const std::string& comment)
{
	addComment(currentSection, comment);
}
void al::Config::addComment(const std::string& section, const std::string& comment)
{
	al_add_config_comment(ptr(), section.c_str(), comment.c_str());
}

std::vector<std::string> al::Config::keys() const
{
	return keys(currentSection);
}

std::vector<std::string> al::Config::keys(const std::string& section) const
{
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
			ret.push_back({entry});
		}
		entry = al_get_next_config_entry(&iterator);
	}
	return ret;
}

std::vector<std::string> al::Config::sections() const
{
	ALLEGRO_CONFIG_SECTION* iterator;

	std::vector<std::string> ret = {""};
	std::unordered_set<std::string> existingValues;

	const char* entry = al_get_first_config_section(ptr(), &iterator);
	while(iterator) {
		if(entry) {
			std::string strEntry(entry);
			if(strEntry != "" && existingValues.count(strEntry)==0) {
				ret.push_back(strEntry);
				existingValues.insert(strEntry);
			}
		}
		entry = al_get_next_config_section(&iterator);
	}
	return ret;
}


void al::Config::saveToDisk() const
{
	if(filename == "") {
		throw ConfigError("cannot save config to file: no filename given");
	}
	saveToDisk(filename);
}

void al::Config::saveToDisk(const std::string& filename) const
{
	if(!al_save_config_file(filename.c_str(), ptr())) {
		throw ConfigError("cannot save config file to {}", filename);
	}
}

void al::Config::merge(const Config& other)
{
	al_merge_config_into(ptr(), other.ptr());
}
al::Config al::Config::Merge(const Config& master, const Config& source)
{
	ALLEGRO_CONFIG* newCfg = al_merge_config(master.ptr(), source.ptr());
	return Config(newCfg);
}
al::Config al::Config::clone()
{
	return Merge(*this, Config());
}
