/*
  ==============================================================================

	Synthesiser.h
	Created: 29 Dec 2018 7:22:08pm
	Author:  Mike

  ==============================================================================
*/
#include "JuceHeader.h"
#pragma once

class SynthesiserInherited : public Synthesiser {

public:
	SynthesiserVoice* findVoiceToSteal(SynthesiserSound* soundToPlay,
		int /*midiChannel*/, int midiNoteNumber) const override
	{
		// This voice-stealing algorithm applies the following heuristics:
		// - Re-use the oldest notes first
		// - Protect the lowest & topmost notes, even if sustained, but not if they've been released.

		// apparently you are trying to render audio without having any voices...
		jassert(!voices.isEmpty());

		// These are the voices we want to protect (ie: only steal if unavoidable)
		SynthesiserVoice* low = nullptr; // Lowest sounding note, might be sustained, but NOT in release phase
		SynthesiserVoice* top = nullptr; // Highest sounding note, might be sustained, but NOT in release phase

		// this is a list of voices we can steal, sorted by how long they've been running
		Array<SynthesiserVoice*> usableVoices;
		usableVoices.ensureStorageAllocated(voices.size());

		for (auto* voice : voices)
		{
			if (voice->canPlaySound(soundToPlay))
			{
				jassert(voice->isVoiceActive()); // We wouldn't be here otherwise

				usableVoices.add(voice);

				// NB: Using a functor rather than a lambda here due to scare-stories about
				// compilers generating code containing heap allocations..
				struct Sorter
				{
					bool operator() (const SynthesiserVoice* a, const SynthesiserVoice* b) const noexcept { return a->wasStartedBefore(*b); }
				};

				std::sort(usableVoices.begin(), usableVoices.end(), Sorter());

				if (!voice->isPlayingButReleased()) // Don't protect released notes
				{
					auto note = voice->getCurrentlyPlayingNote();

					if (low == nullptr || note < low->getCurrentlyPlayingNote())
						low = voice;

					if (top == nullptr || note > top->getCurrentlyPlayingNote())
						top = voice;
				}
			}
		}

		// Eliminate pathological cases (ie: only 1 note playing): we always give precedence to the lowest note(s)
		if (top == low)
			top = nullptr;

		// The oldest note that's playing with the target pitch is ideal..
		for (auto* voice : usableVoices)
			if (voice->getCurrentlyPlayingNote() == midiNoteNumber)
				return voice;

		// Oldest voice that has been released (no finger on it and not held by sustain pedal)
		for (auto* voice : usableVoices)
			//if (voice != low && voice != top && voice->isPlayingButReleased())
			if (voice->isPlayingButReleased())
				return voice;

		// Oldest voice that doesn't have a finger on it:
		for (auto* voice : usableVoices)
			//if (voice != low && voice != top && !voice->isKeyDown())
			if (voice->isKeyDown())
				return voice;

		// Oldest voice that isn't protected
		for (auto* voice : usableVoices)
			//if (voice != low && voice != top)
			//if (voice != low && voice != top)
			return voice;

		// We've only got "protected" voices now: lowest note takes priority
		//jassert(low != nullptr);

		// Duophonic synth: give priority to the bass note:
		//if (top != nullptr)
		//	return top;

		//return low;
	}
};