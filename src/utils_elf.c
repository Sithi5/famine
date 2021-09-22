/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_elf64.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/28 15:58:26 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/28 15:58:26 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void check_elf_header(t_woody *woody)
{
    if (woody->binary_data_size < sizeof(t_elf_ehdr))
    {
        error(ERROR_NOT_ELF, woody);
    }
    t_elf_ehdr *ehdr = (t_elf_ehdr *)woody->mmap_ptr;
    /*
    The first check we make to the Ehdr is whether it is an Elf64 file.
    This means that the first four bytes (e_ident[EI_MAG0..EI_MAG3])
    must equal 'ELF64_MAGIC' ("\x7FELF").
    For e_ident[EI_VERSION] we want EV_CURRENT.
    For e_ident[EI_OSABI] we want ELFOSABI_SYSV and
    for e_ident[EI_ABIVERSION] we want 0 (zero).
    */
    if (!(ehdr->e_ident[1] == ELFMAG1 &&
          ehdr->e_ident[2] == ELFMAG2 &&
          ehdr->e_ident[3] == ELFMAG3 &&
          ehdr->e_ident[EI_VERSION] == EV_CURRENT &&
          ehdr->e_ident[EI_OSABI] == ELFOSABI_SYSV &&
          ehdr->e_ident[EI_ABIVERSION] == 0 &&
          ehdr->e_ident[EI_PAD] == 0))
    {
        error(ERROR_NOT_ELF, woody);
    }
    // Checking if class is well define.
    if (ARCH_32 && ehdr->e_ident[EI_CLASS] != ELFCLASS32)
    {
        error(ERROR_NOT_ELF32, woody);
    }
    else if (ARCH_64 && ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        error(ERROR_NOT_ELF64, woody);
    }

    // Check if file have already been infected
    if (ehdr->e_ident[EI_PAD + 3] == 7)
    {
        error(ERROR_FILE_IS_ALREADY_INFECTED, woody);
    }
    /*e_ident[EI_DATA] to equal ELFDATA2LSB (little-endian data structures).*/
    if (!(ehdr->e_ident[EI_DATA] == ELFDATA2LSB))
    {
        error(ERROR_ELF_NOT_LITTLE_ENDIAN, woody);
    }
    /*Next check if the EIF type is an executable or a shared library e_type == ET_EXEC or ET_DYN.*/
    if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
    {
        error(ERROR_NOT_EXECUTABLE_BINARY, woody);
    }
}

void set_string_table_ptr(t_woody *woody)
{
    t_elf_shdr *sh_table;
    sh_table = woody->mmap_ptr + woody->ehdr->e_shoff;
    woody->string_table_ptr = woody->mmap_ptr + sh_table[woody->ehdr->e_shstrndx].sh_offset;
}
