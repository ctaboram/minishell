/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:39:52 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/13 13:01:51 by nikotina         ###   ########.fr       */
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

# define BPURPLE			"\001\033[1;35m\002"
# define RESET				"\001\033[0m\002"
# define MAX_PROMPT_SIZE	1042

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

typedef struct s_token
{
	char			*value; 
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_parser
{
	t_token	*current;	// token actual
	t_cmd	*head;		// cabeza de la lista de comandos
	t_cmd	*cmd;		// comando actual
}	t_parser;

typedef struct s_cmd
{
	char			**av;			// argumentos (argv[0] = comando)
	char			*redir_in;		// archivo de entrada
	char			*redir_out;		// archivo de salida
	int				is_append;		// 1 si >>
	struct s_cmd	*next;			// siguiente comando si hay pipe
}	t_cmd;

typedef struct s_tokenizer
{
	char	*start;
	char	*end;
}	t_tokenizer;

typedef struct s_data
{
	char		**env;
	char		*input;
	char		*input_expanded;
	t_token		*tokens;
	t_tokenizer	*tokenizer;
	t_cmd		*cmds_list;
	int			exit_status;
}	t_data;

typedef struct s_expand
{
	char	*result;
	char	*var;
	char	*value;
	int		i;
	int		start;
}	t_expand;

// PROMPT FUNCTIONS
t_prompt_error	init_prompt(t_data *data);

// TOKENIZER FUNCTIONS
t_tokenizer_error	tokenizer(t_data *data, t_tokenizer	*tokenizer);

// UTILS_TOKENIZER FUNCTIONS
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **tokens, t_token *token);

// EXPAND FUNCTION
t_expand_error	expand_word(t_data *data);

// UTILS_EXPAND FUNCTIONS
void	init_expand(t_expand *ex);

// PARSER FUNCTIONS
t_parser_error	parser_tokens(t_data *data);

// UTILS_PARSER FUNCTIONS
t_parser_error	append_arg(t_cmd *cmd, char *value);
t_cmd	*new_cmd(void);

// UTILS FUNCTIONS
void	init_parser(t_parser *parser, t_token *tokens);
void	init_data(t_data *data, char **env);
void	free_tokens(t_token *tokens);

// ERROR FUNCTIONS
void	ft_tokenizer_error(t_tokenizer_error err);
void	ft_expand_error(t_expand_error err);
void	ft_parser_error(t_parser_error err);

// FREE FUNCTIONS
void	free_array(char **array);

#endif
