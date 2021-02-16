---
layout: post
title:  "Implementing Dynamic Type System [2/2]"
date:   2021-01-04 03:14:00 +05300
categories: jekyll update
permalink: "/compiler-design/dynamic-type-system/2"
---

Todo:
 - introduction, (V8 using a method called pointer tagging)
 - implementations are in pure C

We've learned about [union tagging]({% post_url /compiler/2021-01-05-dynamic-type-system-1 %}) in the last post, (if you haven't read it, I recommend you to read it first). But the problem with using union tagging is that it takes a **lot** of space. Let's say the biggest type (in size) in our union is `std::string`, then our unions size would be 4 bytes for the tag plus the [size of](https://en.cppreference.com/w/cpp/language/sizeof) `std::string`. String size completely depends on the compiler and runtime library you're using but in general, it'll take up to 32 bytes, and in the debug version it's even worse. So our unions total size would be about 36 bytes (4 plus 32). If we're using only going to use booleans, we wasting 36 bytes for just a bit.

These variants are the particles in our program which flows here and there forever. It'll not only allocate almost 36 bytes (for each assignment, coping, calling, returning, etc) but also call the variant's constructor to initialize the values, and when its lifetime ends it calls the destructor also, which is quite inefficient. This could be optimized with a methodology called [memory tagging](https://arxiv.org/ftp/arxiv/papers/1802/1802.09517.pdf).

## Pointer tagging

Pointer tagging is one of the popular methods, Google's [V8](https://v8.dev/blog/pointer-compression) javascript engine, some of ruby's (C/C++) implementations are using this method to represent it's value, you could also use this method, however I'm not going to implement it now, so if you don't want to take a quck glimpse on it, just skip to the `Nan Tagging` below.

Most architectures arrange their data in memory to access data at memory offsets equal to a multiple of [word](https://en.wikipedia.org/wiki/Word_(computer_architecture)) using [alignment or padding](https://en.wikipedia.org/wiki/Data_structure_alignment). In a 32-bit architecture, a word is 32 bits = 4 bytes, while on a 64-bit architecture a word is 64 bits = 8 bytes. For example on a 32-bit architecture these addresses could be 0, 4, 8, 16, 32, ... for all of these addresses the last 2 bits of a pointer (32-bits) would be 00. Similarly, the last 3 bits of 64-bit architecture pointer would always be 000. We can use those bits as our tag value. The next method is called NaN tagging, let's deep dive into that, and I'm going to explain with C implementation.

## NaN tagging

The IEEE 754 standard specifies a double precision (64 bit) binary floating point as having first bit as sign, next 11 bits as exponent and the last 52 bits as fraction/mantissa. We don't need to know how that's represented as bytes but we ...
![ieee754.png](/assets/nan-tagging-ieee754.png)

nan-value
![nan-value.png](/assets/nan-tagging-nan-value.png)

var
![var.png](/assets/nan-tagging-var.png)