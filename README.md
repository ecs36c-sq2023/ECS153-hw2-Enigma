# Homework 2 Enigma

> Due: 05-29-2023 11:59:59 PM

In another words, do it before the final season!

# Description

"[The Enigma machine](https://en.wikipedia.org/wiki/Enigma_machine#) is a cipher device developed and used in the early- to mid-20th century to protect commercial, diplomatic, and military communication." -- Wikipedia

In this homework, we implement an enigma machine.
You might want to check [this video](https://youtu.be/ybkkiGtJmkM) first to understand how an enigma works.
An enigma machine has roughly three components, and we would implement them one by one.

- [Plugboard](https://en.wikipedia.org/wiki/Enigma_machine#Plugboard)
- [Reflector](https://en.wikipedia.org/wiki/Enigma_machine#Reflector)
- [Rotors](https://en.wikipedia.org/wiki/Enigma_rotor_details)

I trust you can understand how reflector and plugboard works through the video and wikipedia.
Now rotors are the most tricky. Rotors have two properties:

- It turns,
- It maps one letter to another.

Everytime a letter is pressed, the rotor first ticks once.
You have to carefully connect the rotors such that when the notch of the previous one is hit, the later rotor tick as well. 
In another words, [turnover](https://en.wikipedia.org/wiki/Enigma_machine#Turnover) needs to be maintained carefully.
For Enigma I, each rotor only have one notch, so if the previous wheel turns by one circle (26 letters), the later should tick once.
But for Enigma M4, rotor VI to VIII has two notches, while rotor beta doesn't have notch, i.e. Beta never turns!

For the transformation part, the wiring of each rotor is fixed. 
Many of the commonly used rotors (I - VIII) has been set in `common.h`.
You can use [this page](https://www.cryptomuseum.com/crypto/enigma/wiring.htm) how the wiring works exactly.
You can also define your own rotors.

Finally, as a helper for debugging, this is an [online Enigma smiluator](https://cryptii.com/pipes/enigma-machine).
You can use it to encrypt many things.

# Some useful backgrounds

## [Module arithmetic](https://en.wikipedia.org/wiki/Modular_arithmetic)

For a rotor that turns, `25 + 1 = 0 `. It means that if the rotor shows "Z" (25), and you tick it once, you get "A" (0). Mathmatically speaking, that's module arithmetic.

You might need some of the following properties when turning a rotor (`%` is modular in C):

`a + b == ((a % n) + (b % n)) % n`

The rule works the same for minus.
However, modular in C doesn't work quite the same as math. For example, `-1 % 3` may give you `-1` instead of `2`. (Depends on compiler, system, etc.) You can add `n` before every module operations. `(-1 + n) % n == n - 1` instead of a negative number.

You may check [C standard](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n3088.pdf) page 82, semantics of modular for more details.

## Type conversion and [ascii](https://en.wikipedia.org/wiki/ASCII)

`char` is a very tricky type in C. It could represent a character in a string, or an integer. When it is an integer, it may be 8-bit signed or 8-bit unsigned. This may complicate things a little bit.

Luckly, all ascii character can be represented as an 8-bit signed integer.
For example 'A' is 65, or 0x41 in hexadecimal.
Converting between `'A'` and `65` is easy in C, you do nothing.
If you do `char B = 'A' + 1`, you get `'B'` if you need a character (e.g. print it using `%c`), `66` if you need an integer (e.g. print it using `%d`).

That also means `char*` it could be:

- A pointer to a character or an 8-bit integer, who have target depend singness.
- A null terminated string, you can use `strlen` to calcualte its length.
- A buffer of characters or 8-bit integers, whose length is not known.
- A generic representation of a piece of memory, no single bytes really means anything, but together they could be casted to other types of pointers.

So be careful when you use them.

## [Unit testing](https://en.wikipedia.org/wiki/Unit_testing)

I find some students don't test their code as much. 
Therefore, we use unit testing to do grading in this homework.
You are also encouraged to write your own unit testing to see if your code works.

The framework we use is [GoogleTest](http://google.github.io/googletest/primer.html).
GoogleTest is a C++ testing framework, but it also works in C, mostly. 
("mostly" contains too many corner cases where it is not the case. I don't want to go into that, you can [read it](https://en.wikipedia.org/wiki/Compatibility_of_C_and_C%2B%2B) yourself, but for our use case GoogleTest is good enough)
Don't panic.
I have already taken care of the C++ part.

To write a test, you only need to:

```cpp
TEST(<TestGroup>, <TestName>) {
    // Your test code.
    int my_result = 1;

    // The following command to test if you code is running correctly
    EXPECT_EQ(my_result, 1);
    EXPECT_TRUE(my_result == 1);

    // ASSERT halt program immediately on error. That means you won't have a change to clean up the memory, leak sanitizer may not be too happy about it.
    ASSERT_EQ(my_result, 1);

    // Remember to clean up your memory!
}
```

GoogleTest will compile all your tests into one single binary executable.
In this homework, it is `EnigmaTests`.
Running `EnigmaTests` will execute all tests you have.
You can also use filters to run only a handful of tests:

```sh
./EnigmaTests --gtest_filter=<regex>
```

GoogleTest will find names whose name matchs the [regular expression](https://en.wikipedia.org/wiki/Regular_expression) you provided.

As a result, this homework doesn't have example input and output as well. I wrote some unit tests for you, but that's it. You should come up with your own unit tests, and pass them before submitting.

## [Shared library](https://en.wikipedia.org/wiki/Library_(computing)#Shared_libraries)

In this homework, we don't need a `main` function.
All the code you wrote will be packed together and linked into a shared library.
The end user will only see a header `Enigma.h` and your library. 
If they want to use Enigma, they only need to include `Enigma.h` in their code, call the functions in it, and link your library with their code.

This made code sharing easy while your implementation details kept secret.
But it also opens rabbit whole of memory issues.
Before you write any code, especially those visiable to the end users, i.e. everything in `Enigma.h`, think really hard what are the arguments?
When they pass a pointer, is it a valid object or NULL? What is the [lifetime](https://en.wikipedia.org/wiki/Object_lifetime) of the buffers?

For all you know, they could be coding at 2 am, drunk. &#x1F972;

## Debug mode

Some of you like to print the debug info to the screen, our autograder doesn't accept that.
Wouldn't it be nice if the printing works on your local machine, but doesn't execute on autograder?

Well, the answer is macro.

In `common.h` I defined a `DEBUGF`, you can use it as if `fprintf`. 
It prints to stderr, and only works when macro `DEBUG` is defined. There are two ways to define a macro `DEBUG`:

- Uncomment `common.h:11`, `#define DEBUG`. But that would mean any build will activate it.
- Pass it through cmake. Add `-DCMAKE_C_FLAGS=-DDEBUG -DCMAKE_CXX_FLAGS=-DDEBUG` when you are using cmake. It would define `DEBUG` when building.

# File structure & submission

Here's how all the files are structured:

- `lib`. You can put all your implementation here.
- `include`. You can put all your definitions here. You are free to add stuff to `common.h` and `Enigma.h`, but DO NOT modify anything that's anything in it. Otherwise your code may not compile on autograder. `Enigma.h` contains detailed requirements for each function.
- `tests`. You can put any tests you wrote here. Changes to `AutograderTests.cpp` will be ignored. You are encouraged to pass `ExampleTests.cpp` before you submit.

You should use the script provided to create a submission:

```sh
./generate_submission.sh
```

Submit the generated zip file to gradescope should work.

If you got `No such file or directory: './src/build_asan/Main'` on canvas, make sure you are:

1. Compiling your code with asan
2. Using GNU 11.3.0

# Coding guide

## For the bald

Well, you are free to do whatever you want. Just make sure don't change `common.h` and `Enigma.h`.

## [Incremental building](https://en.wikipedia.org/wiki/Incremental_build_model)

The design of the whole project is pretty self-explanatory.
For each component, you can code it, then test it with your own unit testing.
You should then put the component into `Enigma` and use that component in `encrypt_Enigma`.
`EnigmaTests` have many example tests that test each components. You should pass them before submitting.

### Plugboard

Let's start with the easy part shall we? 
Plugboard is simple, it exchanges two letters, that's it.

When creating enigma `new_Enigma`, `size_t num_pairs` and `const char* pairs` is used to describe the pairs of letters that we want to exchange. You can assume `num_pairs` will be smaller than the buffer size of `pairs`.
If any configuration is wrong, you should return NULL.

Now that you've finished the plugboard, you can write tests to see if it works as you wish.
If it does, you can create an `Enigma`!
Although it still needs rotors and reflectors, you can put plugboard in the machine. You can do it by creating a field in `Enigma` that represent plugboard, and initialize it when `Enigma` is created.
This way, autograder can test if it works.

### Reflector

Reflector is more complicated than Plugboard. It shuffles the letters.
`const char* reflectors` in `new_Enigma` is used to describe it.
It will always (let's assume the user is sane enough) be a buffer of size 27. The buffer will describe how will the letter be shuffled. 
For example, if the 0-th char is `B`, that means `A` is shuffled to `B`

Again, test if your shuffle works on `I_UKW_A` listed in `common.h`.
Then put the reflector in your `Enigma`.

### Rotor initializion and setting

In `new_Enigma`, we provide you with the number of rotors (`size_t num_rotors`), a set of rotors (`const char** rotors`), and their initial settings (`size_t* inits`).
You can assume `num_rotors` is correct and won't cause overflow.
Each rotor is described using a null terminated string. The first 26 characters shows [the wiring](https://www.cryptomuseum.com/crypto/enigma/wiring.htm), followed by a comma, then the notchs location. As shown in `common.h`, a rotor may have multiple notches (VIII), or no notches (Beta). 

The actual machine has a small window to show the letter of the rotor. Let's call that setting (`inits`). Should you put multiple rotors in the machine, the user should be able to get the current setting. Therefore, please implement `get_setting_Enigma` first. You can assume the buffer provided is always valid.

The user should also be able to `reset_rotors_Enigma`. Enigma is symmetric. Reset the setting and type the encrypted message is how you do decryption. 


### Rotor tick

After each key press, the rotor turns once, or `tick`. After each `tick`, the setting changes accordingly. `tick_rotors_n_Enigma` press the keys of enigma for `n` times. Check how `tick` and notch works [here]((https://www.cryptomuseum.com/crypto/enigma/wiring.htm))

Hint: for `tick_rotors_n_Enigma`, do you really need to `tick` it n times, or can you compute the setting and then `reset_rotors_Enigma`?

### Rotor connect 

Let's not worry about the rings for now. The string described the mapping, like reflectors.
However, notice that rotors can be connected backwards. That means you need a revers look up: "For output X, what letter does the rotor converts to X?"
For the tests you write here, you can set `rings` to `A` to ignore the effect of rings.

Ring is an offset of the mapping. Imagine shifting the mapping by `ring` characters. Try to figure out (through the docs or simulator) how that would affect the encryption?

# Grading

For each section described in the [coding guide](#coding-guide), the autograder has a unit testing for it. So you get partial credits even if you can't finish everything.

We are still going to grade you with AddressSanitizer, so make sure your code is memory safe.

# Disclaimer

Engineerers often make the mistake called overengineering[[1](https://xkcd.com/974/), [2](https://en.wikipedia.org/wiki/Overengineering)]. 
This HW is no different. 
For example, `Reflector` is just a `Rotor` that doesn't rotate. 
As a matter of fact, you probably realized that `Reflector`'s code look a lot alike `Rotor`, but simpler.

The final result is (if you got all credits on this homework), you wrote a superset of Enigma.
You can put rotor III, II, I, reflector UKW-B, and any number of cables in the plugboards, then you get an Enigma I.
You can also use rotor Beta and reflector UKW-C to make an Enigma M4 (Used by U-Boat)
You can even define your own rotors, and put any number of them in your machine to make it super complicated, the kind of machines that is never build, and probably will never be used.
(Mind you, we do have tests for this)

However, I designed this purposfully for some reasons:

- I want to simplify the HW. If I ask you to write a `enigma.c` with 300 lines of code (LoC) and not tell you where to start, what component to write, you're probably lost
- I hope you can understand what is a decent project structure and how to do unit testing... although this is probably part of Software Enigneering.
- I don't want you to copy algorithms online, as our solution is more capable than of theirs &#x1F600;

# Difficulty estimation

Many of you like to ask how long it would take to finish the homework. 
I understand approaching final you might need better estimations for each task. 
So here it is:

It took me three days and 20+ hours to put all the pieces together. 
That includes finding references, program reference code, debugging and testing, writing this README and prepare autograder. 
You don't really need to find references, as I have linked most useful resources here, and you don't need to write documentations (although highly recommended) as well. 
But you do need to write the code, learn GoogleTest and write your own test cases, and debug your code. 
Debugging takes a lot of time.

The program itself is pretty short. My reference code includes ~100 LoC headers, ~200 LoC C code and ~250 LoC C++ code. 
Like I mentioned in [unit testing](#unit-testing), all C++ code are unit testing codes. 
It's pretty normal to write more test code than the code itself. After all, there are many cornor cases.

So this is the time and energy I spent to do this HW, hope it helps you plan your finals. &#x1F600;