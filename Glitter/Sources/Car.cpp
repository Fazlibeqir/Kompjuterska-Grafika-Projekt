//
// Created by Elena on 19.8.2023.
//

#include "Car.h"

Car::Car(const string& carShaderVertexPath,
         const string& carShaderFragmentPath,
         const string& carModelPath,
         const string& tyre1ModelPath,
         const string& tyre2ModelPath) :
            carShader(carShaderVertexPath.c_str(), carShaderFragmentPath.c_str()),
            carModel(carModelPath),
            tyre1Model(tyre1ModelPath),
            tyre2Model(tyre2ModelPath){
}
