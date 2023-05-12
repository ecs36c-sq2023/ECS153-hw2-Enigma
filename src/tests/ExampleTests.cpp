extern "C" {
#include "Enigma.h"
#include "common.h"
}
#include <gtest/gtest.h>

#define ECS153 "I REALLY LIKE ECS ONE FIVE THREE"

namespace {

TEST(ExampleTests, TestPlugboard) {
  Enigma *enigma = new_Enigma(0, NULL, NULL, NULL, LETTERS, 5, "ECSONFIVTH");
  char encrypt[64];
  memset(encrypt, 0, 64);
  encrypt_Enigma(enigma, encrypt, ECS153);
  EXPECT_STREQ(encrypt, ECS153);
  free_Enigma(enigma);
}

TEST(ExampleTests, TestReflector_Ident) {
  Enigma *enigma = new_Enigma(0, NULL, NULL, NULL, LETTERS, 0, NULL);
  char encrypt[64];
  memset(encrypt, 0, 64);
  encrypt_Enigma(enigma, encrypt, ECS153);
  EXPECT_STREQ(encrypt, ECS153);
  free_Enigma(enigma);
}

TEST(ExampleTests, TestReflector_UKWA) {
  Enigma *enigma = new_Enigma(0, NULL, NULL, NULL, I_UKW_A, 0, NULL);
  char encrypt[64];
  memset(encrypt, 0, 64);
  encrypt_Enigma(enigma, encrypt, ECS153);
  EXPECT_STREQ(encrypt, "V NAEFFG FVWA AMT QRA LVIA SXNAA");
  free_Enigma(enigma);
}

TEST(ExampleTests, TestPlugboardAndReflector_UKWA) {
  Enigma *enigma = new_Enigma(0, NULL, NULL, NULL, I_UKW_B, 5, "ECSONFIVTH");
  char encrypt[64];
  memset(encrypt, 0, 64);
  encrypt_Enigma(enigma, encrypt, ECS153);
  EXPECT_STREQ(encrypt, "W BUYGGA GWFU UQM NOU KWPU DZBUU");
  free_Enigma(enigma);
}

Enigma *get_default_Enigma() {
  const char *rotors[] = {ROTOR_I, ROTOR_II, ROTOR_III};
  size_t rings[] = {1, 5, 3};
  size_t inits[] = {1, 5, 3};
  return new_Enigma(3, rotors, rings, inits, I_UKW_A, 5, "ECSONFIVTH");
}

TEST(ExampleTests, TestRotorSetting) {
  Enigma *enigma = get_default_Enigma();
  char settings[8];
  memset(settings, 0, 8);
  get_setting_Enigma(enigma, settings);
  ASSERT_STREQ("BFD", settings);
  free_Enigma(enigma);
}

TEST(ExampleTests, TestRotorTicking) {
  Enigma *enigma = get_default_Enigma();
  char settings[8];
  memset(settings, 0, 8);
  tick_n_Enigma(enigma, 153);
  get_setting_Enigma(enigma, settings);
  ASSERT_STREQ("YLD", settings);
  free_Enigma(enigma);
}

TEST(ExampleTests, TestEnigmaOneRotorWORing) {
  // Plugboard: C->C
  // Rotor I: C->J
  // Reflector: J->B
  // Reverse Rotor I: B->E
  // Plugboard: E->E
  const char *rotors[] = {ROTOR_I};
  size_t rings[] = {0};
  size_t inits[] = {1};
  Enigma *enigma = new_Enigma(1, rotors, rings, inits, I_UKW_A, 0, NULL);
  char encrypt[] = "\0\0\0\0";
  encrypt_Enigma(enigma, encrypt, "C");
  ASSERT_STREQ(encrypt, "E");
  free_Enigma(enigma);
}

TEST(ExampleTests, TestEnigmaOneRotorWRing) {
  // Plugboard: C->C
  // Rotor I: C->E
  // Reflector: E->A
  // Reverse Rotor I: A->V
  // Plugboard: V->V
  const char *rotors[] = {ROTOR_I};
  size_t rings[] = {1};
  size_t inits[] = {1};
  Enigma *enigma = new_Enigma(1, rotors, rings, inits, I_UKW_A, 0, NULL);
  char encrypt[] = "\0\0\0\0";
  encrypt_Enigma(enigma, encrypt, "C");
  ASSERT_STREQ(encrypt, "V");
  free_Enigma(enigma);
}

TEST(ExampleTests, IntegralTest) {
  Enigma *enigma = get_default_Enigma();
  char encrypt[64];
  memset(encrypt, 0, 64);
  encrypt_Enigma(enigma, encrypt, ECS153);
  EXPECT_STREQ(encrypt, "H LAXPFV CHTN CYC BKD JMBH IUPAL");
  size_t inits[] = {1, 5, 3};
  reset_rotor_Enigma(enigma, inits);
  char decrypt[64];
  memset(decrypt, 0, 64);
  encrypt_Enigma(enigma, decrypt, encrypt);
  EXPECT_STREQ(ECS153, decrypt);
  free_Enigma(enigma);
}

} // namespace