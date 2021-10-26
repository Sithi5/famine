/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_segments.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/23 18:55:32 by mabouce           #+#    #+#             */
/*   Updated: 2021/09/23 18:55:32 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

bool is_text_segment(const t_elf_phdr segment)
{
    if (segment.p_type != PT_LOAD)
        return false;

    return segment.p_flags == (PF_R | PF_X);
}

bool is_data_segment(const t_elf_phdr segment)
{
    if (segment.p_type != PT_LOAD)
        return false;

    return segment.p_flags == (PF_W | PF_R);
}

bool is_note_segment(const t_elf_phdr segment)
{
    return segment.p_type == PT_NOTE;
}
