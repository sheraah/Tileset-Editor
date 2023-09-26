#include "stdafx.h"
#include "AnyResourceManager.h"

void AnyResourceManager::forEachItem(std::function<void(const std::string& name)> func) const
{
	for (const std::string& name : m_insertedKeysInOrder)
	{
		func(name); // execute the function for every item in the map in the right order
	}
}

const std::vector<std::string>& AnyResourceManager::getKeysInOrder() const
{
	return m_insertedKeysInOrder; 
}

bool AnyResourceManager::keyExists(const std::string& key) const
{
	auto it = m_resources.find(key);
	return it != m_resources.end();
}
