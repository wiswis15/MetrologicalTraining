# Agreements on code

## Coding style

* We use the [Wekbit coding style](https://webkit.org/code-style-guidelines/) as default. Make sure whenever you check 
in, to adhere to this coding style.

  Rationale: Readability increases when only one coding style is employed.
  
## Compiler features

* Do not use #pragma once, rather use a plain old #ifdef/#define/#endif guard.

  Rationale: see [here](http://stackoverflow.com/questions/787533/is-pragma-once-a-safe-include-guard)
  
## C++11 coding

* Use the C++11 keyword **override** to signal the compiler that your are overriding a virtual method.
 
  Rationale: **override** protects you from thinking you override a method while you don't due to a different signature. 
  If you use **override**, the compiler will check whether there is a method with the exact same signature that you can 
  override (i.e. it is **virtual** and not **final**).
  
* Use the C++11 keyword **final** to signal a method should not be overridden.

  Rationale: **final** protects your method from being overridden. This is a new language construct in C++, and the 
  compiler will enforce it.
  
## Coding guidelines

* Don't be over-exposing, keep internal details internal

* Do not expose platform specific details
