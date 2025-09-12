#include "../includes/minishell.h"

int ft_builtin_pwd(void)
{
	char cwd[512];

	if(getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		return(0);
	}
	else
	{
		perror("pwd");
		return(1);
	}
}