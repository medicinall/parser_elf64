# 🧬 ELF Inspector

**ELF Inspector** is a low-level ELF parsing utility written in C that reads and prints details from ELF64 binary headers, program headers, and section headers. It serves as a minimalist but educational tool for understanding the internal structure of ELF files — especially useful for security researchers, reverse engineers, and students studying executable formats on Linux.

---

## 📋 Features

- 📦 Parses and validates ELF magic headers
- 🧠 Detects ELF class (32/64-bit), endianness, OS ABI, and version
- 📊 Reads and prints:
  - ELF header
  - Program Header (PHDR) details
  - Section Header (SHDR) details
- 🧾 Hex-dumps content of PHDR and SHDR segments
- 🧼 Uses only standard C libraries (`unistd`, `fcntl`, `stdio`, `stdlib`, etc.)

---

## 📦 Requirements

- Linux operating system (uses `<linux/elf.h>`)
- GCC or any C99-compatible compiler

### 📚 Headers Used

- `<fcntl.h>`
- `<stdio.h>`
- `<stdlib.h>`
- `<unistd.h>`
- `<string.h>`
- `<linux/elf.h>`

No external libraries are required. Only the Linux kernel's ELF header definitions are used for structure parsing.

---

## 🛠️ Compilation

Use `gcc` to compile the ELF Inspector:

```bash
gcc -o elf_parser elf_parser64.c
```
---

 ## 🚀  Usage


./elf_parser


The script is hardcoded to analyze /bin/ls, but you can modify this line in the main() function:
```
if ((fd = open("/bin/ls", O_RDONLY)) < 0)

```
