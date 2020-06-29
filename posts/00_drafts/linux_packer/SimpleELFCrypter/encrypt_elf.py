#!/usr/bin/python2
from pwn import *
import os

def encryptSection(e):
	cryptSection = e.get_section_by_name('.elf')
	baseAddr = cryptSection['sh_offset']
	sz = cryptSection['sh_size']

	print("Entry point @ 0x{:02X}".format(e.address))
	print("Crypted Section @ 0x{:02X} - 0x{:02X} Size: {}B".format(baseAddr, baseAddr + sz, sz))

	secData = e.read(e.address + baseAddr, sz)

	encryptedData = [chr((ord(x)^0xe3)) for x in list(secData)]

	print(hexdump(secData))
	print("Encrypted:")
	print(hexdump(encryptedData))

	e.write(e.address + baseAddr, ''.join(encryptedData))

e = ELF("./a.out")
encryptSection(e)

e.save("encrypted.elf")
os.chmod("encrypted.elf", 0777)
