/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 11:15:00 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/18 10:23:34 by nacuna-g         ###   ########.fr       */
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
            return ; // No debería llegar aquí
        
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

static char	*try_path(char **paths, char *cmd)
{
	char	*full;
	int		i;

	i = 0;
	while (paths[i] != NULL)
	{
		full = ft_strjoin(paths[i], "/");
		full = ft_strjoin_free(full, cmd);
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

static void child_process(t_cmd *current, int prev_fd, int *pipes, 
                         int i, int num_cmds, char ***envp)
{
    // Cerrar todos los pipes que no necesite este proceso
    int j = 0;
    while (j < num_cmds - 1)
    {
        if (j != i)
        {
            close(pipes[j * 2]);
            close(pipes[j * 2 + 1]);
        }
        j++;
    }
    
    // Configurar entrada
    if (prev_fd != STDIN_FILENO)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    
    // Configurar salida (excepto para el último comando)
    if (current->next != NULL && i < num_cmds - 1)
    {
        close(pipes[i * 2]); // Cerrar lectura
        dup2(pipes[i * 2 + 1], STDOUT_FILENO);
        close(pipes[i * 2 + 1]); // Cerrar escritura
    }
    
    // Aplicar redirecciones
    apply_redir_in(current->redir_in);
    apply_redir_out(current->redir_out);
    
    // Ejecutar comando
    if (is_builtin(current->args[0]))
    {
        g_exit_status = exec_builtin(current->args, envp);
        exit(g_exit_status);
    }
    
    char *path = find_path(current->args[0], *envp);
    if (path == NULL)
    {
        printf("%s: command not found\n", current->args[0]);
        exit(127);
    }
    
    execve(path, current->args, *envp);
    perror("execve");
    exit(126);
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
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
		i++;
	}
}

void	execute(t_cmd *cmds, char ***envp)
{
	int		num_cmds;
	int		*pipes;
	pid_t	*pids;
	t_cmd	*current;
	int		prev_fd;
	int		i;

	num_cmds = count_cmds(cmds);
	if (num_cmds == 1 && is_builtin(cmds->args[0]) && cmds->redir_in == NULL && cmds->redir_out == NULL && cmds->next == NULL)
	{
		g_exit_status = exec_builtin(cmds->args, envp);
		return ;
	}
	pipes = malloc(sizeof(int) * (num_cmds - 1) * 2);
	pids = malloc(sizeof(pid_t) * num_cmds);
	i = 0;
	while (i < num_cmds - 1)
	{
		pipe(pipes + i * 2);
		i++;
	}
	current = cmds;
	prev_fd = STDIN_FILENO;
	i = 0;
	while (current != NULL)
	{
		pids[i] = fork();
		if (pids[i] == 0)
			child_process(current, prev_fd, pipes, i, num_cmds, envp);
		if (prev_fd != STDIN_FILENO)
			close(prev_fd);
		if (current->next != NULL)
		{
			close(pipes[i * 2 + 1]);
			prev_fd = pipes[i * 2];
		}
		current = current->next;
		i++;
	}
	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipes[i * 2]);
		i++;
	}
	wait_children(pids, num_cmds);
	free(pipes);
	free(pids);
}
