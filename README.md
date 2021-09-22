# [Famine](./subject/fr.subject.pdf)
Famine is a binary of your design which will have to modify one or more binary(s) to apply additional functionalities to it, without altering the initial behavior of said binary. For once, we will limit ourselves to adding a "signature" to this binary and nothing else.

![hacking gif](./resources/panda_hacker.gif)

# Usefull commands
```bash
readelf -h $(binary) # Display elf-header information for binaries files.
readelf -l $(binary) # Display program-header info.
readelf -S $(binary) # Display section header.
readelf --symbol $(binary) # Display symbol information for binaries files.
objdump -d $(binary) # Disassemble a binary file and show usefull info.
strings $(binary) # display the binary strings.
```

# Build and run
```bash
make all
./famine
```

# Testing
```bash
make tests
```

# Resources

#### c programming

* [lstat](https://man7.org/linux/man-pages/man2/lstat.2.html)

#### elf file
* [elf.h](https://code.woboq.org/linux/include/elf.h.html)

#### Infection method
* [UNIX ELF Parasites and virus](https://ivanlef0u.fr/repo/madchat/vxdevl/vdat/tuunix02.htm)
* [Malware Engineering Part 0x2— Finding shelter for parasite](https://medium.com/analytics-vidhya/malware-engineering-part-0x2-finding-shelter-for-parasite-751145dd18d0)
* [Linux x64 Infection for Lamers (by a Lamer)](https://vx-underground.org/archive/VxHeaven/lib/vjp01.html)
* [Return To Original Entry Point Despite PIE](https://tmpout.sh/1/11.html)
