#ifndef _RESSOURCE_MANAGER_H_ 
#define _RESSOURCE_MANAGER_H_

#include <vector>
#include "texture.h"

struct RessourceManager
{
public:
    std::vector<Texture> textures;

public:
    RessourceManager();
    ~RessourceManager();
    void addFromFile(const char* filename);
};


#endif 