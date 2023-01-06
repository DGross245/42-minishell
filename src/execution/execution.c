/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgross <dgross@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/23 10:13:09 by dgross            #+#    #+#             */
/*   Updated: 2023/01/06 20:16:36 by dgross           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"

#include <unistd.h> // dup2 access fork close
#include <stdlib.h> // exit
#include <sys/stat.h> // fstat
#include <stdio.h>
static void	open_pipe(t_koopa *shell)
{
	if (pipe(shell->fd) == -1)
	{
		print_error("Pipe", NULL, "Not enough space/cannot \
		allocate memory");
		return ;
	}
}

static void	close_pipe(t_koopa *shell)
{
	dup2(shell->fd[0], shell->in);
	close(shell->fd[0]);
	close(shell->fd[1]);
}

void	write_to(t_koopa *shell, t_data *tabel)
{
	struct stat	file_stat;

	dup2(shell->in, STDIN_FILENO);
	close(shell->in);
	if (tabel->next != NULL)
	{
		if (tabel->operator == PIPE)
			dup2(shell->fd[1], STDOUT_FILENO);
		else if (fstat(shell->out, &file_stat) == 0)
		{
			if (shell->skip == 1)
				return ;
			dup2(shell->out, STDOUT_FILENO);
			close(shell->out);
		}
	}
	else if (tabel->operator != OUT)
	{
		dup2(shell->out, STDOUT_FILENO);
		close(shell->out);
	}
}

static	void	close_fd(t_koopa *shell)
{
	close(shell->out);
	close(shell->in);
	dup2(shell->tmp_stdin, STDIN_FILENO);
	close(shell->tmp_stdin);
	dup2(shell->tmp_stdout, STDOUT_FILENO);
	close(shell->tmp_stdout);
}

int	ft_execute(t_koopa *shell, t_data *tabel)
{
	shell->head = tabel;
	if (check_for_heredoc(shell, tabel) == ERROR)
		return (ERROR);
	while (tabel != NULL)
	{
		if (ft_redirection(shell, tabel) == ERROR)
			break ;
		if (tabel->operator == PIPE)
		{
			open_pipe(shell);
			pipe_cmd(shell, tabel);
			close_pipe(shell);
		}
		else if (tabel->operator == CMD)
		{
			write_to(shell, tabel);
			if (ft_execute_builtin(shell, tabel) == 1)
				ft_execute_cmd(shell, tabel);
		}
		tabel = tabel->next;
	}
	close_fd(shell);
	get_exit_status(shell);
	return (0);
}
