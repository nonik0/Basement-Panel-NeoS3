#pragma once

#include <Arduino.h>

// macros for each note in an octave
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

// notes are specified with 16 bits
// the octave:    high nibble, high byte
// the note:      low nibble,  high byte
// the duration:  low byte
#define N(note, octave, timing) ((octave << 12) | (note << 8) | timing)
#define NOTE_MASK 0x0F00
#define OCTAVE_MASK 0xF000
#define TIMING_MASK 0x00FF

// other "sounds" are defined within octave zero
// which otherwise does not exits
#define PAUSE(timing) N(0, 0, timing) // silence

#define BEEP_HIGH(timing) N(1, 0, timing) // 4100 Hz beep
#define BEEP_HIGH_TOP 1951

#define BEEP_LOW(timing) N(3, 0, timing) // 1367 Hz beep
#define BEEP_LOW_TOP 5854

const uint16_t t2_theme[] PROGMEM = {
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),
    N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), PAUSE(3), N(Dn, 6, 6), N(Dn, 6, 6), PAUSE(20),

    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(Fn, 5, 60),
    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(An, 6, 60), N(Gn, 6, 60),
    N(Dn, 6, 9), N(En, 6, 9), N(Fn, 6, 60), N(En, 6, 15), N(Cn, 6, 9), N(Gn, 5, 60),
    N(Fn, 5, 60), N(Dn, 5, 9), N(Fn, 5, 60), N(En, 5, 60),
    0};