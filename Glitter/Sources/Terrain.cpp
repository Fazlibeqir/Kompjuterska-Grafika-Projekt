//
// Created by Elena on 19.8.2023.
//

#include "Terrain.h"

Terrain:: Terrain(const string& terrainShaderVertexPath,
                  const string& terrainShaderFragmentPath,
                  const string& terrainModel1Path,
                  const string& terrainModel2Path)
                  : terrainShader(terrainShaderVertexPath.c_str(), terrainShaderFragmentPath.c_str()),
                    terrainModel1(terrainModel1Path), terrainModel2(terrainModel2Path){
}