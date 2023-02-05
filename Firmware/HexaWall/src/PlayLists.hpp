#pragma once

#ifndef _PLAYLISTFLOWS_H
#define _PLAYLISTFLOWS_H

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

//#include "SequenceManager.h"
#include "Playlist.hpp"

class PlayListFlows : public PlayList
{
public:
	PlayListFlows()
	{
		PlaylistItems = {
			SequenceName::Blendwave,
			SequenceName::TimedPalettes,
			SequenceName::Pride,
			SequenceName::StripedPalettes};
	}

	String getName() { return "Nice Flows"; }
};

class PlayListAll : public PlayList
{
public:
	PlayListAll()
	{
		PlaylistItems = {
			SequenceName::CyclePrimaryColors,
			SequenceName::CycleTertiaryColors,
			SequenceName::Pride,
			SequenceName::Dripping,
			SequenceName::TimedPalettes,
			SequenceName::Fire,
			SequenceName::Blendwave,
			SequenceName::Blur,
			SequenceName::Confetti,
			SequenceName::DotBeat,
			SequenceName::Easing,
			SequenceName::InoiseFire,
			SequenceName::Lightning,
			SequenceName::JugglePal,
			SequenceName::MatrixPal,
			SequenceName::Mover,
			SequenceName::Noise16a,
			SequenceName::Noise16b,
			SequenceName::Noise16c,
			SequenceName::OneSinePal,
			SequenceName::PaletteCrossfade,
			SequenceName::Plasma,
			SequenceName::RainbowMarch,
			SequenceName::RipplePal,
			SequenceName::Ripples,
			SequenceName::ThreeSinPal,
			SequenceName::TwoSinPal,
			SequenceName::SineLon,
			SequenceName::StripedPalettes,
			SequenceName::SimulatedTVLighting,
			SequenceName::RepeatingBlockPattern,
			SequenceName::Pacifica,
			SequenceName::RainbowBeat,
			SequenceName::FadeTest,
			SequenceName::GradientPalette,
			SequenceName::VertColumnLeftRight};
	}

	String getName() { return "All of Them"; }
};

class PlayListAllShort : public PlayList
{
public:
	PlayListAllShort()
	{
		PlaylistItems = {
			SequenceName::CyclePrimaryColors,
			SequenceName::CycleTertiaryColors,
			SequenceName::Pride,
			SequenceName::Dripping,
			SequenceName::TimedPalettes,
			SequenceName::Fire,
			SequenceName::Blendwave,
			SequenceName::Blur,
			SequenceName::Confetti,
			SequenceName::DotBeat,
			SequenceName::Easing,
			SequenceName::InoiseFire,
			SequenceName::Lightning,
			SequenceName::JugglePal,
			SequenceName::MatrixPal,
			SequenceName::Mover,
			SequenceName::Noise16a,
			SequenceName::Noise16b,
			SequenceName::Noise16c,
			SequenceName::OneSinePal,
			SequenceName::PaletteCrossfade,
			SequenceName::Plasma,
			SequenceName::RainbowMarch,
			SequenceName::RipplePal,
			SequenceName::Ripples,
			SequenceName::ThreeSinPal,
			SequenceName::TwoSinPal,
			SequenceName::SineLon,
			SequenceName::StripedPalettes,
			SequenceName::SimulatedTVLighting,
			SequenceName::RepeatingBlockPattern,
			SequenceName::Pacifica,
			SequenceName::RainbowBeat,
			SequenceName::FadeTest,
			SequenceName::GradientPalette,
			SequenceName::VertColumnLeftRight};

		setMinimumDurationSeconds(40);
		setMaximumDurationSeconds(60);
	}

	String getName() { return "All of Them (but short)"; }
};

#endif