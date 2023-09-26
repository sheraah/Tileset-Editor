#pragma once
#include "AnyResourceManager.h"
#include <fstream>

class Settings
{
public:
	//saves the settings
	static void save(const std::string& filePath);
	//loads the settings
	static void load(const std::string& filePath);
	//returns the setting with this name and casts it to to T
	template<typename T>
	static T& get(const std::string& name);
	//sets the settings with the name to that value
	template<typename T>
	static void set(const std::string& name, const T& value);

private:
	//the actual settings stored as a hashmap of std::any
	inline static AnyResourceManager m_settings;

private:
	//staic instance(Singleton)
	static Settings& getInstance();
	//loads the current line containing the name and the type value after it
	template<typename T>
	void loadStr(std::ifstream& in);
	//takes in all types of the types that the file contains as template arguments in the right order to read them in
	template<typename... Types>
	void loadFromFile(std::ifstream& in);
	//tkae in all types of the file and saves their name with their value
	template<typename... Types>
	void saveToFile(std::ofstream& out);
	//saves the string to the provided file name and then value
	template<typename T> 
	void saveStr(const std::string& name, std::ofstream& out);
};

template<typename T>
inline T& Settings::get(const std::string& name)
{
	return m_settings.get<T>(name);
}

template<typename T>
inline void Settings::set(const std::string& name, const T& value)
{
	T& element = get<T>(name);

	element = value;
}

template<typename T>
inline void Settings::loadStr(std::ifstream& in)
{
	std::string name;
	T value;

	in >> name >> value;

	m_settings.load(name, value);
}

template<typename ...Types>
inline void Settings::loadFromFile(std::ifstream& in)
{
	(loadStr<Types>(in), ...);
}

template<typename... Types>
inline void Settings::saveToFile(std::ofstream& out)
{
	size_t index = 0;

	try
	{
		(saveStr<Types>(m_settings.getKeysInOrder()[index++], out), ...);
	}
	catch(const std::out_of_range& e)
	{
		std::cerr << "index out of range in Settings.h\n";
	}
}

template<typename T>
inline void Settings::saveStr(const std::string& name, std::ofstream& out)
{
	out << name << ' ' << get<T>(name) << '\n';
}
