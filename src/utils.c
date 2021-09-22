/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:36:50 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:50 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void set_famine_ptrs_to_null(t_famine *famine)
{
    famine->mmap_ptr = NULL;
    famine->string_table_ptr = NULL;
    famine->binary_data_size = 0;
    famine->payload_data = NULL;
    famine->payload_size = 0;
    famine->infected_file = NULL;
    famine->cipher = NULL;
    famine->encryption_key = NULL;
}

/* Free the famine structure properly. */
void free_famine(t_famine *famine)
{
    if (famine)
    {
        famine->mmap_ptr ? munmap(famine->mmap_ptr, famine->binary_data_size) : 0;
        famine->payload_data ? free(famine->payload_data) : 0;
        famine->infected_file ? free(famine->infected_file) : 0;
        famine->cipher ? free(famine->cipher) : 0;
        famine->encryption_key ? free(famine->encryption_key) : 0;
        free(famine);
    }
}

void print_famine_infos(t_famine *famine)
{
    (void)famine;
    // printf("\n");
    // printf("Infos:\n");
    // printf("\tARCHITECTURE:\t\t\t\t\t%s\n", ARCH_32 ? "ARCH_32" : "ARCH_64");
    // printf("\tPAGE_SIZE:\t\t\t\t\t%u\n", PAGE_SIZE);
    // printf("\tPAYLOAD_SIZE:\t\t\t\t\t%lu\n", famine->payload_size);
    // printf("Elf ehdr:\n");
    // printf("\tfamine->ehdr->e_phoff:\t\t\t\t%lu\n", famine->ehdr->e_phoff);      /* Program header table file offset */
    // printf("\tfamine->ehdr->e_shoff:\t\t\t\t%lu\n", famine->ehdr->e_shoff);      /* Section header table file offset */
    // printf("\tfamine->ehdr->e_flags:\t\t\t\t%u\n", famine->ehdr->e_flags);       /* Processor-specific flags */
    // printf("\tfamine->ehdr->e_ehsize:\t\t\t\t%u\n", famine->ehdr->e_ehsize);     /* ELF header size in bytes */
    // printf("\tfamine->ehdr->e_phentsize:\t\t\t%u\n", famine->ehdr->e_phentsize); /* Program header table entry size */
    // printf("\tfamine->ehdr->e_phnum:\t\t\t\t%u\n", famine->ehdr->e_phnum);       /* Program header table entry count */
    // printf("\tfamine->ehdr->e_shentsize:\t\t\t%u\n", famine->ehdr->e_shentsize); /* Section header table entry size */
    // printf("\tfamine->ehdr->e_shnum:\t\t\t\t%u\n", famine->ehdr->e_shnum);       /* Section header table entry count */
    // printf("\tfamine->ehdr->e_shstrndx:\t\t\t%u\n", famine->ehdr->e_shstrndx);   /* Section header string table index */
    // printf("Offsets:\n");
    // printf("\tfamine->text_p_start_offset:\t\t\t%lu\n", famine->text_p_start_offset);
    // printf("\tfamine->text_p_end_offset:\t\t\t%lu\n", famine->text_p_end_offset);
    // printf("\tfamine->encrypt_s_start_offset:\t\t\t%lu\n", famine->encrypt_s_start_offset);
    // printf("\tfamine->encrypt_s_end_offset:\t\t\t%lu\n", famine->encrypt_s_end_offset);
    // printf("Adresses:\n");
    // printf("\tfamine->old_entry_point:\t\t\t\t0x%lx -> %lu\n", famine->old_entry_point, famine->old_entry_point);
    // printf("\tfamine->new_entry_point:\t\t\t\t0x%lx -> %lu\n", famine->new_entry_point, famine->new_entry_point);
    // printf("\tfamine->text_p_vaddr:\t\t\t\t0x%lx -> %lu\n", famine->text_p_vaddr, famine->text_p_vaddr);
    // printf("\tfamine->encrypt_s_addr:\t\t\t\t0x%lx -> %lu\n", famine->encrypt_s_addr, famine->encrypt_s_addr);
    // printf("\n");
}

void print_memory_hex(void *memory_ptr, size_t memory_size)
{
    printf("printing memory at address : %p :\n\n", memory_ptr);
    for (size_t i = 0; i < memory_size; i++)
    {
        printf("%x ", ((char *)(memory_ptr))[i]);
    }
    printf("\n");
}

void print_memory_char(void *memory_ptr, size_t memory_size)
{
    for (size_t i = 0; i < memory_size; i++)
    {
        printf("%c", ((char *)(memory_ptr))[i]);
    }
    printf("\n");
}