#include <iostream>
#include "ressourceManager.hpp"

RessourceManager::RessourceManager()
{
    constexpr unsigned int nbLoadedTextures = 10;

    textures.reserve(nbLoadedTextures);
    textures.emplace_back(std::move(Texture("media/midna/midona_body.png")));
    //textures.emplace_back(std::move(Texture("media/crate.png")));
    textures.emplace_back(std::move(Texture("media/crate2.png")));
}

RessourceManager::~RessourceManager()
{

}

void RessourceManager::addFromFile(const char* filename)
{
    textures.emplace_back(std::move(Texture(filename)));
}
