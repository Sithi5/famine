/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infection.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/29 14:29:35 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/29 14:29:35 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void silvio_text_infection(t_woody *woody)
{
    // Create the output file
    if (!(woody->infected_file = malloc(woody->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, woody);
    }
    woody->infected_file_size = woody->binary_data_size + PAGE_SIZE;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {

            woody->payload_vaddr = woody->text_p_vaddr + woody->phdr[i].p_filesz;
            woody->ehdr->e_entry = woody->payload_vaddr;
            woody->new_entry_point = woody->payload_vaddr;

            woody->phdr[i].p_filesz += woody->payload_size;
            woody->phdr[i].p_memsz += woody->payload_size;

            for (int j = i + 1; j < woody->ehdr->e_phnum; j++)
                woody->phdr[j].p_offset += PAGE_SIZE;

            break;
        }
    }

    if (woody->text_p_end_offset % PAGE_SIZE + woody->payload_size > PAGE_SIZE)
    {
        error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, woody);
    }

    // Adding offset of one page in all section located after text section end. And get text section offset for the encryption.
    for (size_t i = 0; i < woody->ehdr->e_shnum; i++)
    {
        if (woody->shdr[i].sh_offset > woody->text_p_end_offset)
        {
            woody->shdr[i].sh_offset += PAGE_SIZE;
        }
        else if (woody->shdr[i].sh_addr + woody->shdr[i].sh_size == woody->payload_vaddr)
        {
            woody->shdr[i].sh_size += woody->payload_size;
        }
        // get section to encrypt info.
        if (!ft_strncmp(SECTION_TO_ENCRYPT_NAME,
                        (woody->string_table_ptr + woody->shdr[i].sh_name),
                        ft_strlen(SECTION_TO_ENCRYPT_NAME)))
        {
            woody->encrypt_s_start_offset = woody->shdr[i].sh_offset;
            woody->encrypt_s_size = woody->shdr[i].sh_size;
            woody->encrypt_s_end_offset = woody->encrypt_s_start_offset + woody->encrypt_s_size;
            woody->encrypt_s_addr = woody->shdr[i].sh_addr;
            woody->shdr[i].sh_flags |= SHF_WRITE;
        }
    }

    // Increase section header offset by PAGE_SIZE
    woody->ehdr->e_shoff += PAGE_SIZE;

    cipher_woody_file_data(woody);
    if (ARCH_32)
    {
        overwrite_payload_ret2oep(woody);
    }
    else if (ARCH_64)
    {
        overwrite_keysection_payload(woody);
        overwrite_payload_getencryptedsectionaddr(woody);
        overwrite_payload_ret2oep(woody);
        overwrite_payload_getencryptedsectionsize(woody);
        overwrite_payload_gettextsectionaddr(woody);
        overwrite_payload_gettextsize(woody);
    }

    // Copy until text section end
    ft_memcpy(woody->infected_file, woody->mmap_ptr, woody->text_p_end_offset);
    // Rewrite text section with cipher data.
    ft_memcpy(woody->infected_file + woody->encrypt_s_start_offset, woody->cipher, woody->encrypt_s_size);
    // Initialize value to zero for padding.
    ft_bzero(woody->infected_file + woody->text_p_end_offset, PAGE_SIZE);
    // Insert payload after text section end
    ft_memcpy(woody->infected_file + woody->text_p_end_offset, woody->payload_data, woody->payload_size);
    // Insert rest of binary
    ft_memcpy(woody->infected_file + woody->text_p_end_offset + PAGE_SIZE, woody->mmap_ptr + woody->text_p_end_offset, woody->binary_data_size - woody->text_p_end_offset);
}