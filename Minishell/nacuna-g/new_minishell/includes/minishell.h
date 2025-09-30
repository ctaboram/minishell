/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nacuna-g <nacuna-g@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 12:39:52 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/30 11:23:31 by nacuna-g         ###   ########.fr       */
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

typedef enum e_prompt {
	PROMPT_CONTINUE = 0,
	PROMPT_EOF,
	PROMPT_EXIT
} t_prompt;

typedef enum e_error {
	ERR_OK = 0,
	ERR_MEMORY_ALLOC,
	ERR_UNCLOSED_QUOTE,
	ERR_SYNTAX_PIPE,
	ERR_SYNTAX_REDIR
} t_error;

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

typedef struct s_tokenizer
{
	char	*start;
	char	*end;
}	t_tokenizer;

typedef struct s_data
{
	char		**env;
	char		*input;
	t_token		*tokens;
	t_tokenizer	*tokenizer;
	int			exit_status;
}	t_data;

// PROMPT FUNCTIONS
int	init_prompt(t_data *data);

// UTILS FUNCTIONS
void	init_data(t_data *data, char **env);
void	free_tokens(t_token *tokens);

// TOKENIZER FUNCTIONS
int	tokenizer(t_data *data, t_tokenizer	*tokenizer);

// UTILS_TOKENIZER FUNCTION
t_token	*create_token(char *value, t_token_type type);
void	add_token(t_token **tokens, t_token *token);

// ERROR FUNCTIONS
void	ft_fatal_error(char *msg);
void	ft_print_error(t_data *data, int error, int *exit_status);

// FREE FUNCTIONS
void	ft_free_array(char **array);

#endif