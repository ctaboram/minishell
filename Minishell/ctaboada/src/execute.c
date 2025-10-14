/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 10:37:38 by ctaboada          #+#    #+#             */
/*   Updated: 2025/10/14 11:24:46 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->redir_in)
	{
		fd = open(cmd->redir_in, O_RDONLY);
		if (fd == -1)
		{
			perror(cmd->redir_in);
			exit(1);
		}
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
	if (cmd->redir_out)
	{
		if (cmd->is_append)
			fd = open(cmd->redir_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->redir_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
		{
			perror(cmd->redir_out);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

static void	exec_pipeline(t_data *data, t_cmd *cmd)
{
	int		fd[2];
	int		in_fd = 0;
	pid_t	pid;

	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			perror("pipe");
		pid = fork();
		if (pid == 0)
		{
			if (in_fd != 0)
				dup2(in_fd, STDIN_FILENO);
			if (cmd->next)
				dup2(fd[1], STDOUT_FILENO);
			if (cmd->redir_in || cmd->redir_out)
				handle_redirections(cmd);
			if (cmd->next)
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (is_builtin(cmd->av[0]))
				exec_builtin(data, cmd);
			else
			{
				char *path = find_command_path(cmd->av[0], data->env);
				if (!path)
					exit(127);
				execve(path, cmd->av, data->env);
				perror(cmd->av[0]);
				exit(1);
			}
		}
		if (in_fd != 0)
			close(in_fd);
		if (cmd->next)
		{
			close(fd[1]);
			in_fd = fd[0];
		}
		cmd = cmd->next;
	}
	while (wait(NULL) > 0)
		;
}

static void	exec_single(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->av[0]))
	{
		exec_builtin(data, cmd);
		return ;
	}
	pid = fork();
	if (pid == 0)
	{
		handle_redirections(cmd);
		char *path = find_command_path(cmd->av[0], data->env);
		if (!path)
			exit(127);
		execve(path, cmd->av, data->env);
		perror(cmd->av[0]);
		exit(1);
	}
	waitpid(pid, &status, 0);
	data->exit_status = WEXITSTATUS(status);
}

int	execute(t_data *data, t_cmd *cmd)
{
	if (!cmd)
		return (0);
	if (!cmd->next)
		exec_single(data, cmd);
	else
		exec_pipeline(data, cmd);
	return (0);
}
