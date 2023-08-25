//
// Created by Elena on 19.8.2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_CAR_H
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_CAR_H


#include "shader.hpp"
#include "Model.hpp"

class Car {
public:
    Shader carShader;
    Model carModel;
    Model tyre1Model;
    Model tyre2Model;

    Car(const string& carShaderVertexPath,
             const string& carShaderFragmentPath,
             const string& carModelPath,
             const string& tyre1ModelPath,
             const string& tyre2ModelPath);
};

#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_CAR_H
