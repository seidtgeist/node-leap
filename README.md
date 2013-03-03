# node-leap

A simple Leap server written in node.

This is my own unofficial server. The official server is bundled with the SDK.

The server uses a small C++ addon to access the Leap API and returns a JS object.

## Why?

So I can have all the C++ features in JS! The demo features screen
intersection support!

## Installation

1. Create a symlink called `sdk` in the root of this directory.
   It should point to the `LeapSDK` folder of the distribution, e.g.:
   ```
   ln -s path/to/LeapSDK ./sdk
   ```
2. `npm install`
3. `make`
4. `make run`
5. Visit http://localhost:1337/ (if you have problems check the console for errors)
6. Enable full-screen mode with `Command-Shift-F` and point fingers at the screen. COLORS APPEAR MAGICALLY!
