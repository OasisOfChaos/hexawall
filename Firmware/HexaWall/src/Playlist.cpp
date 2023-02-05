#include "Playlist.hpp"
#include <random>

PlayList::PlayList()
{
}

// void PlayList::Test()
// {
//     Serial.println("Test():");
//     for (SequenceName x : PlaylistItems)
//     {
//         Serial.printf("%s, ", x._to_string());
//     }
//     Serial.print("\n\n");
// }

void PlayList::randomizeList()
{
    unsigned num = esp_random();
    std::shuffle(PlaylistItems.begin(), PlaylistItems.end(), default_random_engine(num));
    //Serial.println("RANDOMIZING LIST");
}

SequenceName PlayList::getNextSequence()
{
    if (_currentPlaylistItemIndex > PlaylistItems.capacity() - 1)
    {
        randomizeList();
        _currentPlaylistItemIndex = 0;
    }

    return PlaylistItems[_currentPlaylistItemIndex++];
}

int PlayList::getSequenceCount()
{
    return PlaylistItems.capacity();
}

