# Tau Ceti

<p>
<a href="https://github.com/mupfelofen-de/TauCeti">
  <img src="https://img.shields.io/badge/project-GitHub-blue?style=flat?svg=true" alt="GitHub project" />
</a>
<a href="https://github.com/mupfelofen-de/TauCeti/blob/master/LICENCE.md">
  <img src="https://img.shields.io/badge/licence-BEER--WARE-blue?style=flat?svg=true" alt="Licence" />
</a>
<a href="https://ci.appveyor.com/project/mupfelofen-de/tauceti">
  <img src="https://ci.appveyor.com/api/projects/status/hg87r0evoqu6lfgr?svg=true" alt="Build status" />
</a>
<a href="https://www.codacy.com/manual/mupf/TauCeti?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=mupfelofen-de/TauCeti&amp;utm_campaign=Badge_Grade">
  <img src="https://app.codacy.com/project/badge/Grade/4c19342f6cda4cc3b8b9bbb773406599" alt="Codacy Badge" />
</a>
</p>

## About

Tau Ceti is a cross-platform 2D sci-fi themed tech demo written in C99
to demonstrate the capabilities of
[eszFW](https://github.com/mupfelofen-de/eszFW): a cross-platform game
engine written.

[![Tau Ceti](https://media.eszfw.de/tc-01-tn.png)](https://media.eszfw.de/tc-01.png?raw=true "Tau Ceti 1")
[![Tau Ceti](https://media.eszfw.de/tc-02-tn.png)](https://media.eszfw.de/tc-02.png?raw=true "Tau Ceti 2")

## Status

I am currently completely rewriting this project.  Expect nothing until
the next release!

## Android version

An Android version is available on Google Play:

[![Get it on Google Play](https://play.google.com/intl/en_us/badges/images/generic/en_badge_web_generic.png)](https://play.google.com/store/apps/details?id=de.mupfelofen.TauCeti)

## Controls

To play the game using the touch screen, simply hold your finger in the
direction you want to go.  To jump, swipe your finger up and to crouch,
hold your finger on the bottom of the screen.

```text
F:       toggle fullscreen
Q:       quit
SPACE:   jump
LEFT:    walk left
RIGHT:   walk right
DOWN:    crouch
```

## Compiling

First clone the repository including the submodules:
```bash
git clone --recurse-submodules -j2 https://github.com/mupfelofen-de/TauCeti.git
cd eszFW
```

To compile _TauCeti_ under Linux use CMake as follows:
```bash
mkdir build
cd build
cmake ..
make
```

You can run the demo by entering:
```bash
ln -s ../res
./TauCeti
```

## Licence and Credits

This project is licensed under the "The MIT License".  See the file
[LICENSE.md](LICENSE.md) for details.

[Warped City](https://ansimuz.itch.io/warped-city) by Luis Zuno
(@ansimuz).  Dedicated to [public
domain](https://creativecommons.org/publicdomain/zero/1.0/).

Every other work is also dedicated to [public
domain](https://creativecommons.org/publicdomain/zero/1.0/).
