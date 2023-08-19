//
// Created by Elena on 19.8.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_TERRAIN_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_TERRAIN_H


#include "shader.hpp"
#include "Model.hpp"

class Terrain {
public:
    Shader terrainShader;
    Model terrainModel1;
    Model terrainModel2;

    Terrain(const string& terrainShaderVertexPath,
            const string& terrainShaderFragmentPath,
            const string& terrainModel1Path,
            const string& terrainModel2Path);
};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_TERRAIN_H
