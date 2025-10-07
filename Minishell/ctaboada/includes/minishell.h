/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctaboada <ctaboada@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:38 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/10/07 11:57:13 by ctaboada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include "../includes/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

#define PROMPT "$>"
#define MAX_PROMPT_SIZE	1024

# define BPURPLE	"\001\033[1;35m\002"
# define YELLOW     "\001\033[0;33m\002"
# define RESET		"\001\033[0m\002"

# define HEADER ("\n\033[1;36m\
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
\n")

typedef enum {
    TOKEN_WORD,         // Comandos o argumentos (e.g., "ls", "-l", "txt")
    TOKEN_PIPE,         // |
    TOKEN_REDIRECT_IN,  // <
    TOKEN_REDIRECT_OUT, // >
    TOKEN_APPEND,        // >>
    TOKEN_AND,           // &&
    TOKEN_OR,            // ||
    TOKEN_QUOTE,         // Cadenas entre comillas
    TOKEN_VARIABLE,      // $VAR
    TOKEN_EOF            // Fin de la entrada
} t_token_type;

typedef struct s_token {
    char *value;          // El contenido del token (e.g., "ls", "|")
    t_token_type type;    // El tipo de token
    struct s_token *next; // Puntero al siguiente token (lista enlazada)
} t_token;
typedef struct s_data
{
	char		**env;
	char		*input;
	t_token		*tokens;
	int			exit_status;
}	t_data;

extern char **environ;

// MAIN FUNCTIONS
int ft_prompt(t_data *data);

// BUILTINS
int ft_builtin_cd(char **args, t_data *data);
int ft_builting_echo(char **args);
int ft_builtin_pwd(void);
int ft_builtin_env(char **env);
char **ft_builtin_export(char **arg,char **env);
int is_valid_identifier(char *arg);
char **add_or_update_env(char **env, char *arg);
char	**ft_builtin_unset(char **args, char **env);    

char **add_new_env(char **env, char *new_var);

// TOKENIZATION
t_token *tokenize(char *line);
void add_token(t_token **list, t_token *new_token);
t_token *create_token(char *value, t_token_type type);

// UTILS
char ** dup_env(char **env);
void ft_free_env(char **env);

#endif