
Debian
====================
This directory contains files used to package seed2needd/seed2need-qt
for Debian-based Linux systems. If you compile seed2needd/seed2need-qt yourself, there are some useful files here.

## seed2need: URI support ##


seed2need-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install seed2need-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your seed2need-qt binary to `/usr/bin`
and the `../../share/pixmaps/seed2need128.png` to `/usr/share/pixmaps`

seed2need-qt.protocol (KDE)

