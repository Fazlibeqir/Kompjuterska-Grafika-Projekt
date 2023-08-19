#include <iostream>
#include <vector>
#include <irrKlang.h>
#include <thread>
#include <chrono>
#include <string>
#include <Windows.h>
#include <algorithm>
#include <random>
using namespace irrklang;
using namespace std;
#pragma comment(lib, "irrKlang.lib")

int main()
{
    ISoundEngine* engine = createIrrKlangDevice();
    if (!engine) {
        cout << "Failed to create IrrKlang device." << endl;
        return 1;
    }
    string mediaPath = "media\\";

    vector<string> songList;
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

    for (size_t i = 0; i < songList.size(); ++i) {
        const string& song = songList[i];

        // Print a message indicating that the next song is loading
        if (i < songList.size() - 1) {
            cout << "Loading next song: " << songList[i] << endl;
        }
        ISound* sound = engine->play2D(song.c_str(), false, false, true);
        sound->setVolume(0.4f);
        while (!sound->isFinished()) {
            // Wait for the song to finish playing
            this_thread::sleep_for(chrono::milliseconds(100));
        }
        sound->drop();
    }

    engine->drop();
    return 0;
}
