/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:39:52 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/22 11:00:34 by nikotina         ###   ########.fr       */
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
# include <limits.h>

# define BPURPLE			"\001\033[1;35m\002"
# define YELLOW				"\001\033[0;33m\002"
# define RESET				"\001\033[0m\002"
# define MAX_PROMPT_SIZE	1042
# define PATH_MAX 4096

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

// ERRORS
typedef enum e_prompt
{
	ERROR_TOKENIZER,
	ERROR_EXPAND,
	ERROR_PARSER,
	ERROR_EXECUTOR,
	PROMPT_CONTINUE = 0,
	PROMPT_EOF,
	PROMPT_EXIT
}	t_prompt_error;

typedef enum e_tokenizer_error
{
	TOK_OK = 0,
	TOK_MEMORY_ALLOC,
	TOK_UNCLOSED_QUOTE,
	TOK_SYNTAX_PIPE,
	TOK_SYNTAX_REDIR
}	t_tokenizer_error;

typedef enum e_parser_error
{
	PARSER_OK = 0,
	PARSER_SYNTAX_REDIR,
	PARSER_SYNTAX_PIPE,
	PARSER_MEMORY_ALLOC,
}	t_parser_error;

typedef enum e_expand_error
{
	EXPAND_OK = 0,
	EXPAND_MEMORY_ALLOC
}	t_expand_error;

typedef enum e_executor_error
{
	EXECUTOR_OK = 0,
	EXECUTOR_FORK_FAIL,
	EXECUTOR_PIPE_FAIL,
	EXECUTOR_CMD_NOT_FOUND,
	EXECUTOR_REDIR_FAIL,
	EXECUTOR_MEMORY_ALLOC,
	EXECUTOR_DUP_FAIL
}	t_executor_error;

typedef enum e_child_error
{
	CHILD_OK = 0,
	CHILD_REDIR_FAIL = 1,
	CHILD_DUP_FAIL = 2,
	CHILD_CMD_NOT_FOUND = 127,
	CHILD_EXEC_FAIL = 126,
}	t_child_error;

// TOKEN TYPES
typedef enum e_token_type
{
	TOKEN_QUOTE_SINGLE,
	TOKEN_QUOTE_DOUBLE,
	TOKEN_PIPE,
	TOKEN_WORD,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

// GENERAL STRUCTURES
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**av;
	char			*redir_in;
	char			*redir_out;
	int				is_append;
	struct s_cmd	*next;
}	t_cmd;

// EXPAND STRUCTURE
typedef struct s_expand
{
	char	*result;
	char	*var;
	char	*value;
	int		i;
	int		start;
	char	*input;
	char	*input_expanded;
	char	**env;
	int		exit_status;
}	t_expand;

// TOKENIZER STRUCTURE
typedef struct s_tokenizer
{
	char	*start;
	char	*end;
	char	*input_to_tokenize;
	t_token	*tokens;
}	t_tokenizer;

// PARSER STRUCTURE
typedef struct s_parser
{
	t_token	*current;
	t_cmd	*head;
	t_cmd	*cmd;
	t_token	*tokens;
	t_cmd	*cmds_list;
}	t_parser;

// EXECUTE STRUCTURE
typedef struct s_execute
{
	t_cmd	*cmds_list;
	char	**env;
	int		exit_status;
	pid_t	last_pid;
	int		pipe_fds[2];
	int		in_fd;
}	t_execute;

// MAIN DATA STRUCTURE
typedef struct s_data
{
	char		**env;
	char		*input;
	int			exit_status;
	t_expand	expand;
	t_tokenizer	tokenizer;
	t_parser	parser;
	t_execute	execute;
}	t_data;

// PROMPT FUNCTIONS
t_prompt_error		init_prompt(t_data *data);

// EXPAND FUNCTION
t_expand_error		expand_word(t_data *data);

// UTILS_EXPAND FUNCTIONS
void				init_expand(t_data *data);
char				*ft_strjoin_free(char *s1, char *s2);

// TOKENIZER FUNCTIONS
t_tokenizer_error	tokenizer(t_data *data);

// UTILS_TOKENIZER FUNCTIONS
t_token				*create_token(char *value, t_token_type type);
void				add_token(t_token **tokens, t_token *token);

// PARSER FUNCTIONS
t_parser_error		parser_tokens(t_data *data);

// UTILS_PARSER FUNCTIONS
t_parser_error		append_arg(t_cmd *cmd, char *value);
t_cmd				*new_cmd(void);

// EXECUTOR FUNCTIONS
t_executor_error	execute(t_data *data);

// UTILS_EXECUTOR FUNCTIONS
char	*find_command_path(char *cmd, char **env);

// UTILS FUNCTIONS
void				init_parser(t_parser *parser, t_token *tokens);
void				init_data(t_data *data, char **env);
void				free_tokens(t_token *tokens);

// ERROR FUNCTIONS
void				ft_tokenizer_error(t_tokenizer_error err);
void				ft_expand_error(t_expand_error err);
void				ft_parser_error(t_parser_error err);
void				ft_executor_error(t_executor_error err);

// CHILD ERROR FUNCTIONS
void				handler_error_child(t_child_error code,
						const char *context);

// FREE FUNCTIONS
void				free_array(char **array);

// BUILTIN FUNCTIONS
int					is_builtin(char *cmd);
void				exec_builtin(t_data *data, t_cmd *cmd);
int					ft_builtin_cd(char **args, t_data *data);
int					ft_builtin_pwd(void);
int					ft_builtin_echo(char **args);
int					ft_builtin_env(char **env);
char				**ft_builtin_export(char **args, char **env);
char				**ft_builtin_unset(char **args, char **env);
char				**dup_env(char **env);
char				**add_or_update_env(char **env, char *arg);
int					is_valid_identifier(char *arg);
char				**add_new_env(char **env, char *new_var);
char				*find_command_path(char *cmd, char **env);
t_prompt_error		handler_error(t_data *data,
						t_prompt_error error_type);
void				ft_free_env(char **env);

void				ft_free_all(t_data *data);

// SIGNAL FUNCTIONS
void				setup_signals(void);
void				setup_child_signals(void);
void				setup_exec_signals(void);

#endif
