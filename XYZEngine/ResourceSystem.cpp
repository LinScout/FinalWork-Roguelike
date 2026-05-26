#include "pch.h"
#include "ResourceSystem.h"
#include "Logger.h"
#include "EngineException.h"
#include <sstream>

namespace XYZEngine
{
	ResourceSystem* ResourceSystem::Instance()
	{
		static ResourceSystem resourceSystem;
		return &resourceSystem;
	}

	void ResourceSystem::LoadTexture(const std::string& name, std::string sourcePath, bool isSmooth)
	{
		if (textures.find(name) != textures.end())
		{
			LOG_WARN("Texture already loaded: " + name);
			return;
		}

		LOG_ASSERT(!name.empty(), "Texture name must not be empty");
		LOG_ASSERT(!sourcePath.empty(), "Texture path must not be empty");

		LOG_INFO("Loading texture: " + name + " from " + sourcePath);
		sf::Texture* newTexture = new sf::Texture();
		if (newTexture->loadFromFile(sourcePath))
		{
			newTexture->setSmooth(isSmooth);
			textures.emplace(name, newTexture);
			LOG_INFO("Texture loaded: " + name);
		}
		else
		{
			delete newTexture;
			LOG_ERROR("Failed to load texture: " + name);
			throw ResourceLoadException(name, sourcePath);
		}
	}
	const sf::Texture* ResourceSystem::GetTextureShared(const std::string& name) const
	{
		auto it = textures.find(name);
		if (it == textures.end())
		{
			LOG_ERROR("Texture not found: " + name);
			throw EngineException("Texture not found: " + name);
		}
		return it->second;
	}
	sf::Texture* ResourceSystem::GetTextureCopy(const std::string& name) const
	{
		auto it = textures.find(name);
		if (it == textures.end())
		{
			LOG_ERROR("Texture not found: " + name);
			throw EngineException("Texture not found: " + name);
		}
		return new sf::Texture(*it->second);
	}
	void ResourceSystem::DeleteSharedTexture(const std::string& name)
	{
		auto texturePair = textures.find(name);
		if (texturePair == textures.end())
		{
			LOG_WARN("Attempt to delete unknown texture: " + name);
			return;
		}

		sf::Texture* deletingTexure = texturePair->second;
		textures.erase(texturePair);
		delete deletingTexure;
	}

	void ResourceSystem::LoadTextureMap(const std::string& name, std::string sourcePath, sf::Vector2u elementPixelSize, int totalElements, bool isSmooth)
	{
		if (textureMaps.find(name) != textureMaps.end())
		{
			LOG_WARN("Texture map already loaded: " + name);
			return;
		}

		LOG_INFO("Loading texture map: " + name + " from " + sourcePath);
		sf::Texture textureMap;
		if (textureMap.loadFromFile(sourcePath))
		{
			std::vector<sf::Texture*> textureMapElements;
			if (totalElements > 0) textureMapElements.reserve(static_cast<size_t>(totalElements));

			auto textureSize = textureMap.getSize();
			int loadedElements = 0;

			for (int y = 0; y <= static_cast<int>(textureSize.y) - static_cast<int>(elementPixelSize.y); y += static_cast<int>(elementPixelSize.y))
			{
				if (loadedElements == totalElements)
				{
					break;
				}

				for (int x = 0; x <= static_cast<int>(textureSize.x) - static_cast<int>(elementPixelSize.x); x += static_cast<int>(elementPixelSize.x))
				{
					if (loadedElements == totalElements)
					{
						break;
					}

					sf::Texture* newTextureMapElement = new sf::Texture();
					if (newTextureMapElement->loadFromFile(sourcePath, sf::IntRect(x, y, elementPixelSize.x, elementPixelSize.y)))
					{
						newTextureMapElement->setSmooth(isSmooth);
						textureMapElements.push_back(newTextureMapElement);
						++loadedElements;
					}
					else
					{
						LOG_WARN("Failed to load texture map element at rect (" + std::to_string(x) + "," + std::to_string(y) + ") for " + name);
						delete newTextureMapElement;
					}
				}
			}

			if (loadedElements == 0)
			{
				LOG_ERROR("No elements loaded for texture map: " + name);
				// clean up just in case (vector should be empty, but be defensive)
				for (auto t : textureMapElements) delete t;
				throw ResourceLoadException(name, sourcePath);
			}

			textureMaps.emplace(name, std::move(textureMapElements));
			LOG_INFO("Texture map loaded: " + name + " (elements: " + std::to_string(loadedElements) + ")");
		}
		else
		{
			LOG_ERROR("Failed to load texture map: " + name);
			throw ResourceLoadException(name, sourcePath);
		}
	}
	const sf::Texture* ResourceSystem::GetTextureMapElementShared(const std::string& name, int elementIndex) const
	{
		auto textureMap = textureMaps.find(name);
		if (textureMap == textureMaps.end())
		{
			LOG_ERROR("Texture map not found: " + name);
			throw EngineException("Texture map not found: " + name);
		}

		auto& texturesVec = textureMap->second;
		if (elementIndex < 0 || elementIndex >= static_cast<int>(texturesVec.size()))
		{
			LOG_ERROR("Texture map element index out of range: " + name);
			throw EngineException("Texture map element index out of range");
		}

		return texturesVec[elementIndex];
	}
	sf::Texture* ResourceSystem::GetTextureMapElementCopy(const std::string& name, int elementIndex) const
	{
		auto textureMapIt = textureMaps.find(name);
		if (textureMapIt == textureMaps.end())
		{
			LOG_ERROR("Texture map not found: " + name);
			throw EngineException("Texture map not found: " + name);
		}
		auto& texturesVec = textureMapIt->second;
		if (elementIndex < 0 || elementIndex >= static_cast<int>(texturesVec.size()))
		{
			LOG_ERROR("Texture map element index out of range: " + name);
			throw EngineException("Texture map element index out of range");
		}
		return new sf::Texture(*texturesVec[elementIndex]);
	}
	int ResourceSystem::GetTextureMapElementsCount(const std::string& name) const
	{
		auto textureMapIt = textureMaps.find(name);
		if (textureMapIt == textureMaps.end())
		{
			LOG_ERROR("Texture map not found: " + name);
			throw EngineException("Texture map not found: " + name);
		}
		return static_cast<int>(textureMapIt->second.size());
	}
	void ResourceSystem::DeleteSharedTextureMap(const std::string& name)
	{
		auto textureMapIt = textureMaps.find(name);
		if (textureMapIt == textureMaps.end())
		{
			LOG_WARN("Attempt to delete unknown texture map: " + name);
			return;
		}

		auto& deletingTextures = textureMapIt->second;

		for (size_t i = 0; i < deletingTextures.size(); i++)
		{
			delete deletingTextures[i];
		}

		textureMaps.erase(textureMapIt);
	}

	void ResourceSystem::LoadSound(const std::string& name, std::string sourcePath)
	{
		if (sounds.find(name) != sounds.end())
		{
			LOG_WARN("Sound already loaded: " + name);
			return;
		}

		LOG_INFO("Loading sound: " + name + " from " + sourcePath);
		sf::SoundBuffer* newSound = new sf::SoundBuffer();
		if (newSound->loadFromFile(sourcePath))
		{
			sounds.emplace(name, newSound);
			LOG_INFO("Sound loaded: " + name);
		}
		else
		{
			delete newSound;
			LOG_ERROR("Failed to load sound: " + name);
			throw ResourceLoadException(name, sourcePath);
		}
	}
	const sf::SoundBuffer* ResourceSystem::GetSound(const std::string& name) const
	{
		auto it = sounds.find(name);
		if (it == sounds.end())
		{
			LOG_ERROR("Sound not found: " + name);
			throw EngineException("Sound not found: " + name);
		}
		return it->second;
	}
	void ResourceSystem::DeleteSound(const std::string& name)
	{
		auto soundPair = sounds.find(name);
		if (soundPair == sounds.end())
		{
			LOG_WARN("Attempt to delete unknown sound: " + name);
			return;
		}

		sf::SoundBuffer* deletingSound = soundPair->second;
		sounds.erase(soundPair);
		delete deletingSound;
	}

	void ResourceSystem::Clear()
	{
		DeleteAllTextures();
		DeleteAllTextureMaps();
		DeleteAllSounds();
	}

	void ResourceSystem::DeleteAllTextures()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& texturePair : textures)
		{
			keysToDelete.push_back(texturePair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTexture(key);
		}
	}
	void ResourceSystem::DeleteAllTextureMaps()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& textureMapPair : textureMaps)
		{
			keysToDelete.push_back(textureMapPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSharedTextureMap(key);
		}
	}

	void ResourceSystem::DeleteAllSounds()
	{
		std::vector<std::string> keysToDelete;

		for (const auto& soundPair : sounds)
		{
			keysToDelete.push_back(soundPair.first);
		}

		for (const auto& key : keysToDelete)
		{
			DeleteSound(key);
		}
	}
}