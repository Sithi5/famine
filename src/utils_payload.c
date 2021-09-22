/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_payload.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 15:42:33 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/09 15:42:33 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody_woodpacker.h"

void load_payload(t_woody *woody, char *payload_name)
{
    double payload_size;
    int fd;

    if ((fd = open(payload_name, O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, woody);
    }
    if ((payload_size = lseek(fd, 0, SEEK_END)) != -1)
    {
        woody->payload_size = (size_t)payload_size;
        /* Go back to the start of the file. */
        if (lseek(fd, 0, SEEK_SET) != 0)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
        }
        if (!(woody->payload_data = malloc(payload_size)))
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_MALLOC, woody);
        }
        if (read(fd, woody->payload_data, woody->payload_size) == -1)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_READ, woody);
        }
    }
    else
    {
        close(fd) == -1 ? error(ERROR_CLOSE, woody) : error(ERROR_LSEEK, woody);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, woody) : 0;
}