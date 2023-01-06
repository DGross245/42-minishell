/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgross <dgross@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 16:05:58 by dgross            #+#    #+#             */
/*   Updated: 2023/01/06 15:13:03 by dgross           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // malloc
#include <readline/readline.h> // readline
#include <readline/history.h> // readline
#include <fcntl.h>

static void	increas_shlvl(t_koopa *shell)
{
	char	*tmp;
	char	*new_lvl;
	int		i;

	tmp = ft_strdup(ft_getenv(shell, "SHLVL=") + 6);
	i = ft_atoi(tmp);
	free(tmp);
	i++;
	new_lvl = ft_strjoin("SHLVL=", ft_itoa(i));
	ft_export(shell, to_double(new_lvl));
	free(new_lvl);
}

int	init_envp(t_koopa *shell, char **envp)
{
	int	i;

	i = -1;
	shell->envp = ft_calloc(ft_ptrcnt(envp) + 1, sizeof(char *));
	if (shell->envp == NULL)
		return (-1);
	while (envp[++i] != NULL)
		shell->envp[i] = ft_strdup(envp[i]);
	shell->envp[i] = NULL;
	increas_shlvl(shell);
	return (0);
}

static int	execute_minishell(t_koopa *shell)
{
	t_data				*tabel;
	char				**token_arr;
	char				*cmd;

	tabel = NULL;
	while (TRUE)
	{
		signal(SIGINT, ft_signal_handler);
		cmd = readline("👉 ");
		if (cmd == NULL)
			break ;
		add_history(cmd);
		token_arr = tokenizer(cmd);
		tabel = parser(token_arr);
		// list_test(tabel);
		signal(SIGINT, SIG_IGN);
		ft_execute(shell, tabel);
		free_data(tabel);
	}
	free_shell(shell);
	rl_clear_history();
	return (0);
}

static t_koopa	*init_shell(void)
{
	t_koopa	*shell;

	shell = ft_calloc(1, sizeof(t_koopa));
	g_exit_status = 0;
	shell->envp = NULL;
	shell->path = NULL;
	shell->line = NULL;
	shell->file = NULL;
	shell->tmp_stdin = -1;
	shell->tmp_stdout = -1;
	shell->in = -1;
	shell->out = -1;
	shell->skip = 0;
	shell->redirect = 1;
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_koopa				*shell;

	(void)argv;
	argc++;
	shell = NULL;
	signal(SIGQUIT, SIG_IGN);
	shell = init_shell();
	init_envp(shell, envp);
	ft_terminal(1);
	execute_minishell(shell);
	ft_terminal(0);
	return (0);
}
