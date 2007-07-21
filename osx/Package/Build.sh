#!/bin/bash

DIST=./Dist
EXTRAS=./Extras
RESOURCES="$EXTRAS/Resources"
LPROJ="$RESOURCES/English.lproj";
SRCDIR=../../src
MANDIR=../../man
ROOT=./Root
PKG=./Pkg/Info

# Need to have absolute path so the 'default' utility sees them
INFO_PLIST=$(pwd)/Pkg/Info
DESC_PLIST=$(pwd)/Pkg/Description

myself=$(basename $0)

if [ -z "$1" ]; then
    echo "Usage: $myself <version>"
    exit
fi

echo "Checking version... $version"

version=$1
version_major=$(echo "$version" | cut -d. -f1);
version_minor=$(echo "$version" | cut -d. -f2);
version_full="$version_major.$version_minor.0";


DISTOUT="$DIST/gbsed v$version"
echo "Creating dist dir $DISTOUT..."
mkdir -p "$DISTOUT";

# Generate package information.
(
    echo "Generating package extras..."
    mkdir -p "$LPROJ"
    for file in $EXTRAS/*.template;
    do
        bname=$(basename $file);
        rtf=$(echo "$bname"|perl -ne's/\.template$//;print');
        cp -f "$EXTRAS/$bname" "$LPROJ/$rtf";
        perl -pi -e"s/%%VERSION%%/$version/g" "$LPROJ/$rtf";
    done
)

# Copy dist files.
echo "Copying distribution files."
mkdir -p "$ROOT"
mkdir -p "$ROOT/usr/bin"
mkdir -p "$ROOT/usr/include"
mkdir -p "$ROOT/usr/lib"
mkdir -p "$ROOT/usr/man/man1"
mkdir -p "$ROOT/usr/man/man3"

cp -f "$SRCDIR/gbsed/gbsed"         $ROOT/usr/bin/
cp -f "$SRCDIR/libgbsed/libgbsed.a" $ROOT/usr/lib/
cp -f "$SRCDIR/libgbsed/libgbsed.h" $ROOT/usr/include/
cp -f "$MANDIR/gbsed.1"             $ROOT/usr/man/man1/
cp -f "$MANDIR/libgbsed.3"          $ROOT/usr/man/man3/

echo "Removing DS_Store files from dist root..."

find "$ROOT" -name ".DS_Store" -exec rm -f \"{}\" \;

# Change version and such in the package's propertylist

echo "Setting up version and bundle information..."

cp -f "$INFO_PLIST.plist.template" "$INFO_PLIST.plist";
cp -f "$DESC_PLIST.plist.template" "$DESC_PLIST.plist";

defaults write "$INFO_PLIST" CFBundleGetInfoString      -string "0x61736b.net"
defaults write "$INFO_PLIST" CFBundleIdentifier         -string "net.0x61736b.gbsed"
defaults write "$INFO_PLIST" CFBundleShortVersionString -string "$version_full";
defaults write "$INFO_PLIST" IFMajorVersion             -int    "$version_major";
defaults write "$INFO_PLIST" IFMinorVersion             -int    "$version_minor";
defaults write "$DESC_PLIST" IFPkgDescriptionTitle      -string "gbsed v$version";


# Build package

echo "Building package..."

PackageMaker -build -p "$DISTOUT/gbsed v$version Installer.pkg" \
    -f Root -i Pkg/Info.plist -d Pkg/Description.plist      \
    -r "$RESOURCES"

echo "Copying DSstore to installer folder..."

cp -f ./DStore/DS_Store "$DISTOUT/.DS_Store"

dmgname="$DIST/gbsed.v$version.static.universal.osx.dmg"
echo "Creating disk image to $dmgname";
hdiutil create -fs HFS+ -volname "gbsed v$version" -scrub   \
    -imagekey zlib-level=9 -srcdir "$DISTOUT" -format UDZO  \
    -o "$dmgname"

echo "Setting the internet-enable flag..."
hdiutil internet-enable "$dmgname"

echo "Cleaning up..."

rm -rf "$RESOURCES"
rm -f  "$INFO_PLIST.plist"
rm -f  "$DESC_PLIST.plist"
rm -rf "$ROOT"
rm -rf "$DISTOUT"

echo "Done!"
