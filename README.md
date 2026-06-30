<h1 align="center">meerOS - An operating system for x86_64 systems </h1>

<h2>- About </h2>
<div>This kernel was homemade with little to no AI at all (flex).

It is booted by the 
[Limine Bootloader](https://github.com/limine-bootloader/limine)
and use its protocol

It only supports x86_64 but soon I will add a support for x86_32 or any other architectures

</div>

<h2>- Compiling and Running</h2>

Before compiling you must have these tools installed
```sh
make		# makefile, for making..
x86_64-elf-gcc	# for compiling
x86_64-elf-*	# binutils and other tools
nasm		# for compiling .asm files
qemu		# for running and emulating our operating system
```

Running and Compiling:
```sh
# we clean first to remove all old version
# for uefi
make clean && make run-efi
# for bios:
make clean && make run-bios

# cleaning the tools
make clean-tools
# and for the build objs
make clean
```

