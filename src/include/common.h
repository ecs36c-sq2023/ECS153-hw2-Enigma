#ifndef COMMON_H
#define COMMON_H

#define NEW_TYPE(Type, ...)                                                    \
  do {                                                                         \
    Type *x__obj = (Type *)malloc(sizeof(Type));                               \
    *x__obj = (Type){__VA_ARGS__};                                             \
    return x__obj;                                                             \
  } while (0);

/// Debug utilities, define DEBUG to enable them.
// #define DEBUG
#ifdef DEBUG
#include <stdio.h>
#define DEBUGF(format, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, format, __VA_ARGS__);                                      \
  } while (0);
#else
#define DEBUGF(format, ...)                                                    \
  do {                                                                         \
  } while (0);
#endif

#ifdef DEBUG
#define UNREACHABLE assert(0);
#include "assert.h"
#else
#define UNREACHABLE ;
#endif
#endif

/// Max number for many buffers
#define MAX_SIZE 26

/// Letters for reference
#define LETTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/// Rotor I ~ V used in Enigma I
#define ROTOR_I "EKMFLGDQVZNTOWYHXUSPAIBRCJ,Q"
#define ROTOR_II "AJDKSIRUXBLHWTMCQGZNPYFVOE,E"
#define ROTOR_III "BDFHJLCPRTXVZNYEIWGAKMUSQO,V"
#define ROTOR_IV "ESOVPZJAYQUIRHXLNFTGKDCMWB,J"
#define ROTOR_V "VZBRGITYUPSDNHLXAWMJQOFECK,Z"

/// Reflectors used in Enigma I
#define I_UKW_A "EJMZALYXVBWFCRQUONTSPIKHGD"
#define I_UKW_B "YRUHQSLDPXNGOKMIEBFZCWVJAT"
#define I_UKW_C "FVPJIAOYEDRZXWGCTKUQSBNMHL"

/// Rotors used in Enigma M4
#define ROTOR_VI "JPGVOUMFYQBENHZRDKASXLICTW,MZ"
#define ROTOR_VII "NZJHGRCXMYSWBOUFAIVLPEKQDT,MZ"
#define ROTOR_VIII "FKQHTLXOCBJSPDZRAMEWNIUYGV,MZ"
#define ROTOR_BETA "LEYJVCNIXWPBQMDRTAKZGFUHOS,\0"
#define ROTOR_GAMMA "FSOKANUERHMBTIYCWLQPZXVGJD,\0"

/// Reflectors in Enigma M4
#define UKW_B "ENKQAUYWJICOPBLMDXZVFTHRGS"
#define UKW_C "RDOBJNTKVEHMLFCWZAXGYIPSUQ"
