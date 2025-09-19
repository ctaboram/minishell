/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:39:52 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/19 11:36:36 by nacuna-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../includes/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <string.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>

# define MAX_PROMPT_SIZE 1024

# define HEADER "\n\033[1;36m\
███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n\
████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n\
██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n\
██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n\
██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n\
╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n\
\n\033[0m\
                A simple shell implemented in C                    \n\
                             \033[1;34mBy\033[0m                   \n\
\t\t--- \033[1;36mctaboada\033[0m && \033[1;36mnacuna-g\033[0m --- \n\
\n"

# define BPURPLE	"\001\033[1;35m\002"
# define RESET		"\001\033[0m\002"

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_PAREN_OPEN,
	TOKEN_PAREN_CLOSE,
	TOKEN_QUOTE_SINGLE,
	TOKEN_QUOTE_DOUBLE,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	char			*file;
	t_token_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redir_in;
	t_redir			*redir_out;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_child
{
	t_cmd			*current;
	int				prev_fd;
	int				*pipes;
	int				i;
	int				num_cmds;
	char			***envp;
}	t_child;

typedef struct s_exec
{
	t_cmd			*cmds;
	int				*pipes;
	pid_t			*pids;
	char			***envp;
	int				num_cmds;
}	t_exec;

extern volatile sig_atomic_t	g_signal;

// PROMPT FUNCTIONS
int		ft_prompt(char ***envp);

// TOKENIZER FUNCTIONS
t_token	*tokenize(char *line);
void	free_tokens(t_token *tokens);

// PARSER FUNCTIONS
t_cmd	*parse(t_token *tokens);
void	free_cmd(t_cmd *cmds);

// EXPAND FUNCTIONS
void	expand_cmd(t_cmd *cmds, char **envp);
char	*expand_str(char *str, char **envp);

// EXECUTE FUNCTIONS
void	execute(t_cmd *cmds, char ***envp);
void	handle_signals(void);
char	*find_path(char *cmd, char **envp);

// BUILTINS FUNCTIONS
int		is_builtin(char *cmd);
int		exec_builtin(char **args, char ***envp);

// MAIN FUNCTIONS
char	**ft_copy_env(char **environ);
int		*exit_status(void);
void	ft_free_array(char **array);

#endif