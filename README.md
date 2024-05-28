# GYS
A scripting language base. Ported from the original C++ on SourceForge to modern C++ and other languages. I love the idea of languages/engines that are built like this so I wanted to toy around with it.

The original is at https://gys.sourceforge.net/ and hasn't been updated since 2004.

## Contents
./gys-041 is the last release made available from Sourceforge.

./cpp brings gys-041 into 2024, using CMake to build statically-linked executables. CLion was used for the IDE for maximum portability.

Plans are for Java, Kotlin, C#, Swift, and other language ports once I get the C++ code working and try extending an example or two.



# Original Home Page Contents
GYS is the scripting engine which helps to create the embedded interpreter of your own language.

GYS has built-in lexical and expression analyzer, but during interpretation it calls different user code for executing commands, operators, conversions and value getting. The interpreter is fully runtime - it reads code and interprets it at once. GYS doesn't generate any intermediate code. User doesn't have to use stack to implement his GYS-derived script language.

The "abstract" GYS interpreter can be called within application to execute the script according to the designed language definition and functionality. The library provides an easy and clear way to define a scipt language interpreter step-by-step: developer can define next language elements:

* types
* operators (binary, prefix and postfix unary)
* conversions between values of different types
* built-in functions (commands)

Mainly this library can be used to create:

* embedded scripting language for your application
* standalone interpreter of your own language

### What is Standard GYS?
Standard GYS (stdGYS) is the C-like script language made using GYS library.

Its main features are:

* 8 basic types (void, bool, int, double, char, string, vector, function) with defined operators and conversions
* about 60 standard built-in functions (IO, math, vector functions, conversions, threading, script execution etc.)
* for, while, do loops
* if-else if-else and return statement
* dynamic half-nonuniform vectors
* functions as first-class members (e.g. can be passed and returned by other functions)
* multithreading
* recursion

This language is extensible and can be easily integrated into your application. Developer can use stdGYS or its parts as basis for his own projects.


