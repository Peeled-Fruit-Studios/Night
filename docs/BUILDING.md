<h1 align="center"> Build Instructions for Night Kernel </h1>

<div align="center">

**Note: The kernel can't be built on windows, so I reccomend you get [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10)**

</div>

## Using GN

GN is a Powerful and Open-Source build system used in many projects, such as [Fuchisa](fuchsia.dev) and [Google Chrome](google.com/chrome)

GN also uses Ninja-build as a backend but can also generate IDE project files (like Visual Studio and XCode).

Anyways, on to installation.


### Installing Ninja

To install Ninja, there are two ways.

Method 1

```` bash
$ sudo apt-get install ninja-build
````

Method 2

```` bash
$ pip install ninja
````

*Note: Only use the second method if you don't have administrator permissions*

### Installing GN

Sadly, GN is not availible as a package and you will have to install it manually from the links below

- [Linux](https://chrome-infra-packages.appspot.com/dl/gn/gn/linux-amd64/+/latest)
- [Mac OS](https://chrome-infra-packages.appspot.com/dl/gn/gn/mac-amd64/+/latest)

Once downloaded, unzip the file and move the GN binary into your /usr/bin directory

Now, we are ready to build!

## Building the kernel

To build the kernel run the following commands...

````bash
$ gn gen out
$ ninja -C out
````

To build a certain target (like libk)

````bash
ninja -C out <name of target>
````
You can boot the kernel (Location: out/kernel) with Grub (Or any other multiboot compliant loader)

The Initrd is also required as a boot module (Location: out/itd.tar)

As for booting/running the kernel, see [RUNNNING.md](RUNNNING.md)
