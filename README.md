# DistroGUI

v.0.01 Usable Beta

DistroGUI is a graphical front end for Distrobox. Distrobox makes it easy to run other Linux distributions via the command line, without running in a virtual machine. It uses podman or docker containers, instead, giving you much better performance, and integrates with the system.

**Note:** *While this app is in a usable state, please understand that I consider it alpha software. It's far from where I want it to be for prime time. There are still more features I would like to add, and I can't garauntee everything will work perfectly. But this is in a usable state, and I think this is an alpha that you'll be ok with using if you want to.*

## Build Instructions

### Build YAD

We included the binary of yad with the source. However, you may want to build YAD from scratch. To comply with the GNU GPL, README.md in the yad folder has the build instructions. `cd yad` and follow the build isntructions provided in README.md there. When you are done, `cd ../` and continue with the instructions here for building the DistroGUI AppImage.

### Build DistroGUI AppImage

Build dependencies: appimagetool, unionfs, fuse, unionfs-fuse, bash (which comes with pretty much any Linux distribution)

This does use a standalone binary of [YAD 12.1](https://github.com/v1cont/yad), - https://github.com/v1cont/yad which is licenced under the GNU-GPL 3.0.  

1. Clone the reipository by typing `git clone https://github.com/fearlessgeekmedia/DistroGUI.git` in the terminal.
2. Type `cd DistroGUI`
3. `sudo ./tempenv.sh setup`
4. `sudo ./tempenv.sh start`
5. `appimagetool distrogui`

This will build the AppImage file which you can now run.

## To do

- Upgrade all containers at one time
- Error checking with container creation
- Figure out why, on occation, it doesn't list all Distrobox containers.
- Fix the error in the shortcut where it's telling you the wrong version number. 
- *Will be updated as I find other issues*
