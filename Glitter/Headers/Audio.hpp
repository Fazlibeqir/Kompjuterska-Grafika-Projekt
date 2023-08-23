//
// Created by Beqir on 22/08/2023.
//

#ifndef KOMPJUTERSKA_GRAFIKA_PROJEKT_AUDIO_HPP
#define KOMPJUTERSKA_GRAFIKA_PROJEKT_AUDIO_HPP
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>
#include <irrKlang.h>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <random>
using namespace irrklang;
using namespace std;
#pragma comment(lib, "irrKlang.lib")

class Audio {
public:
    Audio();
    ~Audio();
    void setDefaultVolume(float newVolume);
private:
    ISoundEngine* engine;
    vector<string> songList;
    std::thread songThread;
    ISound *currentSound;
    static bool shouldStopPlaying;
    static float defaultVolume;
    void SongPlaybackThread(ISoundEngine* engine);

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_AUDIO_HPP
