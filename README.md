# DistroGUI

v.0.01 Usable Beta

DistroGUI is a graphical front end for [Distrobox](https://github.com/89luca89/distrobox). Distrobox makes it easy to run other Linux distributions via the command line, without running in a virtual machine. It uses podman or docker containers, instead, giving you much better performance, and integrates with the system.

**Note:** *While this app is in a usable state, please understand that I consider it alpha software. It's far from where I want it to be for prime time. There are still more features I would like to add, and I can't garauntee everything will work perfectly. But this is in a usable state, and I think this is an alpha that you'll be ok with using if you want to.*

## Build Instructions

### Build YAD

1. Open the terminal and put in `git clone https://github.com/fearlessgeekmedia/DistroGUI.git`.
2. `cd DistroGUI/yad`
3. Follow the build instructions of the [README.md](./yad/README.md) in the Yad directory. Make sure you use the `--enable-standalone` option with the `./configure` command, assuming you want to package yad with this. Otherwise, you can just build yad like normal. Don't do `make install` if you're doing `--enable-standalone.` If you're packaging this for distribution, you should use `--enable-standalone` in case the user does not have yad on their system.
4. Assuming you've done the standalone version of yad, `cp yad ../ && cd ../`

Now you can continue to building the DistroGUI AppImage

### Build DistroGUI AppImage

Build dependencies: appimagetool, unionfs, fuse, unionfs-fuse

Other dependencies: Distrobox (of course), bash (which comes with pretty much any Linux distribution)

1. Assuming you've followed the instructions for building yad, you should now be in the appropriate directory. If you're not, go back and build yad. Or if you have yad already installed on your system, you may skip that part, but be sure to at least run `git clone https://github.com/fearlessgeekmedia/DistroGUI.git` first. Keep in mind, if you're building the AppImage without building yad, yad will need to be installed already on whatever system you're putting it on.
2. If you're skipping building yad because you already have yad installed., just type `cd DistroGUI`. If you've followed the yad build instructions, you're already in the right direcotry.
3. `sudo ./tempenv.sh setup`
4. `sudo ./tempenv.sh start`
5. `appimagetool distrogui`

This will build the AppImage file which you can now run.

## To do

- Upgrade all containers at one time
- Error checking with container creation
- Figure out why, on occasion, it doesn't list all Distrobox containers.
- Fix the error in the shortcut where it's telling you the wrong version number. 
- Make it possible to export applications and binaries via the GUI
- Create containers with --init to allow init systems in containers with images that allow it
- *Will be updated as I find other issues*
