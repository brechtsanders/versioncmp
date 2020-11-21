# Version string compare for C
The versioncmp library is a C library providing string compare functions to compare strings while taking version numbers into accounts. These can be used for sorting data containing version numbers.

When sorting strings alphabetically numbers are also sorted alphabetically.
For example `10` will come before `2`, because the text comparison works from left to right, and `1` is smaller than `2`.
When using version strings (e.g. `2.10.5`) the comparison function should detect whole numbers and compare those numerically.
The non-numeric parts of the strings are still compared alphanumerically.

This library has no external dependancies.
