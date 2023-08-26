//
// Created by Beqir on 22/08/2023.
//

#include "Audio.hpp"

bool Audio::shouldStopPlaying = false;
// Initialize the static member variable
Audio::Audio(): currentSound(nullptr){
    engine = createIrrKlangDevice();
    if (!engine) {
        cout << "Failed to create IrrKlang device." << endl;
    }
    //string(SHADER_DIR)+"\\car.vert";
    string mediaPath = string(AUDIO_DIR)+"\\";

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((mediaPath + "*.ogg").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            songList.push_back(mediaPath + findFileData.cFileName);
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(songList.begin(), songList.end(), g);

  songThread=thread(&Audio::SongPlaybackThread,this,engine);
};
Audio::~Audio(){
    if (engine) {
        engine->drop();
    }
    if (songThread.joinable()) {
        songThread.join();
    }
}

void Audio::setDefaultVolume(float newVolume) {
   if(currentSound){
       currentSound->setVolume(newVolume);
   }
}
const std::string& Audio::getCurrentSongName() const {
    return currentSongName;
}
//void Audio::playCarEngineSound() {
//    // Replace 'CarEngine' with the appropriate name of your car engine sound file
//    const string carEngineSoundPath = string(ASSETS_DIR) + "\\engineSound\\carEngine.ogg";
//
//    // Play the car engine sound with the specified volume
//    sound1 = engine->play2D(carEngineSoundPath.c_str(), true, false, true);
//    sound1->setVolume(defaultVolume);
//
//    thread carEngineThread = thread([this]() {
//        while (pressed) {
//            // This loop will continuously play the car engine sound
//            // until the up arrow key is released
//        }
//
//        // Stop the sound and release resources when the loop exits
//        sound1->stop();
//        sound1->drop();
//        sound1 = nullptr;
//    });
//}
void Audio::playSong(const string& songName,const string &song) {
    lock_guard<mutex> lock(songNameMutex);
    currentSongName = songName;
    currentSound = engine->play2D(song.c_str(), false, false, true);
}
void Audio::SongPlaybackThread(ISoundEngine* engine) {
    bool shouldContinuePlaying = true;

    while (shouldContinuePlaying) {
        for (const string &song: songList) {
            if (shouldStopPlaying) {
                shouldContinuePlaying = false;
                break;  // Exit the loop if we need to stop playing
            }
            size_t lastSlash = song.find_last_of("/\\");
            string songName = (lastSlash != string::npos) ? song.substr(lastSlash + 1) : song;

            size_t extensionPos = songName.find_last_of('.');
            if (extensionPos != string::npos && extensionPos > 0) {
                songName = songName.substr(0, extensionPos);
            }
            cout << "Playing song: " << songName << endl;
            playSong(songName,song);

            while (!currentSound->isFinished()) {
                if (shouldStopPlaying) {
                    currentSound->stop();  // Stop the song if we need to stop playing
                    break;
                }
                this_thread::sleep_for(chrono::milliseconds(100));
            }
            currentSound->drop();
        }
    }
}