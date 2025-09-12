/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikotina <nikotina@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 11:55:38 by nacuna-g          #+#    #+#             */
/*   Updated: 2025/09/12 10:29:35 by nikotina         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

#define PROMPT "$>"
#define MAX_PROMPT_SIZE	100

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

# define BPURPLE	"\001\033[1;35m\002"
# define RESET		"\001\033[0m\002"

typedef enum {
    TOKEN_WORD,      // Comandos o argumentos (e.g., "ls", "-l", "txt")
    TOKEN_PIPE,      // |
    TOKEN_REDIRECT_IN,  // <
    TOKEN_REDIRECT_OUT, // >
    TOKEN_APPEND,    // >>
    TOKEN_AND,       // &&
    TOKEN_OR,        // ||
    TOKEN_QUOTE,     // Cadenas entre comillas
    TOKEN_VARIABLE,  // $VAR
    TOKEN_EOF        // Fin de la entrada
} t_token_type;

typedef struct s_token {
    char *value;          // El contenido del token (e.g., "ls", "|")
    t_token_type type;    // El tipo de token
    struct s_token *next; // Puntero al siguiente token (lista enlazada)
} t_token;

// PROMPT FUNCTIONS
int ft_prompt(void);

// TOKENIZER FUNCTIONS
t_token *tokenize(char *line);
void add_token(t_token **list, t_token *new_token);
t_token *create_token(char *value, t_token_type type);
