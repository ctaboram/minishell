/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:15:00 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/24 12:00:24 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds != NULL)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

static void	apply_redir_in(t_redir *redir)
{
	int		fd;
	int		pipefd[2];
	char	*line;

	while (redir != NULL)
	{
		if (redir->type == TOKEN_REDIRECT_IN)
		{
			fd = open(redir->file, O_RDONLY);
			if (fd < 0)
			{
				perror(redir->file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (redir->type == TOKEN_HEREDOC)
		{
			pipe(pipefd);
			line = readline("> ");
			while (line != NULL && ft_strcmp(line, redir->file) != 0)
			{
				write(pipefd[1], line, ft_strlen(line));
				write(pipefd[1], "\n", 1);
				free(line);
				line = readline("> ");
			}
			free(line);
			close(pipefd[1]);
			dup2(pipefd[0], STDIN_FILENO);
			close(pipefd[0]);
		}
		redir = redir->next;
	}
}

static void apply_redir_out(t_redir *redir)
{
	int fd;

	while (redir != NULL)
	{
		if (redir->type == TOKEN_REDIRECT_OUT)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == TOKEN_APPEND)
			fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else
			return ;
		if (fd < 0)
		{
			perror(redir->file);
			exit(1);
		}
		dup2(fd, STDOUT_FILENO);
		close(fd);
		redir = redir->next;
	}
}

static char	*get_path_from_env(char **envp)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			break ;
		}
		i++;
	}
	return (path);
}

static char *try_path(char **paths, char *cmd)
{
    char    *full;
    char    *slash;
    int     i;

    i = 0;
    while (paths[i] != NULL)
    {
        slash = ft_strjoin(paths[i], "/");
        if (slash == NULL)
        {
            i++;
            continue ;
        }
        full = ft_strjoin(slash, cmd);
        free(slash);  // Free intermediate
        if (full == NULL)
        {
            i++;
            continue ;
        }
        if (access(full, X_OK) == 0)
            return (full);
        free(full);
        i++;
    }
    return (NULL);
}

char	*find_path(char *cmd, char **envp)
{
	char	*path;
	char	**paths;
	char	*full;

	if (ft_strchr(cmd, '/') != NULL)
		return (ft_strdup(cmd));
	path = get_path_from_env(envp);
	if (path == NULL)
		return (NULL);
	paths = ft_split(path, ':');
	full = try_path(paths, cmd);
	ft_free_array(paths);
	return (full);
}

static void child_process(t_child *params)
{
	int		j;
	char	**new_envp;
	char	*path;

	j = 0;
	new_envp = NULL;
	while (j < params->num_cmds - 1)
	{
		if (j != params->i)
		{
			close(params->pipes[j * 2]);
			close(params->pipes[j * 2 + 1]);
		}
		j++;
	}
	if (params->prev_fd != STDIN_FILENO)
	{
		dup2(params->prev_fd, STDIN_FILENO);
		close(params->prev_fd);
	}
	if (params->current->next != NULL && params->i < params->num_cmds - 1)
	{
		close(params->pipes[params->i * 2]);
		dup2(params->pipes[params->i * 2 + 1], STDOUT_FILENO);
		close(params->pipes[params->i * 2 + 1]);
	}
	apply_redir_in(params->current->redir_in);
	apply_redir_out(params->current->redir_out);
	if (is_builtin(params->current->args[0]))
	{
		new_envp = exec_builtin(params->current->args, params->envp);
		exit(*exit_status());
	}
	path = find_path(params->current->args[0], params->envp);
	if (path == NULL)
	{
		printf("%s: command not found\n", params->current->args[0]);
		exit(127);
	}
	execve(path, params->current->args, params->envp);
	perror("execve");
	exit(126);
}

static void fork_commands(t_exec *exec)
{
    t_cmd *current;
    int prev_fd;
    int i;

    current = exec->cmds;
    prev_fd = STDIN_FILENO;
    i = 0;
    while (current != NULL)
    {
        exec->pids[i] = fork();
        if (exec->pids[i] == 0)
        {
            t_child params;

            params.current = current;
            params.prev_fd = prev_fd;
            params.pipes = exec->pipes;
            params.i = i;
            params.num_cmds = exec->num_cmds;
            params.envp = exec->envp;
            child_process(&params);
        }
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
        if (current->next != NULL)
        {
            close(exec->pipes[i * 2 + 1]);
            prev_fd = exec->pipes[i * 2];
        }
        current = current->next;
        i++;
    }
}

static void	wait_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			*exit_status() = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status() = 128 + WTERMSIG(status);
		i++;
	}
}

char	**execute(t_cmd *cmds, char **envp)
{
	int		num_cmds;
	int		*pipes;
	pid_t	*pids;
	t_exec	exec_params;

	num_cmds = count_cmds(cmds);
	if (num_cmds == 1 && is_builtin(cmds->args[0]) && cmds->redir_in == NULL && cmds->redir_out == NULL && cmds->next == NULL)
	{
		envp = exec_builtin(cmds->args, envp);
		return (envp);
	}
	pipes = malloc(sizeof(int) * (num_cmds - 1) * 2);
	pids = malloc(sizeof(pid_t) * num_cmds);
	exec_params.cmds = cmds;
	exec_params.pipes = pipes;
	exec_params.pids = pids;
	exec_params.envp = envp;
	exec_params.num_cmds = num_cmds;
	num_cmds = exec_params.num_cmds - 1;
	num_cmds = 0;
	while (num_cmds < exec_params.num_cmds - 1)
	{
		pipe(exec_params.pipes + num_cmds * 2);
		num_cmds++;
	}
	fork_commands(&exec_params);
	num_cmds = 0;
	while (num_cmds < exec_params.num_cmds - 1)
	{
		close(exec_params.pipes[num_cmds * 2]);
		num_cmds++;
	}
	wait_children(exec_params.pids, exec_params.num_cmds);
	free(exec_params.pipes);
	free(exec_params.pids);
	return (envp);
}
