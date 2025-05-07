#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/elf.h>

void print_phdr_content(int fd, int Phdr_size, Elf64_Off phdr_offset) 
{
	char *content_of_phdr_section = malloc(Phdr_size);
	int i ;

	if (lseek(fd, phdr_offset, SEEK_SET) < 0) {
		perror("lseek");
		exit(EXIT_FAILURE);
	}
	if (read(fd, content_of_phdr_section, Phdr_size) < 0) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("content of phdr section: \n");
	for (i = 0; i < Phdr_size; i++) {
		printf(" Ox%02x ", content_of_phdr_section[i]);
	}
	printf("\n");
}



void print_shdr_content(int fd, int size, Elf64_Off offset)
{
	char *content_of_section = malloc(size);
	int i ;

	if (lseek(fd, offset, SEEK_SET) < 0 ) {
		perror("lseek");
		exit(EXIT_FAILURE);
	}
	if (read(fd, content_of_section, size) < 0 ) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("content of section shdr: \n");
	for (i = 0; i < size; i++) {
		printf(" 0x%02x ", content_of_section[i]);
	}
	printf("\n");
}

void parse_shdr(int fd, Elf64_Off shdr_offset) {	
	Elf64_Shdr Shdr;

	printf("addr: 0x%04x\n", shdr_offset);
	if (lseek(fd, shdr_offset, SEEK_SET) < 0) {
		perror("lseek");
		exit(EXIT_FAILURE);
	}
	if (read(fd, &Shdr, sizeof(Shdr)) < 0) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("TATA: 0x08x\n", Shdr.sh_offset);
	printf("name: 0x%04x\n", Shdr.sh_name);
	printf("type: 0x%04x\n", Shdr.sh_type);
	printf("addr: 0x%08x\n", Shdr.sh_addr);
	printf("Offs: 0x%08x\n", Shdr.sh_offset);
	printf("size: 0x%08x\n", Shdr.sh_size);
	print_shdr_content(fd, Shdr.sh_size, Shdr.sh_offset);
}

void parse_phdr(int fd, Elf64_Off phdr_offset) {

int Phdr_size;
	Elf64_Phdr Phdr;
	printf("addr: 0x%08x\n", phdr_offset);
	if (lseek(fd, phdr_offset, SEEK_SET) < 0) { 
		perror("lseek");
		exit(EXIT_FAILURE);
	}
	if (read(fd, &Phdr, sizeof(Phdr) < 0)) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("TOTO: 0x%08x\n", Phdr.p_offset);

printf("type: 0x%04x\n", Phdr.p_type);
printf("addr: 0x%08x\n", Phdr.p_vaddr);
printf("Xword: 0x%08x\n", Phdr.p_filesz);
printf("Xwordmem: 0x%08x\n", Phdr.p_memsz);
print_phdr_content(fd, Phdr_size, phdr_offset);
}	

int main(int c, char *v[])
{
    int fd;
    int i;
    int nbread;
    int offset;
    Elf64_Ehdr ElfHdr;
    

    if ((fd = open("/bin/ls", O_RDONLY)) < 0) {
	perror("open");
	exit(EXIT_FAILURE);
    }
if (lseek(fd, 0, SEEK_SET) < 0) {
	 perror("lseek");
 	exit(EXIT_FAILURE);  
	}
    if ((nbread = read(fd, &ElfHdr, sizeof(ElfHdr))) < 0) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    if (nbread != sizeof(ElfHdr)) {
        perror("Header bad size");
        exit(EXIT_FAILURE);
    }
    if (ElfHdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ElfHdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ElfHdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ElfHdr.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "It's not a valid ELF file cause magic not matched\n");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // classe none, 32, 64 bits
    printf("Class:  ");
    switch (ElfHdr.e_ident[EI_CLASS]) {
        case ELFCLASS32: printf("ELF32\n"); break;
        case ELFCLASS64: printf("ELF64\n"); break;
        default:         printf("classe non connu\n"); break;
    }

    // Endianesss DATA 
    printf("Data:   ");
    switch (ElfHdr.e_ident[EI_DATA]) {
        case ELFDATA2LSB: printf("Little Endian\n"); break;
        case ELFDATA2MSB: printf("Big Endian\n"); break;
        default:          printf("Not Endian\n"); break;
    }

    // EI_VERSION ( EV_CURRENT )
    printf("Version: ");
    if (ElfHdr.e_ident[EI_VERSION] == EV_CURRENT) {
        printf("1 (current)\n");
    } else {
        printf("(invalid)\n");
    }

    // EI_OSABI if the kernel was freebsd, linux, unix etc...
    printf("OS: ");
    switch (ElfHdr.e_ident[EI_OSABI]) {
        case ELFOSABI_LINUX: printf("this machine used linux\n"); break; 
        case ELFOSABI_NONE:  printf("NONE this machine used systemv\n"); break; 
        default:             printf("other (%d)\n", ElfHdr.e_ident[EI_OSABI]); break;
    }

    // Version ABI (pour les changements mineur de l'os permet d'éviter les incompatibilités d'os)
    printf("ABI version: %d\n", ElfHdr.e_ident[EI_VERSION]);
    printf("SHNUM: 0x%02x\n", ElfHdr.e_shnum);
    parse_phdr(fd, ElfHdr.e_phoff);
    for (i = 0; i < ElfHdr.e_shnum; i++ ) {
            offset = ElfHdr.e_shoff+(i*sizeof(Elf64_Shdr));
	    printf("SECTION NUMERO: %d\n", i);
	    parse_shdr(fd, offset);
    }
    if (close(fd) == -1) {
	    perror("close");
	    exit(EXIT_FAILURE);
    }

    return 0;
}

