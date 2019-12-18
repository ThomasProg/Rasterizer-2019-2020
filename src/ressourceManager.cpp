#include <iostream>
#include "ressourceManager.hpp"

RessourceManager::RessourceManager()
{
    constexpr unsigned int nbLoadedTextures = 3;

    textures.reserve(nbLoadedTextures);
    textures.emplace_back(std::move(Texture("media/midna/midona_body.png")));
    textures.emplace_back(std::move(Texture("media/crate.png")));
    textures.emplace_back(std::move(Texture("media/crate2.png")));
    textures.emplace_back(std::move(Texture("media/snowman/kardanadam.png")));
    textures.emplace_back(std::move(Texture("media/WaddleDeeLow-Poly/waddledee.png")));
    textures.emplace_back(std::move(Texture("media/SuperMarioGalaxyBoo/boo1.png")));
    textures.emplace_back(std::move(Texture("media/SuperMarioGalaxyBoo/boo2.png")));
    textures.emplace_back(std::move(Texture("media/SuperMarioGalaxyBoo/boo3.png")));
    textures.emplace_back(std::move(Texture("media/SuperMarioGalaxyBoo/boo4.png")));
}

RessourceManager::~RessourceManager()
{

}

void RessourceManager::addFromFile(const char* filename)
{
    textures.emplace_back(std::move(Texture(filename)));
}
