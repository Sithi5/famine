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

#include "famine.h"

void load_payload(t_famine *famine, char *payload_name)
{
    double payload_size;
    int fd;

    if ((fd = open(payload_name, O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, famine);
    }
    if ((payload_size = lseek(fd, 0, SEEK_END)) != -1)
    {
        famine->payload_size = (size_t)payload_size;
        /* Go back to the start of the file. */
        if (lseek(fd, 0, SEEK_SET) != 0)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine);
        }
        if (!(famine->payload_data = malloc(payload_size)))
        {
            close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_MALLOC, famine);
        }
        if (read(fd, famine->payload_data, famine->payload_size) == -1)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_READ, famine);
        }
    }
    else
    {
        close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, famine) : 0;
}