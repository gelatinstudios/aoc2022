
My solutions for Advent of Code 2022 done in C99 or otherwise if i feel.

# FAQs

(nobody's actually asked me any questions but if they did it might be these)

## How do these read the input file?

Stdin. I run these by piping the file with `<`, which is supported by most shells (not PowerShell apparently, which is why I just stick with cmd).

## Why is the style sloppy? Why doesn't this give *me* the correct solution for *my* input? etc.

The idea is that I stop coding when I've got the answers for *my* input, so they may be buggy and imperfect. That being said, I *do* make sure that each program gives the answers to *both* parts, so I don't just edit the code from part 1 to give me only part 2's answer. Other than that, I'm not really striving for perfect code here, I'm just having fun trying to solve the problems.

## Why C99?

I use [tcc](https://bellard.org/tcc/) to turn C into sort-of a scripting language. It can compile C code in memory and execute it without producing an exe file. Using Windows' ftype and assoc to set the shell to use tcc to run .c files, I can run each day's problem like this:

```
1.c < input_1.txt
```

I'm not sure how you might do this on *nix (or why you would really need to).
