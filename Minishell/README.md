# 🐚 Minishell

[![42 Project](https://img.shields.io/badge/42-Project-blue?style=flat-square)](https://42.fr)
[![Made with C](https://img.shields.io/badge/Made%20with-C-blue?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Norminette](https://img.shields.io/badge/Norminette-passing-green?style=flat-square)](https://github.com/42School/norminette)

Un shell simple implementado en C como parte del proyecto 42.

> **🎯 Objetivo**: Recrear un shell básico con funcionalidades fundamentales de bash, incluyendo parsing, ejecución de comandos, pipes, redirections y built-ins.

## 📋 Tabla de Contenidos

- [Estructura del Proyecto](#-estructura-del-proyecto)
- [Instalación](#-instalación)
- [Compilación y Uso](#-compilación-y-uso)
- [Características](#-características)
- [Arquitectura](#-arquitectura)
- [Cumplimiento de Normas 42](#-cumplimiento-de-normas-42)
- [Autores](#-autores)

## 📁 Estructura del Proyecto

```
minishell/
├── includes/
│   ├── minishell.h
│   └── libft.h
│
├── libft/
│
├── src/
│   ├── builtins/
│   │   ├── builtins.c
│   │   ├── cd.c
│   │   ├── echo.c
│   │   ├── env.c
│   │   ├── exit.c
│   │   ├── export.c
│   │   ├── pwd.c
│   │   ├── unset.c
│   │   ├── utils_cd.c
│   │   ├── utils_export.c
│   │   ├── utils_export2.c
│   │   ├── utils_export3.c
│   │   └── utils_unset.c
│
│   ├── core/
│   │   ├── init_structs.c
│   │   ├── main.c
│   │   ├── prompt.c
│   │   └── utils_main.c
│
│   ├── execution/
│   │   ├── executor.c
│   │   ├── utils_executor.c
│   │   ├── utils_executor2.c
│   │   ├── utils_executor3.c
│   │   └── utils_executor4.c
│
│   ├── parsing/
│   │   ├── expand.c
│   │   ├── parser.c
│   │   ├── tokenizer.c
│   │   ├── utils_expand.c
│   │   ├── utils_parser.c
│   │   ├── utils_parser2.c
│   │   ├── utils_parser3.c
│   │   ├── utils_tokenizer.c
│   │   └── utils_tokenizer2.c
│
│   └── utils/
│       ├── errors_child.c
│       ├── errors.c
│       ├── free.c
│       ├── signals.c
│       └── signals2.c
│
├── Makefile
└── README.md

```

## 🛠️ Instalación

### Prerrequisitos
- CC
- Make
- Valgrind (para tests de memoria)

### Clonar el repositorio
```bash
git clone https://github.com/ctaboram/minishell.git
cd minishell
```

## 🚀 Compilación y Uso

### Compilar el proyecto
```bash
make                   # Compilación completa
make clean             # Limpiar archivos objeto
make fclean            # Limpieza completa
make re                # Recompilar desde cero
```

### 🧪 Testing

#### Tests Manuales Recomendados
```bash
# Comandos básicos
ls -la
echo "Hello World"
pwd
env

# Pipes y redirections
ls | grep minishell
echo "test" > output.txt
cat < output.txt
ls | wc -l

# Variables
export TEST=hello
echo $TEST
echo $?

# Built-ins
cd ..
pwd
unset TEST
exit
```

### Ejecutar
```bash
./minishell
```

## ✨ Características

- **Tokenización avanzada** - Manejo de quotes, pipes y redirections
- **Expansión de variables** - Soporte para `$VAR` y `$?`
- **Comandos built-in** - `cd`, `echo`, `env`, `export`, `unset`, `pwd`, `exit`
- **Pipes y redirections** - Soporte para `|`, `<`, `>`, `>>`
- **Manejo de señales** - `Ctrl+C`, `Ctrl+D`, `Ctrl+\`

## 🏗️ Arquitectura

El proyecto sigue una arquitectura modular donde cada componente tiene responsabilidades específicas:

1. **Core** - Inicialización y control principal
2. **Parsing** - Análisis léxico y sintáctico
3. **Execution** - Ejecución de comandos y pipes
4. **Builtins** - Implementación de comandos integrados
5. **Utils** - Servicios transversales

## 📜 Cumplimiento de Normas 42

Este proyecto cumple estrictamente con todas las normas requeridas:

### ✅ **Makefile**
- Usa flags obligatorios: `-Wall -Wextra -Werror`
- Compilador: `cc`
- Reglas requeridas: `$(NAME)`, `all`, `clean`, `fclean`, `re`
- Sin relinking innecesario
- Compila libft usando su propio Makefile

### ✅ **Código**
- Escrito en C según la Norma V4
- Sin segmentation faults o comportamiento indefinido
- Gestión adecuada de memoria (sin leaks)
- Manejo correcto de errores

## 👥 Autores

- **ctaboada** -
- **nacuna-g** -

## 📝 Licencia

Este proyecto es de código abierto y está disponible bajo los términos del proyecto académico de 42.

---

<div align="center">
  <sub>Desarrollado con ❤️ como parte del cursus de <a href="https://www.42malaga.com/">42</a></sub>
</div>
