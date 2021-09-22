/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypto.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/27 15:36:43 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:43 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void cipher_woody_file_data(t_woody *woody)
{
    if (!(woody->cipher = (void *)malloc(woody->encrypt_s_size + 8)))
        error(ERROR_MALLOC, woody);
    ft_memcpy(woody->cipher, woody->mmap_ptr + woody->encrypt_s_start_offset, woody->encrypt_s_size);

    key_generator(woody);
    if (ARCH_32)
    {
        // asm_xor_cipher(woody->cipher, woody->encrypt_s_size, "BBBB", 4);
    }
    else if (ARCH_64)
    {
        rc4_cipher_start(woody->cipher, woody->encrypt_s_size, woody->encryption_key, KEY_LEN);
    }
}