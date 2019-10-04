#! /bin/sh

LIBSDL="libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev"
LIBXML="libxml2-dev"

for pkg in $LIBSDL $LIBXML; do
    if dpkg --get-selections | grep -q "^$pkg[[:space:]]*install$" >/dev/null; then
        echo -e "$pkg is already installed"
    else
        apt-get -y -qq install $pkg
    fi
done
