all:
	gcc cryptme.c -fomit-frame-pointer -O0 -Wl,-e__decryption_stub -no-pie
	python2 encrypt_elf.py
