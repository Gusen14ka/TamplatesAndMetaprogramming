# # template Print

This brach implements utilities for beautifully outputting tuples (tuple-like types) into an output stream (`std::ostream`) using C++20 templates and concepts.

## Features

- Automatic output of the contents of tuples of any size via the overloaded operator `<<'.
- Using fold expressions and universal templates to support any tuple-like types.
- Compatible with `std::tuple', `std::pair` and custom tuple-like types.