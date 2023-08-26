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
#include <mutex>

using namespace irrklang;
using namespace std;
#pragma comment(lib, "irrKlang.lib")

class Audio {
public:
    Audio();
    ~Audio();
    void setDefaultVolume(float newVolume);
    const std::string& getCurrentSongName() const;
    void playSong(const string& songName,const string &song);
   // void playCarEngineSound();
    bool pressed=false;
private:
    ISoundEngine* engine;
    vector<string> songList;
    thread songThread;
    ISound *currentSound;
    mutex songNameMutex;
    string currentSongName;

    static bool shouldStopPlaying;

    void SongPlaybackThread(ISoundEngine* engine);

};


#endif //KOMPJUTERSKA_GRAFIKA_PROJEKT_AUDIO_HPP
