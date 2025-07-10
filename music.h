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
#define NOTE_COUNT 12
#define OCTAVE_MASK 0xF000
#define NOTE_MASK 0x0F00
#define TIMING_MASK 0x00FF
#define SILENCE 0xF
#define N(note, octave, timing) ((octave << 12) | (note << 8) | timing)
#define PAUSE(timing) N(SILENCE, SILENCE, timing)
#define END N(0, 0, 0)
#define A4_FREQUENCY 440.0
#define MIN_OCTAVE 1 // actually 0 but I want 8 steps and normal piezo buzzers def can't play that low
#define MAX_OCTAVE 8

const char *Notes[] = {"C", "Cs", "D", "Ds", "E", "F", "Fs", "G", "Gs", "A", "As", "B"};

int getNoteFrequency(int note, int octave)
{
    // includes defined SILENCE value for note and octave
    if (note > Bn || octave > MAX_OCTAVE)
    {
        return 0;
    }

    note = note < Cn ? Cn : note;
    note = note > Bn ? Bn : note;
    int semitoneDistance = note - An + 12 * (octave - 4);
    int frequency = static_cast<int>(std::round(A4_FREQUENCY * std::pow(2.0, semitoneDistance / 12.0)));
    return frequency;
}

// using PlayNoteCallback = std::function<void(uint8_t, uint8_t, uint8_t)>;
// void playMusic(const uint16_t *music, PlayNoteCallback playNote)
// {
//     int noteIndex = 0;
//     uint16_t note = music[noteIndex++];
//     while (note != END)
//     {
//         uint8_t octave = (note & OCTAVE_MASK) >> 12;
//         uint8_t index = (note & NOTE_MASK) >> 8;
//         uint8_t timing = note & TIMING_MASK;

//         playNote(index, octave, timing);

//         note = music[noteIndex++];
//     }
// }

const uint16_t BigBen[] PROGMEM = {
    N(Bn, 5, 32), N(Gn, 5, 32), N(An, 5, 32), N(Dn, 5, 32),
    N(Gn, 5, 32), N(An, 5, 32), N(Bn, 5, 32), N(Gn, 5, 32),
    N(Bn, 5, 32), N(An, 5, 32), N(Gn, 5, 32), N(Dn, 5, 32),
    N(Dn, 5, 32), N(An, 5, 32), N(Bn, 5, 32), N(Gn, 5, 32),
    N(Gn, 5, 32), N(Gn, 5, 32), N(Gn, 5, 32), N(Gn, 5, 32),
    PAUSE(32),
    END};
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
    END};
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
    END};
const uint16_t PinkPanther[] PROGMEM = {
    PAUSE(16), PAUSE(4), PAUSE(8), N(Dn, 4, 4),
    N(En, 4, 12), PAUSE(4), N(Fs, 4, 4), N(Gn, 4, 12), PAUSE(4), N(Ds, 4, 4),
    N(En, 4, 6), N(Fs, 4, 4), N(Gn, 4, 6), N(Cn, 5, 4), N(Bn, 4, 6), N(En, 4, 4), N(Gn, 4, 6), N(Bn, 4, 4),
    N(As, 4, 16), N(An, 4, 3), N(Gn, 4, 3), N(En, 4, 3), N(Dn, 4, 3),
    N(En, 4, 16), PAUSE(8), PAUSE(4), N(Dn, 4, 8),

    N(En, 4, 4), PAUSE(4), N(Fs, 4, 4), N(Gn, 4, 12), PAUSE(4), N(Ds, 4, 4),
    N(En, 4, 6), N(Fs, 4, 4), N(Gn, 4, 6), N(Cn, 5, 4), N(Bn, 4, 6), N(Gn, 4, 4), N(Bn, 4, 6), N(En, 5, 4),
    N(Ds, 5, 32),
    N(Dn, 5, 16), PAUSE(8), PAUSE(4), N(Ds, 4, 4),
    N(En, 4, 12), PAUSE(4), N(Fs, 4, 4), N(Gn, 4, 12), PAUSE(4), N(Ds, 4, 4),
    N(En, 4, 6), N(Fs, 4, 4), N(Gn, 4, 6), N(Cn, 5, 4), N(Bn, 4, 6), N(En, 4, 4), N(Gn, 4, 6), N(Bn, 4, 4),
  
    N(As, 4, 16), N(An, 4, 3), N(Gn, 4, 3), N(En, 4, 3), N(Dn, 4, 3),
    N(En, 4, 12), PAUSE(8),
    PAUSE(8), N(En, 5, 6), N(Dn, 5, 4), N(Bn, 4, 6), N(An, 4, 4), N(Gn, 4, 6), N(En, 4, 6),
    N(As, 4, 2), N(An, 4, 6), N(As, 4, 2), N(An, 4, 6), N(As, 4, 2), N(An, 4, 6), N(As, 4, 2), N(An, 4, 6),
    N(As, 4, 3), N(En, 4, 3), N(Dn, 4, 3), N(En, 4, 2), N(En, 4, 2), N(En, 4, 2),
    END};
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
    END};
const uint16_t SongOfStorms[] PROGMEM = {
    N(Dn, 4, 8), N(An, 4, 8), N(An, 4, 8),
    PAUSE(4), N(En, 4, 4), N(Bn, 4, 16),
    N(Fn, 4, 8), N(Cn, 5, 8), N(Cn, 5, 8),
    PAUSE(4), N(En, 4, 4), N(Bn, 4, 16),
    N(Dn, 4, 8), N(An, 4, 8), N(An, 4, 8),
    PAUSE(4), N(En, 4, 4), N(Bn, 4, 16),
    N(Fn, 4, 8), N(Cn, 5, 8), N(Cn, 5, 8),
    PAUSE(4), N(En, 4, 4), N(Bn, 4, 16),
    N(Dn, 4, 4), N(Fn, 4, 4), N(Dn, 5, 16),

    N(Dn, 4, 4), N(Fn, 4, 4), N(Dn, 5, 16),
    N(En, 5, 12), N(Fn, 5, 4), N(En, 5, 4), N(En, 5, 4),
    N(En, 5, 4), N(Cn, 5, 4), N(An, 4, 16),
    N(An, 4, 8), N(Dn, 4, 8), N(Fn, 4, 4), N(Gn, 4, 4),
    N(An, 4, 24),
    N(An, 4, 8), N(Dn, 4, 8), N(Fn, 4, 4), N(Gn, 4, 4),
    N(En, 4, 24),
    N(Dn, 4, 4), N(Fn, 4, 4), N(Dn, 5, 16),
    N(Dn, 4, 4), N(Fn, 4, 4), N(Dn, 5, 16),

    N(En, 5, 12), N(Fn, 5, 4), N(En, 5, 4), N(En, 5, 4),
    N(En, 5, 4), N(Cn, 5, 4), N(An, 4, 16),
    N(An, 4, 8), N(Dn, 4, 8), N(Fn, 4, 4), N(Gn, 4, 4),
    N(An, 4, 16), N(An, 4, 8),
    N(Dn, 4, 32),
    END};
const uint16_t StarWarsCantina[] PROGMEM = { 
    N(Bn, 4, 12), N(En, 5, 12), N(Bn, 4, 12), N(En, 5, 12),
    N(Bn, 4, 4), N(En, 5, 12), N(Bn, 4, 4), PAUSE(4), N(As, 4, 4), N(Bn, 4, 4),
    N(Bn, 4, 4), N(As, 4, 4), N(Bn, 4, 4), N(An, 4, 4), PAUSE(4), N(Gs, 4, 4), N(An, 4, 4), N(Gn, 4, 4),
    N(Gn, 4, 8), N(En, 4, 24),
    N(Bn, 4, 12), N(En, 5, 12), N(Bn, 4, 12), N(En, 5, 12),
    N(Bn, 4, 4), N(En, 5, 12), N(Bn, 4, 4), PAUSE(4), N(As, 4, 4), N(Bn, 4, 4),

    N(An, 4, 12), N(An, 4, 12), N(Gs, 4, 4), N(An, 4, 12),
    N(Dn, 5, 4), N(Cn, 5, 12), N(Bn, 4, 12), N(An, 4, 12),
    N(Bn, 4, 12), N(En, 5, 12), N(Bn, 4, 12), N(En, 5, 12),
    N(Bn, 4, 4), N(En, 5, 12), N(Bn, 4, 4), PAUSE(4), N(As, 4, 4), N(Bn, 4, 4),
    N(Dn, 5, 8), N(Dn, 5, 12), N(Bn, 4, 4), N(An, 4, 12),
    N(Gn, 4, 12), N(En, 4, 24),
    N(En, 4, 16), N(Gn, 4, 16),
    N(Bn, 4, 16), N(Dn, 5, 16),

    N(Fn, 5, 12), N(En, 5, 12), N(As, 4, 4), N(As, 4, 4), N(Bn, 4, 4), N(Gn, 4, 8 ),
    END};
const uint16_t SuperMario[] PROGMEM = {
    N(En, 5, 4), N(En, 5, 4), PAUSE(4), N(En, 5, 4), PAUSE(4), N(Cn, 5, 4), N(En, 5, 4), // 1
    N(Gn, 5, 8), PAUSE(8), N(Gn, 4, 4), PAUSE(8),
    N(Cn, 5, 12), N(Gn, 4, 4), PAUSE(8), N(En, 4, 12), // 3
    N(An, 4, 8), N(Bn, 4, 8), N(As, 4, 4), N(An, 4, 8),
    N(Gn, 4, 6), N(En, 5, 6), N(Gn, 5, 6), N(An, 5, 8), N(Fn, 5, 4), N(Gn, 5, 4),
    PAUSE(4), N(En, 5, 8), N(Cn, 5, 4), N(Dn, 5, 4), N(Bn, 4, 12),
    N(Cn, 5, 12), N(Gn, 4, 4), PAUSE(8), N(En, 4, 12), // repeats from 3
    N(An, 4, 8), N(Bn, 4, 8), N(As, 4, 4), N(An, 4, 8),
    N(Gn, 4, 6), N(En, 5, 6), N(Gn, 5, 6), N(An, 5, 8), N(Fn, 5, 4), N(Gn, 5, 4),
    PAUSE(4), N(En, 5, 8), N(Cn, 5, 4), N(Dn, 5, 4), N(Bn, 4, 12),


    PAUSE(8), N(Gn, 5, 4), N(Fs, 5, 4), N(Fn, 5, 4), N(Ds, 5, 8), N(En, 5, 4), // 7
    PAUSE(4), N(Gs, 4, 4), N(An, 4, 4), N(Cn, 4, 4), PAUSE(4), N(An, 4, 4), N(Cn, 5, 4), N(Dn, 5, 4),
    PAUSE(8), N(Ds, 5, 8), PAUSE(4), N(Dn, 5, 12),
    N(Cn, 5, 16), PAUSE(16),

    PAUSE(8), N(Gn, 5, 4), N(Fs, 5, 4), N(Fn, 5, 4), N(Ds, 5, 8), N(En, 5, 4), // repeats from 7
    PAUSE(4), N(Gs, 4, 4), N(An, 4, 4), N(Cn, 4, 4), PAUSE(4), N(An, 4, 4), N(Cn, 5, 4), N(Dn, 5, 4),
    PAUSE(8), N(Ds, 5, 8), PAUSE(4), N(Dn, 5, 12),
    N(Cn, 5, 16), PAUSE(16),

    N(Cn, 5, 4), N(Cn, 5, 8), N(Cn, 5, 4), PAUSE(4), N(Cn, 5, 4), N(Dn, 5, 8), // 11
    N(En, 5, 4), N(Cn, 5, 8), N(An, 4, 4), N(Gn, 4, 16),

    N(Cn, 5, 4), N(Cn, 5, 8), N(Cn, 5, 4), PAUSE(4), N(Cn, 5, 4), N(Dn, 5, 4), N(En, 5, 4), // 13
    PAUSE(32),
    N(Cn, 5, 4), N(Cn, 5, 8), N(Cn, 5, 4), PAUSE(4), N(Cn, 5, 4), N(Dn, 5, 8),
    N(En, 5, 4), N(Cn, 5, 8), N(An, 4, 4), N(Gn, 4, 16),
    N(En, 5, 4), N(En, 5, 4), PAUSE(4), N(En, 5, 4), PAUSE(4), N(Cn, 5, 4), N(En, 5, 8),
    N(Gn, 5, 8), PAUSE(8), N(Gn, 4, 8), PAUSE(8),
    N(Cn, 5, 12), N(Gn, 4, 4), PAUSE(8), N(En, 4, 12), // 19

    N(An, 4, 8), N(Bn, 4, 8), N(As, 4, 4), N(An, 4, 8),
    N(Gn, 4, 6), N(En, 5, 6), N(Gn, 5, 6), N(An, 5, 8), N(Fn, 5, 4), N(Gn, 5, 4),
    PAUSE(4), N(En, 5, 8), N(Cn, 5, 4), N(Dn, 5, 4), N(Bn, 4, 12),

    N(Cn, 5, 12), N(Gn, 4, 4), PAUSE(8), N(En, 4, 12), // repeats from 19
    N(An, 4, 8), N(Bn, 4, 8), N(As, 4, 4), N(An, 4, 8),
    N(Gn, 4, 6), N(En, 5, 6), N(Gn, 5, 6), N(An, 5, 8), N(Fn, 5, 4), N(Gn, 5, 4),
    PAUSE(4), N(En, 5, 8), N(Cn, 5, 4), N(Dn, 5, 4), N(Bn, 4, 12),

    N(En, 5, 4), N(Cn, 5, 8), N(Gn, 4, 4), PAUSE(8), N(Gs, 4, 8), // 23
    N(An, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(An, 4, 16),
    N(Dn, 5, 6), N(An, 5, 6), N(An, 5, 6), N(An, 5, 6), N(Gn, 5, 6), N(Fn, 5, 6),

    N(En, 5, 4), N(Cn, 5, 8), N(An, 4, 4), N(Gn, 4, 16), // 26
    N(En, 5, 4), N(Cn, 5, 8), N(Gn, 4, 4), PAUSE(8), N(Gs, 4, 8),
    N(An, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(An, 4, 16),
    N(Bn, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(Fn, 5, 6), N(En, 5, 6), N(Dn, 5, 6),
    N(Cn, 5, 4), N(En, 4, 8), N(En, 4, 4), N(Cn, 4, 16),

    N(En, 5, 4), N(Cn, 5, 8), N(Gn, 4, 4), PAUSE(8), N(Gs, 4, 8), // repeats from 23
    N(An, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(An, 4, 16),
    N(Dn, 5, 6), N(An, 5, 6), N(An, 5, 6), N(An, 5, 6), N(Gn, 5, 6), N(Fn, 5, 6),

    N(En, 5, 4), N(Cn, 5, 8), N(An, 4, 4), N(Gn, 4, 16), // 26
    N(En, 5, 4), N(Cn, 5, 8), N(Gn, 4, 4), PAUSE(8), N(Gs, 4, 8),
    N(An, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(An, 4, 16),
    N(Bn, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(Fn, 5, 6), N(En, 5, 6), N(Dn, 5, 6),
    N(Cn, 5, 4), N(En, 4, 8), N(En, 4, 4), N(Cn, 4, 16),
    N(Cn, 5, 4), N(Cn, 5, 8), N(Cn, 5, 4), PAUSE(4), N(Cn, 5, 4), N(Dn, 5, 4), N(En, 5, 4),
    PAUSE(32),

    N(Cn, 5, 4), N(Cn, 5, 8), N(Cn, 5, 4), PAUSE(4), N(Cn, 5, 4), N(Dn, 5, 8), // 33
    N(En, 5, 4), N(Cn, 5, 8), N(An, 4, 4), N(Gn, 4, 16),
    N(En, 5, 4), N(En, 5, 4), PAUSE(4), N(En, 5, 4), PAUSE(4), N(Cn, 5, 4), N(En, 5, 8),
    N(Gn, 5, 8), PAUSE(8), N(Gn, 4, 8), PAUSE(8),
    N(En, 5, 4), N(Cn, 5, 8), N(Gn, 4, 4), PAUSE(8), N(Gs, 4, 8),
    N(An, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(An, 4, 16),
    N(Dn, 5, 6), N(An, 5, 6), N(An, 5, 6), N(An, 5, 6), N(Gn, 5, 6), N(Fn, 5, 6),

    N(En, 5, 4), N(Cn, 5, 8), N(An, 4, 4), N(Gn, 4, 16), // 40
    N(En, 5, 4), N(Cn, 5, 8), N(Gn, 4, 4), PAUSE(8), N(Gs, 4, 8),
    N(An, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(An, 4, 16),
    N(Bn, 4, 4), N(Fn, 5, 8), N(Fn, 5, 4), N(Fn, 5, 6), N(En, 5, 6), N(Dn, 5, 6),
    N(Cn, 5, 4), N(En, 4, 8), N(En, 4, 4), N(Cn, 4, 16),

    // game over sound
    N(Cn, 5, 12), N(Gn, 4, 12), N(En, 4, 8), // 45
    N(An, 4, 6), N(Bn, 4, 6), N(An, 4, 6), N(Gs, 4, 6), N(As, 4, 6), N(Gs, 4, 6),
    N(Gn, 4, 4), N(Dn, 4, 4), N(En, 4, 24),

    END};
const uint16_t Terminator2[] PROGMEM = {
    N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(30),
    N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(30),
    N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(30),
    N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), PAUSE(6), N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(30),

    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(Fn, 5, 60),
    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(An, 6, 60), N(Gn, 6, 60),
    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(Gn, 5, 60),
    N(Fn, 5, 60), N(Dn, 5, 9), N(Fn, 5, 60), N(En, 5, 60),

    N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(5), N(Dn, 6, 9), PAUSE(5), N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(30),
    N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(5), N(Dn, 6, 9), PAUSE(5), N(Dn, 6, 9), N(Dn, 6, 9), PAUSE(30),
    END};
const uint16_t Tetris[] PROGMEM = {
    N(Bn, 4, 4), N(Cn, 5, 4), N(Dn, 5, 8), N(Cn, 5, 4), N(Bn, 4, 4),
    N(An, 4, 4), N(Cn, 5, 4), N(En, 5, 8), N(Dn, 5, 4), N(Cn, 5, 4),
    N(Cn, 5, 4), N(Dn, 5, 8), N(En, 5, 8),
    N(An, 4, 8), N(An, 4, 4), N(An, 4, 8), N(Bn, 4, 4), N(Cn, 5, 4),

    N(Fn, 5, 4), N(An, 5, 8), N(Gn, 5, 4), N(Fn, 5, 4),
    N(Cn, 5, 4), N(En, 5, 8), N(Dn, 5, 4), N(Cn, 5, 4),
    N(Bn, 4, 4), N(Cn, 5, 4), N(Dn, 5, 8), N(En, 5, 8),
    N(An, 4, 8), N(An, 4, 8),

    N(Bn, 4, 4), N(Cn, 5, 4), N(Dn, 5, 8), N(Cn, 5, 4), N(Bn, 4, 4),
    N(An, 4, 4), N(Cn, 5, 4), N(En, 5, 8), N(Dn, 5, 4), N(Cn, 5, 4),
    N(Cn, 5, 4), N(Dn, 5, 8), N(En, 5, 8),
    N(An, 4, 8), N(An, 4, 4), N(An, 4, 8), N(Bn, 4, 4), N(Cn, 5, 4),

    N(Fn, 5, 4), N(An, 5, 8), N(Gn, 5, 4), N(Fn, 5, 4),
    N(Cn, 5, 4), N(En, 5, 8), N(Dn, 5, 4), N(Cn, 5, 4),
    N(Bn, 4, 4), N(Cn, 5, 4), N(Dn, 5, 8), N(En, 5, 8),
    N(An, 4, 8), N(An, 4, 8),


    N(En, 5, 16), N(Cn, 5, 16),
    N(Dn, 5, 16), N(Bn, 4, 16),
    N(Cn, 5, 16), N(An, 4, 16),
    N(Gs, 4, 16), N(Bn, 4, 8), PAUSE(4),
    N(En, 5, 16), N(Cn, 5, 16),
    N(Dn, 5, 16), N(Bn, 4, 16),
    N(Cn, 5, 8), N(En, 5, 8), N(An, 5, 16),
    N(Gs, 5, 16),
    END};
const uint16_t TwinkleTwinkle[] PROGMEM = {
    N(Cn, 4, 10), N(Cn, 4, 10), N(Gn, 4, 10), N(Gn, 4, 10), N(An, 4, 10), N(An, 4, 10), N(Gn, 4, 20),
    N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 10), N(Dn, 4, 10), N(Cn, 4, 20),
    N(Gn, 4, 10), N(Gn, 4, 10), N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 20),
    N(Gn, 4, 10), N(Gn, 4, 10), N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 20),
    N(Cn, 4, 10), N(Cn, 4, 10), N(Gn, 4, 10), N(Gn, 4, 10), N(An, 4, 10), N(An, 4, 10), N(Gn, 4, 20),
    N(Fn, 4, 10), N(Fn, 4, 10), N(En, 4, 10), N(En, 4, 10), N(Dn, 4, 10), N(Dn, 4, 10), N(Cn, 4, 20), END};
const uint16_t Zelda[] PROGMEM = {
    N(As, 4, 24), N(Fn, 4, 4), N(Fn, 4, 4), N(As, 4, 4), // 1
    N(Gs, 4, 2), N(Fs, 4, 2), N(Gs, 4, 24),
    N(As, 4, 24), N(Fs, 4, 4), N(Fs, 4, 4), N(As, 4, 4),
    N(An, 4, 2), N(Gn, 4, 2), N(An, 4, 24),
    PAUSE(32),

    N(As, 4, 8), N(Fn, 4, 12), N(As, 4, 4), N(As, 4, 2), N(Cn, 5, 2), N(Dn, 5, 2), N(Ds, 5, 2), // 7
    N(Fn, 5, 16), N(Fn, 5, 4), N(Fn, 5, 4), N(Fn, 5, 4), N(Fs, 5, 2), N(Gs, 5, 2),
    N(As, 5, 24), N(As, 5, 4), N(As, 5, 4), N(Gs, 5, 4), N(Fs, 5, 2),
    N(Gs, 5, 6), N(Fs, 5, 2), N(Fn, 5, 16), N(Fn, 5, 8),

    N(Ds, 5, 6), N(Fn, 5, 2), N(Fs, 5, 16), N(Fn, 5, 4), N(Ds, 5, 4), // 11
    N(Cs, 5, 6), N(Ds, 5, 2), N(Fn, 5, 16), N(Ds, 5, 4), N(Cs, 5, 4),
    N(Cn, 5, 6), N(Dn, 5, 2), N(En, 5, 16), N(Gn, 5, 4),
    N(Fn, 5, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 4), N(Fn, 4, 2), N(Fn, 4, 4),

    N(As, 4, 8), N(Fn, 4, 12), N(As, 4, 4), N(As, 4, 2), N(Cn, 5, 2), N(Dn, 5, 2), N(Ds, 5, 2), // 15
    N(Fn, 5, 16), N(Fn, 5, 4), N(Fn, 5, 4), N(Fn, 5, 4), N(Fs, 5, 2), N(Gs, 5, 2),
    N(As, 5, 24), N(Cs, 6, 8),
    N(Cn, 6, 8), N(An, 5, 16), N(Fn, 5, 8),
    N(Fs, 5, 24), N(As, 5, 8),
    N(An, 5, 8), N(Fn, 5, 16), N(Fn, 5, 8),

    N(Fs, 5, 24), N(As, 5, 8),
    N(An, 5, 8), N(Fn, 5, 16), N(Dn, 5, 8),
    N(Ds, 5, 24), N(Fs, 5, 8),
    N(Fn, 5, 8), N(Cs, 5, 16), N(As, 4, 8),
    N(Cn, 5, 6), N(Dn, 5, 2), N(En, 5, 16), N(Gn, 5, 4),
    N(Fn, 5, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 2), N(Fn, 4, 4), N(Fn, 4, 2), N(Fn, 4, 4),
    END};

const uint16_t TestSong[] PROGMEM = {
    N(Dn, 6, 10), N(Dn, 6, 10), PAUSE(5), N(Dn, 6, 10), PAUSE(5), N(Dn, 6, 10), N(Dn, 6, 10), PAUSE(30),
    N(Dn, 6, 10), N(Dn, 6, 10), PAUSE(5), N(Dn, 6, 10), PAUSE(5), N(Dn, 6, 10), N(Dn, 6, 10), PAUSE(30),
    END};

// weird ordering bc of rising index for keypad in each col
const uint16_t(*Songs[]) PROGMEM = {
    MerryXmas,
    MaryHadALittleLamb,
    JollyGoodFellow,
    BigBen,

    StarWarsCantina,
    SongOfStorms,
    PinkPanther,
    Reveille,
    
    TwinkleTwinkle,
    Tetris,
    Terminator2,
    SuperMario,

    Zelda,
};
const size_t SongCount = sizeof(Songs) / sizeof(Songs[0]);
const char* SongName4Chars[] = {
    "XMAS",
    "LAMB",
    "JOLY",
    "BEN ",

    "SWRS",
    "STRM",
    "PINK",
    "REVE",
    
    "TWNK",
    "TETR",
    "T-2 ",
    "MRIO",

    "ZLDA",
};
const uint8_t SongDefaultTimingUnitMs[] = {
    30,
    30,
    30,
    40,

    25,
    40,
    50,
    40,
    
    40,
    60,
    30,
    35,

    45,
};