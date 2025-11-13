/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 12:12:21 by nikotina          #+#    #+#             */
/*   Updated: 2025/10/15 10:14:10 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_executor_error	handle_redirections(t_cmd *cmd)
{
	int	fd;

	if (cmd->redir_in)
	{
		fd = open(cmd->redir_in, O_RDONLY);
		if (fd == -1)
			return (EXECUTOR_REDIR_FAIL);
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			close(fd);
			return (EXECUTOR_DUP_FAIL);
		}
		close(fd);
	}
	if (cmd->redir_out)
	{
		if (cmd->is_append)
			fd = open(cmd->redir_out, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			fd = open(cmd->redir_out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd == -1)
			return (EXECUTOR_REDIR_FAIL);
		if (dup2(fd, STDOUT_FILENO) == -1)
		{
			close(fd);
			return (EXECUTOR_DUP_FAIL);
		}
		close(fd);
	}
	return (EXECUTOR_OK);
}

static t_executor_error	exec_single(t_data *data, t_cmd *cmd)
{
	pid_t	pid;
	int		status;
	t_executor_error	err;

	if (cmd->av && cmd->av[0] && is_builtin(cmd->av[0]) && !cmd->redir_in && !cmd->redir_out)
	{
		exec_builtin(data, cmd);
		data->execute.exit_status = data->exit_status;
		return (EXECUTOR_OK);
	}
	if (!cmd->av || !cmd->av[0])
	{
		data->execute.exit_status = 127;
		return (EXECUTOR_CMD_NOT_FOUND);
	}
	setup_exec_signals();
	pid = fork();
	if (pid == -1)
	{
		setup_signals();
		return (EXECUTOR_FORK_FAIL);
	}
	if (pid == 0)
	{
		setup_child_signals();
		err = handle_redirections(cmd);
		if (err != EXECUTOR_OK)
		{
			if (cmd->redir_in)
				handler_error_child(CHILD_REDIR_FAIL, cmd->redir_in);
			else
				handler_error_child(CHILD_REDIR_FAIL, cmd->redir_out);
		}
		char *path = find_command_path(cmd->av[0], data->env);
		if (!path)
			handler_error_child(CHILD_CMD_NOT_FOUND, cmd->av[0]);
		execve(path, cmd->av, data->env);
		handler_error_child(CHILD_EXEC_FAIL, cmd->av[0]);
	}
	waitpid(pid, &status, WUNTRACED);
	setup_signals();
	if (WIFEXITED(status))
		data->execute.exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_signal_exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
	}
	else if (WIFSTOPPED(status))
	{
		g_signal_exit_code = 128 + WSTOPSIG(status);
		write(STDOUT_FILENO, "\n", 1);
		write(STDOUT_FILENO, "[1]+  Stopped\n", 15);
	}
	else
		data->execute.exit_status = 1;
	return (EXECUTOR_OK);
}

static t_executor_error	exec_pipeline(t_data *data, t_cmd *cmd)
{
	int					fd[2];
	int					in_fd = 0;
	pid_t				pid;
	int					wstatus;
	t_executor_error	err;

	setup_exec_signals();
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
		{
			setup_signals();
			return (EXECUTOR_PIPE_FAIL);
		}
		pid = fork();
		if (pid == -1)
		{
			setup_signals();
			return (EXECUTOR_FORK_FAIL);
		}
	if (pid == 0)
		{
			setup_child_signals();
			if (in_fd != 0 && dup2(in_fd, STDIN_FILENO) == -1)
				exit(EXECUTOR_DUP_FAIL);
			if (cmd->next && dup2(fd[1], STDOUT_FILENO) == -1)
				exit(EXECUTOR_DUP_FAIL);
			err = handle_redirections(cmd);
			if (err != EXECUTOR_OK)
				exit(err);
			if (cmd->next)
			{
				close(fd[0]);
				close(fd[1]);
			}
			if (!cmd->av || !cmd->av[0])
				exit(127);
			if (is_builtin(cmd->av[0]))
				exec_builtin(data, cmd);
			else
			{
				char *path = find_command_path(cmd->av[0], data->env);
				if (!path)
					exit(127);
				execve(path, cmd->av, data->env);
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
	pid_t	last_pid = pid;
	while ((pid = waitpid(-1, &wstatus, WUNTRACED)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(wstatus))
				data->execute.exit_status = WEXITSTATUS(wstatus);
			else if (WIFSIGNALED(wstatus))
			{
				g_signal_exit_code = 128 + WTERMSIG(wstatus);
				if (WTERMSIG(wstatus) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				else if (WTERMSIG(wstatus) == SIGQUIT)
					write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
			}
			else if (WIFSTOPPED(wstatus))
			{
				g_signal_exit_code = 128 + WSTOPSIG(wstatus);
				write(STDOUT_FILENO, "\n", 1);
				write(STDOUT_FILENO, "[1]+  Stopped\n", 15);
			}
		}
	}
	setup_signals();
	if (data->execute.exit_status == 0)
		data->execute.exit_status = 1;
	return (EXECUTOR_OK);
}

t_executor_error	execute(t_data *data)
{
	t_executor_error	status;

	status = EXECUTOR_OK;
	// Inicializar la estructura execute con los datos necesarios
	data->execute.cmds_list = data->parser.cmds_list;
	data->execute.env = data->env;
	data->execute.exit_status = data->exit_status;
	data->execute.in_fd = 0;
	
	if (!data->execute.cmds_list)
		return (EXECUTOR_OK);
	if (!data->execute.cmds_list->next)
		status = exec_single(data, data->execute.cmds_list);
	else
		status = exec_pipeline(data, data->execute.cmds_list);
	
	// Actualizar exit_status en data principal
	if (g_signal_exit_code != 0)
	{
		data->exit_status = g_signal_exit_code;
		g_signal_exit_code = 0;
	}
	else
		data->exit_status = data->execute.exit_status;
	return (status);
}
