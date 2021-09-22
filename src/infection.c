/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_infection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:10:16 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/18 14:10:16 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void choose_infection_method(t_woody *woody)
{
    set_string_table_ptr(woody);
    load_payload(woody, PAYLOAD_NAME);

    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    woody->ehdr->e_ident[EI_PAD + 3] = 7;

    for (size_t i = 0; i < woody->ehdr->e_phnum; i++)
    {
        if (woody->phdr[i].p_type == PT_LOAD && woody->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            woody->text_p_start_offset = woody->phdr[i].p_offset;
            woody->text_p_end_offset = woody->phdr[i].p_offset + woody->phdr[i].p_filesz;
            woody->text_p_size = woody->phdr[i].p_filesz;
            woody->text_section_size = woody->phdr[i].p_filesz;
            woody->text_p_vaddr = woody->phdr[i].p_vaddr;

            // Check if there is enought space for our payload in the text section.
            if (woody->payload_size > PAGE_SIZE)
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, woody);
            }
            else if (woody->text_p_end_offset % PAGE_SIZE + woody->payload_size < PAGE_SIZE)
            {
                silvio_text_infection(woody);
            }
            else
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, woody);
            }
            break;
        }
    }
}