/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgross <dgross@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 16:43:57 by dgross            #+#    #+#             */
/*   Updated: 2022/11/26 13:59:24 by dgross           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h> // printf
#include <limits.h> // PATH_MAX
#include <unistd.h> // getcwd

int ft_pwd(void)
{
	char buf[PATH_MAX];

	if (getcwd(buf, PATH_MAX))
		printf("%s\n", buf);
	else
		print_error();/*error message?*/
	return (0); // !! exit status !!
}