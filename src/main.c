/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/26 12:12:08 by mabouce           #+#    #+#             */
/*   Updated: 2021/07/27 15:36:48 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void get_binary_data(char *file_name, t_famine *famine)
{
    double binary_data_size;
    int fd;
    struct stat FileAttrib;

    // Checking file is a regular file.
    if (stat(file_name, &FileAttrib) < 0)
    {
        error(ERROR_STAT, famine);
    }
    else if ((FileAttrib.st_mode & S_IFMT) != S_IFREG)
    {
        error(ERROR_NOT_A_REGULAR_FILE, famine);
    }

    if ((fd = open(file_name, O_RDONLY)) == -1)
    {
        error(ERROR_OPEN, famine);
    }

    if ((binary_data_size = lseek(fd, 0, SEEK_END)) != -1)
    {
        famine->binary_data_size = (size_t)binary_data_size;
        // Go back to the start of the file.
        if (lseek(fd, 0, SEEK_SET) != 0)
        {
            close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine);
        }
        // Copy binary address map
        if (!(famine->mmap_ptr = mmap(0, famine->binary_data_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)))
        {
            close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_MMAP, famine);
        }
    }
    else
    {
        close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine);
    }
    close(fd) == -1 ? error(ERROR_CLOSE, famine) : 0;
}

int main(int ac, char **av)
{
    t_famine *famine;

    if (!(famine = (t_famine *)malloc(sizeof(t_famine))))
        error(ERROR_MALLOC, famine);
    set_famine_ptrs_to_null(famine);
    if (ac != 2)
    {
        error(ERROR_INPUT_ARGUMENTS_NUMBERS, famine);
    }

    get_binary_data(av[1], famine);
    check_elf_header(famine);

    famine->ehdr = (t_elf_ehdr *)famine->mmap_ptr;
    famine->old_entry_point = famine->ehdr->e_entry;
    famine->phdr = (t_elf_phdr *)((famine->mmap_ptr + famine->ehdr->e_phoff));
    famine->shdr = (t_elf_shdr *)((famine->mmap_ptr + famine->ehdr->e_shoff));

    choose_infection_method(famine);
    free_famine(famine);
    return 0;
}
