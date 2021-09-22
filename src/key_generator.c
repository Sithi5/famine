/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_generator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/22 18:03:45 by mabouce           #+#    #+#             */
/*   Updated: 2021/09/22 18:03:45 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void random_string(t_famine *famine)
{
    char key[KEY_LEN];
    int fd;

    for (size_t n = 0; n < KEY_LEN; n++)
    {
        key[n] = 0;
    }
    if ((fd = open("/dev/random", O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, famine);
    }
    if (read(fd, key, KEY_LEN) == -1)
    {
        close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_READ, famine);
    }
    // Convert random char to readable
    for (int index = 0; index < KEY_LEN; index++)
    {
        key[index] < 0 ? key[index] *= -1 : 0;
        key[index] = (key[index] % 125);
        key[index] < 49 ? key[index] += 49 : 0;
    }
    close(fd) == -1 ? error(ERROR_CLOSE, famine) : 0;
    memcpy((void *)famine->encryption_key, key, KEY_LEN);
    famine->encryption_key[KEY_LEN] = '\0';
}

void key_generator(t_famine *famine)
{
    if (!(famine->encryption_key = (char *)malloc(sizeof(char) * KEY_LEN + 1)))
        error(ERROR_MALLOC, famine);
    random_string(famine);
}