multAssembly.out: multAssembly.o
	ld -melf_i386 -o multAssembly.out multAssembly.o
multAssembly.o: multAssembly.s
	as --32 --gstabs -o multAssembly.o multAssembly.s
	
clean:
	rm -fr *.o *.out
