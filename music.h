#pragma once

#include <Arduino.h>

#define Cn 0  // C  (C normal)
#define Cs 1  // C# (C sharp)
#define Df Cs // Db (D flat)
#define Dn 2  // D  (D normal)
#define Ds 3  // D# (D sharp)
#define Ef Ds // Eb (E flat)
#define En 4  // E  (E normal)
#define Fn 5  // F  (F normal)
#define Fs 6  // F# (F sharp)
#define Gf Fs // Gb (G flat)
#define Gn 7  // G  (G normal)
#define Gs 8  // G# (G sharp)
#define Af Gs // Ab (A flat)
#define An 9  // A  (A normal)
#define As 10 // A# (A sharp)
#define Bf As // Bb (B flat)
#define Bn 11 // B  (B normal)
#define SILENCE 12
#define NOTE_COUNT 12
#define OCTAVE_MASK 0xF000
#define NOTE_MASK 0x0F00
#define TIMING_MASK 0x00FF
#define N(note, octave, timing) ((octave << 12) | (note << 8) | timing)
#define PAUSE(timing) N(SILENCE, SILENCE, timing)
#define END N(SILENCE, SILENCE, 0)
#define NO_TONE PAUSE(0)
#define TIMING_UNIT_MS 60
#define A4_FREQUENCY 440.0
#define MIN_OCTAVE 1
#define MAX_OCTAVE 8
#define OCTAVE_NOTES(octave, timing) \
    N(Cn, octave, timing),           \
        N(Cs, octave, timing),       \
        N(Dn, octave, timing),       \
        N(Ds, octave, timing),       \
        N(En, octave, timing),       \
        N(Fn, octave, timing),       \
        N(Fs, octave, timing),       \
        N(Gn, octave, timing),       \
        N(Gs, octave, timing),       \
        N(An, octave, timing),       \
        N(As, octave, timing),       \
        N(Bn, octave, timing)
#define ALL_NOTES(timing)                       \
    OCTAVE_NOTES(1, timing), PAUSE(timing),     \
        OCTAVE_NOTES(2, timing), PAUSE(timing), \
        OCTAVE_NOTES(3, timing), PAUSE(timing), \
        OCTAVE_NOTES(4, timing), PAUSE(timing), \
        OCTAVE_NOTES(5, timing), PAUSE(timing), \
        OCTAVE_NOTES(6, timing), PAUSE(timing), \
        OCTAVE_NOTES(7, timing), PAUSE(timing), \
        OCTAVE_NOTES(8, timing), PAUSE(timing)

const char *Notes[] = {"C", "Cs", "D", "Ds", "E", "F", "Fs", "G", "Gs", "A", "As", "B"};

// const uint16_t TestTheme[] PROGMEM = {ALL_NOTES(6)};
// const uint16_t TestThemeLength = sizeof(TestTheme) / sizeof(TestTheme[0]);

const uint16_t TwinkleTwinkle[] PROGMEM = {
    N(Cn, 4, 10), N(Cn, 4, 10), N(Gn, 4, 10), N(Gn, 4, 10), N(An, 4, 10), N(An, 4, 10), N(Gn, 4, 20),
    N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 10), N(Dn, 4, 10), N(Cn, 4, 20),
    N(Gn, 4, 10), N(Gn, 4, 10), N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 20),
    N(Gn, 4, 10), N(Gn, 4, 10), N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 20),
    N(Cn, 4, 10), N(Cn, 4, 10), N(Gn, 4, 10), N(Gn, 4, 10), N(An, 4, 10), N(An, 4, 10), N(Gn, 4, 20),
    N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 10), N(Dn, 4, 10), N(Cn, 4, 20), END};
const uint16_t MaryHadALittleLamb[] PROGMEM = {
    N(En, 4, 10), N(Dn, 4, 10), N(Cn, 4, 10), N(Dn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(En, 4, 20),
    N(Dn, 4, 10), N(Dn, 4, 10), N(Dn, 4, 20),
    N(En, 4, 10), N(Gn, 4, 10), N(Gn, 4, 20),
    N(En, 4, 10), N(Dn, 4, 10), N(Cn, 4, 10), N(Dn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(En, 4, 10),
    N(Dn, 4, 10), N(Dn, 4, 10), N(En, 4, 10), N(Dn, 4, 10), N(Cn, 4, 10), N(Dn, 4, 10), N(En, 4, 20),
    N(Dn, 4, 10), N(Dn, 4, 10), N(En, 4, 10), N(Dn, 4, 10), N(Cn, 4, 10), N(Dn, 4, 10), N(En, 4, 20), END};
const uint16_t MerryXmas[] PROGMEM = {
    N(Dn, 6, 16),
    N(Gn, 6, 16), N(Gn, 6, 8), N(An, 6, 8), N(Gn, 6, 8), N(Fs, 6, 8),
    N(En, 6, 16), N(En, 6, 16), N(En, 6, 16),
    N(An, 6, 16), N(An, 6, 8), N(Bn, 6, 8), N(An, 6, 8), N(Gn, 6, 8),
    N(Fs, 6, 16), N(Dn, 6, 16), N(Dn, 6, 16),
    N(Bn, 6, 16), N(Bn, 6, 8), N(Cn, 7, 8), N(Bn, 6, 8), N(An, 6, 8),
    N(Gn, 6, 16), N(En, 6, 16), N(En, 6, 8), N(En, 6, 8),
    N(En, 6, 16), N(An, 6, 16), N(Fs, 6, 16),
    N(Gn, 6, 32),

    N(Dn, 6, 16),
    N(Gn, 6, 16), N(Gn, 6, 16), N(Gn, 6, 16),
    N(Fs, 6, 32), N(Fs, 6, 16),
    N(Gn, 6, 16), N(Fs, 6, 16), N(En, 6, 16),
    N(Dn, 6, 32), N(Bn, 6, 16),
    N(Cn, 7, 16), N(Bn, 6, 16), N(An, 6, 16),
    N(Dn, 7, 16), N(Dn, 6, 16), N(Dn, 6, 8), N(Dn, 6, 8),
    N(En, 6, 16), N(An, 6, 16), N(Fs, 6, 16),
    N(Gn, 6, 32), PAUSE(16),
    0};
const uint16_t JollyGoodFellow[] PROGMEM = {
    N(Dn, 7, 8),
    N(Bn, 6, 16), N(Bn, 6, 8), N(Bn, 6, 8), N(An, 6, 8), N(Bn, 6, 8),
    N(Cn, 7, 24), N(Bn, 6, 16), N(Bn, 6, 8),
    N(An, 6, 16), N(An, 6, 8), N(An, 6, 8), N(Gn, 6, 8), N(An, 6, 8),
    N(Bn, 6, 24), N(Gn, 6, 16), N(An, 6, 8),
    N(Bn, 6, 16), N(Bn, 6, 8), N(Bn, 6, 8), N(An, 6, 8), N(Bn, 6, 8),
    N(Cn, 7, 24), N(En, 7, 16), N(En, 7, 8),
    N(Dn, 7, 8), N(En, 7, 8), N(Dn, 7, 8), N(Cn, 7, 8), N(Bn, 6, 8), N(An, 6, 8),
    N(Gn, 6, 24), N(Gn, 6, 16), N(Bn, 6, 8),
    N(Dn, 7, 8), N(Dn, 7, 8), N(Dn, 7, 8), N(En, 7, 16), N(En, 7, 8),

    N(Dn, 7, 24), N(Dn, 7, 16), N(Dn, 7, 8),
    N(Bn, 6, 8), N(Bn, 6, 8), N(Bn, 6, 8), N(Cn, 7, 16), N(Cn, 7, 8),
    N(Bn, 6, 24), N(Bn, 6, 8), N(Gn, 6, 8), N(An, 6, 8),
    N(Bn, 6, 16), N(Bn, 6, 8), N(Bn, 6, 8), N(An, 6, 8), N(Bn, 6, 8),
    N(Cn, 7, 24), N(Bn, 6, 16), N(Bn, 6, 8),
    N(An, 6, 16), N(An, 6, 8), N(An, 6, 8), N(Gn, 6, 8), N(An, 6, 8),
    N(Bn, 6, 24), N(Gn, 6, 16), N(An, 6, 8),
    N(Bn, 6, 16), N(Bn, 6, 8), N(Bn, 6, 8), N(An, 6, 8), N(Bn, 6, 8),
    N(Cn, 7, 16), N(Dn, 7, 8), N(En, 7, 16), N(En, 7, 8),
    N(Dn, 7, 8), N(En, 7, 8), N(Dn, 7, 8), N(Cn, 7, 16), N(An, 6, 8),
    N(Gn, 6, 24), N(Gn, 6, 16), PAUSE(8),
    PAUSE(48),
    0};
const uint16_t Reveille[] PROGMEM = {
    N(Gn, 6, 4),
    N(Cn, 7, 8), N(En, 7, 4), N(Cn, 7, 4), N(Gn, 6, 8), N(En, 7, 8),
    N(Cn, 7, 8), N(En, 7, 4), N(Cn, 7, 4), N(Gn, 6, 8), N(En, 7, 8),
    N(Cn, 7, 8), N(En, 7, 4), N(Cn, 7, 4), N(Gn, 6, 8), N(Cn, 7, 8),
    N(En, 7, 16), N(Cn, 7, 8), N(Gn, 6, 8),
    N(Cn, 7, 8), N(En, 7, 4), N(Cn, 7, 4), N(Gn, 6, 8), N(En, 7, 8),
    N(Cn, 7, 8), N(En, 7, 4), N(Cn, 7, 4), N(Gn, 6, 8), N(En, 7, 8),
    N(Cn, 7, 8), N(En, 7, 4), N(Cn, 7, 4), N(Gn, 6, 8), N(Gn, 6, 8),
    N(Cn, 7, 24), PAUSE(8),
    N(En, 7, 8),
    N(En, 7, 8), N(En, 7, 8), N(En, 7, 8), N(En, 7, 8),
    N(Gn, 7, 16), N(En, 7, 8), N(Cn, 7, 8),
    N(En, 7, 8), N(Cn, 7, 8), N(En, 7, 8), N(Cn, 7, 8),
    N(En, 7, 16), N(Cn, 7, 8), N(En, 7, 8),
    N(En, 7, 8), N(En, 7, 8), N(En, 7, 8), N(En, 7, 8),
    N(Gn, 7, 16), N(En, 7, 8), N(Cn, 7, 8),
    N(En, 7, 8), N(Cn, 7, 8), N(Gn, 6, 8), N(Gn, 6, 8),
    N(Cn, 7, 24), PAUSE(8),
    0};
const uint16_t BigBen[] PROGMEM = {
    N(Bn, 5, 32), N(Gn, 5, 32), N(An, 5, 32), N(Dn, 5, 32),
    N(Gn, 5, 32), N(An, 5, 32), N(Bn, 5, 32), N(Gn, 5, 32),
    N(Bn, 5, 32), N(An, 5, 32), N(Gn, 5, 32), N(Dn, 5, 32),
    N(Dn, 5, 32), N(An, 5, 32), N(Bn, 5, 32), N(Gn, 5, 32),
    N(Gn, 5, 32), N(Gn, 5, 32), N(Gn, 5, 32), N(Gn, 5, 32),
    PAUSE(32),
    0};
const uint16_t T2Theme[] PROGMEM = {
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),

    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(Fn, 5, 60),
    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(An, 6, 60), N(Gn, 6, 60),
    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(Gn, 5, 60),
    N(Fn, 5, 60), N(Dn, 5, 9), N(Fn, 5, 60), N(En, 5, 60), END};

const uint16_t(*Songs[]) PROGMEM = {
    TwinkleTwinkle,
    MaryHadALittleLamb,
    MerryXmas,
    JollyGoodFellow,
    Reveille,
    BigBen,
    T2Theme};
const size_t SongCount = sizeof(Songs) / sizeof(Songs[0]);

int getNoteFrequency(int note, int octave)
{
    if (note == SILENCE || note < Cn || note > Bn || octave < MIN_OCTAVE || octave > MAX_OCTAVE)
    {
        return 0;
    }

    note = note < Cn ? Cn : note;
    note = note > Bn ? Bn : note;
    int semitoneDistance = note - An + 12 * (octave - 4);
    int frequency = static_cast<int>(std::round(A4_FREQUENCY * std::pow(2.0, semitoneDistance / 12.0)));
    return frequency;
}

using PlayNoteCallback = std::function<void(uint8_t, uint8_t, uint8_t)>;
void playMusic(const uint16_t *music, PlayNoteCallback playNote)
{
    int noteIndex = 0;
    uint16_t note;
    while ((note = music[noteIndex++]) != END)
    {
        uint8_t octave = (note & OCTAVE_MASK) >> 12;
        uint8_t index = (note & NOTE_MASK) >> 8;
        uint8_t timing = note & TIMING_MASK;

        playNote(index, octave, timing);
    }
}