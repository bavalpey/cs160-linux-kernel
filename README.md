## About the Linux Kernel

The linux source code can be found at git https://gitlab.com/linux-kernel/stable.git.
I have vendored the linux kernel in this repository. The reason I vendor this is to simplify
the process to generate a patch. You will *not* turn in the entire copy of the linux Kernel.

The entire size of the Linux kernel and all object files requires as much as 8GB.

This repository uses version 5.10.252 of the Linux kernel. This version of the linux kernel keeps the build process smooth.

Note that the Linux kernel must be compiled with the C89 standard. While this does not change very much about how you program, there is one important difference: variables can only be declared at the top a scope. The compiler will tell you this when you
try to compile it. However, this is something to be aware of.
There are other differences between C89 and the C we learned in class. However, you aren't likely to run into them.

Below I provide an example:
```c
void myFunction(void) {
    int x;
    int y;
    int z = 0;

    // This is permitted; x has been declared
    x = z * 2;

    // This is NOT permitted.
    int w = 4;
}
```

## Compiling the Linux Kernel

The first step before compiling the linux kernel is to get the dependencies that you need.
Also note that the linux kernel *must be compiled with gcc*. You cannot use clang, you cannot use ccache. It *must* be gcc, (and it must be version >= 13.3.0). If you are using OrbStack, this should already be configured.

#### Dependencies

Other than gcc, building the Linux kernel requires the following dependencies. You can install them with apt.

```
sudo apt-get install -y build-essential flex bison libelf-dev libssl-dev bc cpio
```

These are the dependencies you'll need to merely *build* the linux kernel.
To run it, you'll need to use QEMU, which is a virtual machine. Yes, we're using a virtual machine within a virtual machine.

Compiling the Linux kernel normally requires going through a process to determine its configuration. To ensure the kernel remains relatively light, and to ensure you have all of the components necessary for the assignments, I have provided the configuration you must use.

To compile the linux kernel, first, `cd` into the directory: `cd linux-5.4.284`. Then, run `make mrproper`. This will ready the repository.

1. cd into the Linux kernel directory: `cd linux-5.4.284`
2. Prepare the repository for a compile: `make mrproper`
3. Copy over the compile config `cp ../cs160.config .config`
4. Compile the linux kernel. `make -j$(nproc) bzImage`. This will take some time (it took my M4 Mac about 5 minutes).
  - The `-j` argument is a flag to Make that tells it the number of jobs to run in parallel. The `nproc` command evaluates to the number of logical processors on the machine. This will dramatically speed up the compile time.

Once you have compiled the first time, all you need to do is simply repeat the last command: `make -j$(nproc) bzImage`. This will automatically recompile anything that depends on files you modified.

## Booting the Linux Kernel

With the kernel built, we have to then run it. Make sure you are in this directory (not the `linux-5.4.284` directory from before). You can check which directory you are in with `pwd`.

To boot into the linux kernel, you'll need the QEMU virtual machine, and an openwrt disk image.
You *must* use the x86_64 virtual machine on QEMU.

Install QEMU like so:

```sh
sudo apt-get install -y qemu_system_x86
```

Next, we need a disk image. We are going to use [openwrt](https://archive.openwrt.org/releases/21.02.7/targets/x86/64/) version 21.02.7.

```sh
wget https://archive.openwrt.org/releases/21.02.7/targets/x86/64/openwrt-21.02.7-x86-64-generic-ext4-combined.img.gz
```

Make sure the checksums match:

```sh
echo "39f8e1f629cfe1db985809639c2cd1de88db119544144740d968d0b6784a9d6f  openwrt-21.02.7-x86-64-generic-ext4-combined.img.gz" | sha256sum -c -
```

You should see: `openwrt-21.02.7-x86-64-generic-ext4-combined.img.gz: OK`

Next, unzip it
```sh
gunzip openwrt-21.02.7-x86-64-generic-ext4-combined.img.gz
```

Note: The file is 121MB. **Do not commit this to version control**. It has been added to the `.gitignore` file, so you shouldn't have to worry about this.

With all of this done, all you need to do is launch qemu. The `start-qemu.sh` script I have provided will take care of this.
With the Linux kernel compiled, just run

```sh
./start-qemu.sh
```

Once in, you should type `uname -r`. If the output looks like below, you're good!

```
$ uname -r
5.10.252-cs160
```

The linux kernel is kept slim to ensure quick builds. There is no networking. There is also no compiler!
The programs that you are going to run on Linux need to be compiled outside of QEMU. When doing this, you *must* pass the `-static` flag as a compiler option.
This is because the Linux kernel may not have the same dynamic libraries.
The Makefile that I have provided does this for you. However, if you prefer to manually compile, just include `-static` as one of the options.

### Sharing files between your VM and your linux kernel

The `start-qemu.sh` script will make the folder in `shared` available to Linux. However, it still needs to be mounted!

From within the kernel, you will need to mount the folder to view it.

```
mount -t vfat /dev/sdb1 /mnt
```

Before powering off, don't forget to unmount it! (Otherwise you can run into issues)

```sh
umount /mnt
```

When you are done with the linux kernel, you can exit via
```sh
poweroff
```

### Tips

If you're editing the linux kernel in VSCode, you'll probably want good linting. Tom Roeder of Google created a script that will create a `compile_commands.json` file.
After building the linux kernel, you can get the compile commands with a script provided. You will need to do this from the root of the linux kernel directory.
`python3 scripts/gen_compile_commands.py`.
Then, copy this to the root of this repository.

You will need to use `clangd` as the language server. (I could not Microsoft's C/C++ language server to work nicely with the Linux kernel). 
This will generate the compile_commands.json file needed by `clangd` for code analysis. You must copy this file to the project root.

I have provided a `.clangd` file which will ensure clangd doesn't emit any errors or warnings.
