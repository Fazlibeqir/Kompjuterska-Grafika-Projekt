//
// Created by Beqir on 24/08/2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_SCORE_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_SCORE_HPP

#include <iostream>
#include <fstream>

class Score {
public:
    Score(const std::string& filename) : filename(filename) {}
    float readHighScore() {
        float highScore = 0.0f;
        std::ifstream inputFile(filename);
        if (inputFile.is_open()) {
            inputFile >> highScore;
            inputFile.close();
        }
        return highScore;
    }
    void updateHighScore(float newTime) {
        float currentHighScore = readHighScore();
        if (newTime > currentHighScore) {
            std::ofstream outputFile(filename);
            if (outputFile.is_open()) {
                outputFile << newTime;
                outputFile.close();
            }
        }
    }
private:
    std::string filename;
};
#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_SCORE_HPP
