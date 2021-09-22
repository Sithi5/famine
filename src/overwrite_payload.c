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

#include "woody_woodpacker.h"

// Rewrite info in payload ret2oep.
void overwrite_payload_ret2oep(t_woody *woody)
{
    size_t ret2oep_offset;
    if (ARCH_32)
    {
        ret2oep_offset = find_ret2oep_offset_elf32(woody);
        // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
        ft_memcpy(woody->payload_data + ret2oep_offset + 1, (void *)(&(ret2oep_offset)), 4);
        // Rewrite new entry_point in payload ret2oep.
        ft_memcpy(woody->payload_data + ret2oep_offset + 6, (void *)&(woody->new_entry_point), 4);
        // Rewrite old entry_point in payload ret2oep.
        ft_memcpy(woody->payload_data + ret2oep_offset + 11, (void *)&(woody->old_entry_point), 4);
    }
    else if (ARCH_64)
    {
        ret2oep_offset = find_ret2oep_offset_elf64(woody);
        // Rewrite payload size without ret2oep. + 2 to skip two first instructions and go to address.
        ft_memcpy(woody->payload_data + ret2oep_offset + 2, (void *)(&(ret2oep_offset)), 4);
        // Rewrite new entry_point in payload ret2oep.
        ft_memcpy(woody->payload_data + ret2oep_offset + 8, (void *)&(woody->new_entry_point), 4);
        // Rewrite old entry_point in payload ret2oep.
        ft_memcpy(woody->payload_data + ret2oep_offset + 14, (void *)&(woody->old_entry_point), 4);
    }
}

// Rewrite info in payload getencryptedsectionaddr.
void overwrite_payload_getencryptedsectionaddr(t_woody *woody)
{
    size_t getencryptedsectionaddr_offset;
    if (ARCH_32)
    {
        getencryptedsectionaddr_offset = find_getencryptedsectionaddr_offset_elf32(woody);
        // Rewrite payload size without getencryptedsectionaddr. + 2 to skip two first instructions and go to address.
        ft_memcpy(woody->payload_data + getencryptedsectionaddr_offset + 1, (void *)(&(getencryptedsectionaddr_offset)), 4);
        // Rewrite new entry_point in payload getencryptedsectionaddr.
        ft_memcpy(woody->payload_data + getencryptedsectionaddr_offset + 6, (void *)&(woody->new_entry_point), 4);
        // Rewrite old entry_point in payload getencryptedsectionaddr.
        ft_memcpy(woody->payload_data + getencryptedsectionaddr_offset + 11, (void *)&(woody->encrypt_s_addr), 4);
    }
    else if (ARCH_64)
    {
        getencryptedsectionaddr_offset = find_getencryptedsectionaddr_offset_elf64(woody);
        // Rewrite payload size without getencryptedsectionaddr. + 2 to skip two first instructions and go to address.
        ft_memcpy(woody->payload_data + getencryptedsectionaddr_offset + 2, (void *)(&(getencryptedsectionaddr_offset)), 4);
        // Rewrite new entry_point in payload getencryptedsectionaddr.
        ft_memcpy(woody->payload_data + getencryptedsectionaddr_offset + 8, (void *)&(woody->new_entry_point), 4);
        // Rewrite old entry_point in payload getencryptedsectionaddr.
        ft_memcpy(woody->payload_data + getencryptedsectionaddr_offset + 14, (void *)&(woody->encrypt_s_addr), 4);
    }
}

// Rewrite info in payload gettextsectionaddr.
void overwrite_payload_gettextsectionaddr(t_woody *woody)
{
    size_t gettextsectionaddr_offset;
    if (ARCH_32)
    {
        gettextsectionaddr_offset = find_gettextsectionaddr_offset_elf32(woody);
        // Rewrite payload size without gettextsectionaddr. + 2 to skip two first instructions and go to address.
        ft_memcpy(woody->payload_data + gettextsectionaddr_offset + 1, (void *)(&(gettextsectionaddr_offset)), 4);
        // Rewrite new entry_point in payload gettextsectionaddr.
        ft_memcpy(woody->payload_data + gettextsectionaddr_offset + 6, (void *)&(woody->new_entry_point), 4);
        // Rewrite woody->text_p_vaddr.
        ft_memcpy(woody->payload_data + gettextsectionaddr_offset + 11, (void *)&(woody->text_p_vaddr), 4);
    }
    else if (ARCH_64)
    {
        gettextsectionaddr_offset = find_gettextsectionaddr_offset_elf64(woody);
        // Rewrite payload size without gettextsectionaddr. + 2 to skip two first instructions and go to address.
        ft_memcpy(woody->payload_data + gettextsectionaddr_offset + 2, (void *)(&(gettextsectionaddr_offset)), 4);
        // Rewrite new entry_point in payload gettextsectionaddr.
        ft_memcpy(woody->payload_data + gettextsectionaddr_offset + 8, (void *)&(woody->new_entry_point), 4);
        // Rewrite woody->text_p_vaddr.
        ft_memcpy(woody->payload_data + gettextsectionaddr_offset + 14, (void *)&(woody->text_p_vaddr), 4);
    }
}

// Rewrite info in payload getencryptedsectionsize.
void overwrite_payload_getencryptedsectionsize(t_woody *woody)
{
    size_t getencryptedsectionsize_offset;
    if (ARCH_32)
    {
        getencryptedsectionsize_offset = find_getencryptedsectionsize_offset_elf32(woody);
        // Rewrite getencryptedsectionsize_offset + 2 to skip two first instructions and go to encrypted_size value.
        ft_memcpy(woody->payload_data + getencryptedsectionsize_offset + 1, (void *)&(woody->encrypt_s_size), 4);
    }
    else if (ARCH_64)
    {
        getencryptedsectionsize_offset = find_getencryptedsectionsize_offset_elf64(woody);
        // Rewrite getencryptedsectionsize_offset + 2 to skip two first instructions and go to encrypted_size value.
        ft_memcpy(woody->payload_data + getencryptedsectionsize_offset + 2, (void *)&(woody->encrypt_s_size), 4);
    }
}

// Rewrite info in payload gettextsize.
void overwrite_payload_gettextsize(t_woody *woody)
{
    size_t gettextsize_offset;
    if (ARCH_32)
    {
        gettextsize_offset = find_gettextsize_offset_elf32(woody);
        // Rewrite gettextsize_offset + 2 to skip two first instructions and go to textsize value.
        ft_memcpy(woody->payload_data + gettextsize_offset + 2, (void *)&(woody->text_section_size), 4);
    }
    else if (ARCH_64)
    {
        gettextsize_offset = find_gettextsize_offset_elf64(woody);
        // Rewrite gettextsize_offset + 2 to skip two first instructions and go to textsize value.
        ft_memcpy(woody->payload_data + gettextsize_offset + 2, (void *)&(woody->text_section_size), 4);
    }
}

void overwrite_keysection_payload(t_woody *woody)
{
    size_t keysection_offset = find_keysection_offset_elf64(woody);
    ft_memcpy(woody->payload_data + keysection_offset, woody->encryption_key, KEY_LEN);
}
