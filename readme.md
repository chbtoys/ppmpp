# ppmpp.hpp - C++ header-only draw/read/write 2D graphics using the standard library only.

Created by Håkan Blomqvist
Part of the book: https://leanpub.com/2ddrawingwithc17usingthestandardlibraryonly

More information TBD...

## Clone ppmpp and build test.cpp

```
git clone https://github.com/chbtoys/ppmpp.git
cd ppmpp
clang++ -std=c++20 test.cpp -o test
./test
```

NOTE: Uses **-std=c++20** for std::lerp when making bezier quadratic and cubic. Everything else works great with **-std=c++17**.

## License

MIT No Attribution

ppmpp.hpp - A header-only class to draw/read/write 2D graphics using only the standard library.
Version 1.0 release (3:rd of December 2022).
Copyright (c) 2022-2023 Håkan Blomqvist

For more information:
https://leanpub.com/2ddrawingwithc17usingthestandardlibraryonly

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
