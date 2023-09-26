#pragma once
#include <functional>
#include <any>

//a class that can store different types of data with their key values as string
class AnyResourceManager
{
public:
	//loads a resource with a lamba function that returns a resource to initialze the rosource under the key name
	template<typename T>
	bool load(const std::string& name, const T& value);
	//returns the element casted to T
	template<typename T>
	T& get(const std::string& name);
	//iterrates over the map and executes the provided lambda with thier name key value
	void forEachItem(std::function<void(const std::string& name)> func) const;
	//returns a vector of keys with the right order how they were inserted
	const std::vector<std::string>& getKeysInOrder() const;
private:
	//the actual resources stored using std::any
	std::unordered_map<std::string, std::any> m_resources;
	//keeps track of all keys inserted the first one will be [0] last one [size - 1]
	std::vector<std::string> m_insertedKeysInOrder;

private:
	//checks if the key exists
	bool keyExists(const std::string& key) const;
};

template<typename T>
inline bool AnyResourceManager::load(const std::string& name, const T& value)
{
	if (keyExists(name)) return false;

	m_resources[name] = value; //insert value at index name 

	m_insertedKeysInOrder.push_back(name);

	return true;
}

template<typename T>
inline T& AnyResourceManager::get(const std::string& name)
{
	try
	{
		return std::any_cast<T&>(m_resources[name]);
	}
	catch (const std::bad_any_cast& e) // if the cast wasnt successfull print a error message
	{
		std::cerr << "Bad any cast! Failed to get resource with key: " + name << std::endl;
	}
}
