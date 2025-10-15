# 🐚 Minishell

Un proyecto de la escuela **42** hecho por:  
- 👨‍💻 [nacuna-g](https://github.com/nicoagnz)  
- 👨‍💻 [ctaboram](https://github.com/ctaboram)  

---

## 📖 Resumen del proyecto

El objetivo de **Minishell** es recrear un shell simple de UNIX, implementando un intérprete de comandos que soporte funcionalidades básicas como:  
- Ejecución de comandos simples  
- Manejo de rutas (`PATH`)  
- Redirecciones de entrada y salida  
- Pipes (`|`)  
- Manejo de variables de entorno  
- Señales (Ctrl+C, Ctrl+D, Ctrl+\)  

Este proyecto nos ayuda a entender cómo funciona un shell por dentro, profundizando en conceptos de **procesos**, **sistemas de archivos**, y **manejo de señales** en C.

---
static char *get_prompt(void)
	{
	static char	prompt[MAX_PROMPT_SIZE];
	char		*username;
	char		cwd[512];

	username = getenv("USER");
	if(!username)
		username = getenv("LOGNAME");
	if(!username)
		username = "guest";
	if(!getcwd(cwd, sizeof(cwd)))
		cwd[0] = '\0';
	snprintf(prompt,MAX_PROMPT_SIZE,BPURPLE"%s@minishell~"YELLOW"%s->"RESET,username,cwd);
	return(prompt);
	}
