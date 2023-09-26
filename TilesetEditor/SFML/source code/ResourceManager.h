#pragma once
#include <functional>
#include <assert.h>

//this class allows you to store any data with any name
template<typename T>
class ResourceManager
{
public:
	//default contructor
	ResourceManager();
	//destructor that deletes all resource pointers
	~ResourceManager();
	//loads a resoruce with the value of the resource is passed into a lambda function where the user can modify it
	bool load(const std::string& name, const std::function<void(T& res)>& func);
	//loads a resource with a lamba function that returns a resource to initialze the rosource under the key name
	bool load(const std::string& name, const T& value);
	//this function is used when you want a base class pointer to point to another dirived class
	template<typename Child>
	bool load(const std::string& name, const std::function<Child()>& func);
	//this function iterrates over all resources and executes the lambda function that takes in the resource reference
	void iterResources(const std::function<void(T& Resource)>& func);
	//this function iterrates over all resources and executes the lambda function 
	//that takes in the name of the resource and the resource reference
	void iterResources(const std::function<void(const std::string& name, T& Resource)>& func);
	//erasees the value stored under Name as a key
	void erase(const std::string& name);
	//returns the value stored under name as a key
	T& get(const std::string& name);
	//returns get(name)
	T& operator[](const std::string& name);
	//returns the size of the resource map
	size_t size() const;
	//checks if the key exists
	bool keyExists(const std::string& key);
private:
	//the actual map used to store a name as a key and T*
	std::unordered_map<std::string, T*> m_resources;
};


template<typename T>
inline ResourceManager<T>::ResourceManager()
{

}

template<typename T>
inline ResourceManager<T>::~ResourceManager()
{
	for (const auto& [Name, Resource] : m_resources) delete Resource;
}

template<typename T>
inline bool ResourceManager<T>::keyExists(const std::string& key)
{
	auto it = m_resources.find(key);
	return it != m_resources.end();
}

template<typename T>
inline bool ResourceManager<T>::load(const std::string& name, const std::function<void(T& res)>& func)
{
	if (keyExists(name)) return false;

	m_resources[name] = new T();

	func(*m_resources[name]);

	return true;
}

template<typename T>
inline bool ResourceManager<T>::load(const std::string& name, const T& value)
{
	if (keyExists(name)) return false;

	m_resources[name] = new T(value);

	return true;
}

template<typename T>
template<typename Child>
inline bool ResourceManager<T>::load(const std::string& name, const std::function<Child()>& func)
{
	if (keyExists(name)) return false;
	//make create a child class while using T as the base class
	m_resources[name] = new Child(func());

	return true;
}

template<typename T>
inline void ResourceManager<T>::iterResources(const std::function<void(T& Resource)>& func)
{
	for (auto& [name, Res] : m_resources) func(*Res);
}

template<typename T>
inline void ResourceManager<T>::iterResources(const std::function<void(const std::string& name, T& Resource)>& func)
{
	for (auto& [name, Res] : m_resources) func(name, *Res);
}

template<typename T>
inline void ResourceManager<T>::erase(const std::string& name)
{
	m_resources.erase(name);
}

template<typename T>
T& ResourceManager<T>::get(const std::string& name)
{
	if (m_resources[name] == nullptr)
	{
		std::cerr << "ResourceManager.h failed to get resource with key: " + name << std::endl;
	}

	return *m_resources[name];
}

template<typename T>
T& ResourceManager<T>::operator[](const std::string& name)
{
	return get(name);
}

template<typename T>
inline size_t ResourceManager<T>::size() const
{
	return m_resources.size();
}
