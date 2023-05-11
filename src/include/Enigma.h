#ifndef ENIGMA_H
#define ENIGMA_H

#include "common.h"
#include <stdlib.h>

/// "[The Enigma machine](https://en.wikipedia.org/wiki/Enigma_machine#) is a
/// cipher device developed and used in the early- to mid-20th century to
/// protect commercial, diplomatic, and military communication." -- Wikipedia
///
/// This is a struct that represent the machine. It has the following methods.
typedef struct Enigma Enigma;

/// @brief Creates a new Enigma machine
/// @param num_rotors The number of rotors used in this machine
/// @param rotors An array of strings describing the rotor. The first 26
/// charactors is the mapping, or wiring. The 27th charactor must be a comma,
/// followed by the notch(s). The 0th rotor is the fastest one, the last rotor
/// is the closest to the reflector.
/// @param rings The ring setting for each rotor.
/// @param inits The initial setting for each rotor.
/// @param reflector The reflector mapping, or wiring in a 26 charactor string.
/// @param num_pairs The number of cables in the plugboard, i.e. the number of
/// pairs of letters swapped.
/// @param pairs A string with `2*num_pairs` characters describing how letters
/// are swapped. Letter `pairs[i * 2]` is swapped with `pairs[i * 2 + 1]`, vice
/// versa.
/// @return An initialized machine.
Enigma *new_Enigma(size_t num_rotors, const char **rotors, size_t *rings,
                   size_t *inits, const char *reflector, size_t num_pairs,
                   const char *pairs);

/// @brief Encrypt the message
/// @param self A high level representation of the machine.
/// @param dst Destination buffer. Store the result in the buffer. You may
/// assume there will be no overflow.
/// @param src A null terminated string to be encrypted.
/// @return dst
char *encrypt_Enigma(Enigma *self, char *dst, const char *src);

/// @brief Reset the Enigma to the new setting
/// @param self A high level representation of the machine.
/// @param new_setting The new setting for each rotor.
void reset_rotor_Enigma(Enigma *self, size_t *new_setting);

/// @brief Deallocates the Enigma machine.
/// @param self A high level representation of the machine.
void free_Enigma(Enigma *self);

/// @brief Get current setting of the machine.
/// @param self A high level representation of the machine.
/// @param ret A buffer to accept the current setting. You may assume the buffer
/// will have enough size and won't overflow. Setting is written in letters,
/// i.e. 0 -> 'A', 1 -> 'B'
void get_setting_Enigma(Enigma *self, char *ret);

/// @brief Simulate the tick once.
/// @param self A high level representation of the machine.
void tick_Enigma(Enigma *self);

/// @brief Simulate the tick n times.
/// @param self A high level representation of the machine.
/// @param n A number represent how many ticks are there.
void tick_n_Enigma(Enigma *self, size_t n);
#endif