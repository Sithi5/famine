# woody-woodpacker
Project whose principle is to modify the headers of an ELF64 type file, encrypt the original file and insert a payload to decrypt the file before executing it.

![hacking gif](./resources/hacking.gif)

# Usefull commands
```bash
readelf -h $(binary) # Display elf-header information for binaries files
readelf -l $(binary) # Display program-header info
readelf -S $(binary) # Display section header
readelf --symbol $(binary) # Display symbol information for binaries files
objdump -d $(binary) # Disassemble a binary file and show usefull info
```

# Build and run
```bash
make all
./woody-woopacker (binary_file)
./woody
```

# Testing
```bash
make tests
```

# Resources
#### elf file
* [elf.h](https://code.woboq.org/linux/include/elf.h.html)


#### Infection method
* [UNIX ELF Parasites and virus](https://ivanlef0u.fr/repo/madchat/vxdevl/vdat/tuunix02.htm)
* [Malware Engineering Part 0x2— Finding shelter for parasite](https://medium.com/analytics-vidhya/malware-engineering-part-0x2-finding-shelter-for-parasite-751145dd18d0)
* [Linux x64 Infection for Lamers (by a Lamer)](https://vx-underground.org/archive/VxHeaven/lib/vjp01.html)
* [Return To Original Entry Point Despite PIE](https://tmpout.sh/1/11.html)

#### Encryptions
* [blue-team-vs-red-team-how-to-run-your-encrypted-binary-in-memory-and-go-undetected](https://www.redtimmy.com/blue-team-vs-red-team-how-to-run-your-encrypted-binary-in-memory-and-go-undetected/)
* [dcode.fr/rc4-cipher](https://www.dcode.fr/rc4-cipher)
* [wikipedia.org/wiki/RC4](https://fr.wikipedia.org/wiki/RC4)
* [geeksforgeeks.org/implementation-of-rc4-algorithm](https://www.geeksforgeeks.org/implementation-of-rc4-algorithm/)
