# Tau Ceti

<p>
<a href="https://ci.appveyor.com/project/mupfelofen-de/tauceti">
  <img src="https://ci.appveyor.com/api/projects/status/hg87r0evoqu6lfgr?svg=true" alt="Build status" />
</a>
<a href="https://lgtm.com/projects/g/mupfelofen-de/TauCeti/alerts/">
  <img alt="Total alerts" src="https://img.shields.io/lgtm/alerts/g/mupfelofen-de/TauCeti.svg?logo=lgtm&amp;logoWidth=18?svg=true" />
</a>
<a href="https://github.com/mupfelofen-de/TauCeti/blob/master/LICENCE.md">
  <img src="https://img.shields.io/badge/licence-BEER--WARE-brightgreen?style=flat" alt="Licence" />
</a>
</p>

## About

Tau Ceti is a cross-platform 2D sci-fi themed tech demo written in C99
using SDL2 to demonstrate the capabilities of
[eszFW](https://github.com/mupfelofen-de/eszFW): a cross-platform game
development framework written in C.

[![Tau Ceti](.media/01-tn.png)](.media/01.png?raw=true "01")
[![Tau Ceti](.media/02-tn.png)](.media/02.png?raw=true "02")

## Android version

An Android version is available on Google Play:

[![Get it on Google Play](https://play.google.com/intl/en_us/badges/images/generic/en_badge_web_generic.png)](https://play.google.com/store/apps/details?id=de.mupfelofen.TauCeti)

## Controls

To play the game using the touch screen, simply hold your finger in the
direction you want to go.  To jump, swipe your finger up and to crouch,
hold your finger on the bottom of the screen.

```
F:       toggle fullscreen
Q:       quit
SPACE:   jump
LEFT:    walk left
RIGHT:   walk right
DOWN:    crouch
```

## Compiling

First clone the repository including the submodules:
```
git clone --recurse-submodules -j2 https://github.com/mupfelofen-de/TauCeti.git
cd eszFW
```

To compile _TauCeti_ under Linux use CMake as follows:
```
mkdir build
cd build
cmake ..
make
```

You can run the demo by entering:
```
ln -s ../res
./TauCeti
```

## Licence and Credits

This project is licenced under the "THE BEER-WARE LICENCE".  See the
file [LICENCE.md](LICENCE.md) for details.

[Warped City](https://ansimuz.itch.io/warped-city) by Luis Zuno
(@ansimuz).  Dedicated to [public
domain](https://creativecommons.org/publicdomain/zero/1.0/).

Every other work is also dedicated to [public
domain](https://creativecommons.org/publicdomain/zero/1.0/).
