Overview
This header-only utility provides a unified add(a, b) function that works with:

Integral types (int, long, unsigned long, etc.)

Floating‑point types (float, double, long double)

String‑like types (std::string, std::string_view, C‑string literals)

By default C++’s “usual arithmetic conversions” can yield surprising result types (e.g. short + unsigned short → int). Here we override that for integral arguments with a custom rule.

Features
Custom Integral Deduction

Bigger‑type rule: the result type of adding two integrals always matches the larger of the two operand types by size.

Signed‑priority on mix: if one operand is signed and the other unsigned and both promote up to a common wider type (e.g. short+unsigned short → int), we instead return the signed version of the larger original type.

No hidden promotions: you won’t suddenly get a 32‑bit int when adding two 16‑bit shorts—ours respects the operand sizes.

Default Arithmetic
For all other numeric combinations (mixed integer & floating point, pure floats), we fall back to std::common_type_t<T, U>.

String Concatenation
A single overload for any “string‑like” arguments (std::string, std::string_view, const char*) that returns an efficient std::string concatenation.