<h1 align="center"> Build Instructions for the Night Kernel </h1>

Before we get started, I have one thing to say...

If you are on windows, you are unable to build the kernel due to the fact that a elf-supporting compiler is required

Otherwise proceed on to the rest of the instructions

## Using GN

GN is a powerful and Open-Source build system used in many projects, such as [Fuchisa](fuchsia.dev) and [Google Chrome](google.com/chrome)

GN also uses Ninja-build as a backend but can also generate IDE project files (like Visual Studio Solutions).

Anyways,to build the kernel you will need to install GN and Ninja.


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

Once downloaded, unzip the file and move the GN binary into your /bin directory

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

The resulting kernel binary is out/kernel

**Note: I am still getting around to adding Bootable iso support**
