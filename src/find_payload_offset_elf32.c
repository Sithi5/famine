/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_payload_offset_elf32.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 16:13:46 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/26 16:13:46 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

// Find the gettextsize offset in the payload. return true if gettextsize have been found.
size_t find_gettextsize_offset_elf32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x33)
        {
            if (woody->payload_size - i > 4)
            {
                if (((char *)woody->payload_data)[i + 1] == 0x33 &&
                    ((char *)woody->payload_data)[i + 2] == 0x33 &&
                    ((char *)woody->payload_data)[i + 3] == 0x33)
                {
                    // Removing 1 to go to actual start of gettextsize (go back to instructions mov).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_GETTEXTSIZE_NOT_FOUND, woody);
    return -1;
}

// Find the getencryptedsectionsize offset in the payload. return true if getencryptedsectionsize have been found.
size_t find_getencryptedsectionsize_offset_elf32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x55)
        {
            if (woody->payload_size - i > 4)
            {
                if (((char *)woody->payload_data)[i + 1] == 0x55 &&
                    ((char *)woody->payload_data)[i + 2] == 0x55 &&
                    ((char *)woody->payload_data)[i + 3] == 0x55)
                {
                    // Removing 1 to go to actual start of getencryptedsectionsize (go back to instructions mov).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_GETENCRYPTEDSECTIONSIZE_NOT_FOUND, woody);
    return -1;
}

// Find the gettextsectionadd offset in the payload. return true if gettextsectionadd have been found.
size_t find_gettextsectionaddr_offset_elf32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x22)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in gettextsectionadd
                if (((char *)woody->payload_data)[i + 1] == 0x22 && ((char *)woody->payload_data)[i + 2] == 0x22 &&
                    ((char *)woody->payload_data)[i + 3] == 0x22 && ((char *)woody->payload_data)[i + 4] == 0x2d &&
                    ((char *)woody->payload_data)[i + 5] == 0x22 && ((char *)woody->payload_data)[i + 6] == 0x22 &&
                    ((char *)woody->payload_data)[i + 7] == 0x22 && ((char *)woody->payload_data)[i + 8] == 0x22 &&
                    ((char *)woody->payload_data)[i + 9] == 0x05 &&
                    ((char *)woody->payload_data)[i + 10] == 0x22 && ((char *)woody->payload_data)[i + 11] == 0x22 &&
                    ((char *)woody->payload_data)[i + 12] == 0x22 && ((char *)woody->payload_data)[i + 13] == 0x22)
                {
                    // Removing 1 to go to actual start of gettextsectionadd (go back to instructions sub).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_GETTEXTSECTIONADDR_NOT_FOUND, woody);
    return -1;
}

// Find the getencryptedsectionaddr offset in the payload. return true if getencryptedsectionaddr have been found.
size_t find_getencryptedsectionaddr_offset_elf32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x66)
        {
            if (woody->payload_size - i > 14)
            {
                // Actually checking we are in getencryptedsectionaddr
                if (((char *)woody->payload_data)[i + 1] == 0x66 && ((char *)woody->payload_data)[i + 2] == 0x66 &&
                    ((char *)woody->payload_data)[i + 3] == 0x66 && ((char *)woody->payload_data)[i + 4] == 0x2d &&
                    ((char *)woody->payload_data)[i + 5] == 0x66 && ((char *)woody->payload_data)[i + 6] == 0x66 &&
                    ((char *)woody->payload_data)[i + 7] == 0x66 && ((char *)woody->payload_data)[i + 8] == 0x66 &&
                    ((char *)woody->payload_data)[i + 9] == 0x05 &&
                    ((char *)woody->payload_data)[i + 10] == 0x66 && ((char *)woody->payload_data)[i + 11] == 0x66 &&
                    ((char *)woody->payload_data)[i + 12] == 0x66 && ((char *)woody->payload_data)[i + 13] == 0x66)
                {
                    // Removing 1 to go to actual start of getencryptedsectionaddr (go back to instructions sub).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_GETENCRYPTEDSECTIONADDR_NOT_FOUND, woody);
    return -1;
}

// Find the ret2oep offset in the payload. return true if ret2oep have been found.
size_t find_ret2oep_offset_elf32(t_woody *woody)
{
    for (size_t i = 0; i < woody->payload_size; i++)
    {
        if (((char *)woody->payload_data)[i] == 0x77)
        {
            if (woody->payload_size - i > 17)
            {
                // Actually checking we are in ret2oep
                if (((char *)woody->payload_data)[i + 1] == 0x77 && ((char *)woody->payload_data)[i + 2] == 0x77 &&
                    ((char *)woody->payload_data)[i + 3] == 0x77 &&
                    ((char *)woody->payload_data)[i + 4] == 0x2d &&
                    ((char *)woody->payload_data)[i + 5] == 0x77 && ((char *)woody->payload_data)[i + 6] == 0x77 &&
                    ((char *)woody->payload_data)[i + 7] == 0x77 && ((char *)woody->payload_data)[i + 8] == 0x77 &&
                    ((char *)woody->payload_data)[i + 9] == 0x05 &&
                    ((char *)woody->payload_data)[i + 10] == 0x77 && ((char *)woody->payload_data)[i + 11] == 0x77 &&
                    ((char *)woody->payload_data)[i + 12] == 0x77 && ((char *)woody->payload_data)[i + 13] == 0x77)
                {
                    // Removing 2 to go to actual start of ret2oep (go back to instructions sub).
                    return i - 1;
                }
            }
        }
    }
    error(ERROR_RET2OEP_NOT_FOUND, woody);
    return -1;
}
