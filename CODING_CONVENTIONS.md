# Coding Conventions

## CPP Specific
Use C++20 Semantics, like std::ranges, modules, move semantics, etc.

## Function naming and arguments
Functions should be named like a verb. Names for the functions and parameters
should be sufficiently informative for user to understand the purpose of such
parameter or function.

## Doc comments
Before writng any docs for class or function, you should think if such doc
if necessary. If it is, write simple, small docs, explaining why such class or
function was created. 

If large doc is really needed add @brief with @params, @tparams and @return
Explaining the whole, if you think that user is not going to understand the code
without such docs.

No doc inside function or class explaining what such part of the code is doing.
Code itself should be sufficienly informative what it is doing. Function naming as
well indicated what it is doing.

## Building functions
Split functions into smaller ones. If you detected that function is doing 
something AND something, split function into two on this AND word. In classes
you can build helper functions inside private scope of the class, or private 
namespaces inside the file.

Each function should be descriptive enough that other reader is going to understand
what the function is actually doing. 

## Documenting the development
In /dev directory create a file that is going to be named with quick
title what was done. Inside the file at the start give a title what was done
and the date when it was done. 

Inside add descriptive text what was done, why it was done like that, what other 
paths we can take to develop described feature. You can add pieces of code that
are going to make easier to understand what was the main idea that stays behind
the update/new code.

Add as well potential problems with such solution and advantages.

## Naming
Private members: snake_case_ with a trailing underscore. Types: PascalCase.
Free functions and methods: snake_case.
