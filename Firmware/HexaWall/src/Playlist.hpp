// Playlist.h

#ifndef _PLAYLIST_h
#define _PLAYLIST_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "SequenceNames.hpp"
#include <memory>
#include <vector>

using namespace std;

#define DEFAULT_MIN_DURATION 60
#define DEFAULT_MAX_DURATION 240

class PlayList
{
private:
	int _currentPlaylistItemIndex = 999;	//this makes sure the list is randomized after loading

protected:
	std::vector<SequenceName> PlaylistItems;
	int _minimumDurationSeconds = DEFAULT_MIN_DURATION;
	int _maximumDurationSeconds = DEFAULT_MAX_DURATION;

	void randomizeList();

public:
	PlayList();

	SequenceName getNextSequence();
	int getSequenceCount();

	void setMinimumDurationSeconds(int value) { _minimumDurationSeconds = value; }
	void setMaximumDurationSeconds(int value) { _maximumDurationSeconds = value; }

	int getMinimumDurationSeconds() { return _minimumDurationSeconds; }
	int getMaximumDurationSeconds() { return _maximumDurationSeconds; }

	// pure virtual 'abstract' methods
	virtual String getName() = 0;
};

#endif
