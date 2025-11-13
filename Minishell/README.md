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
├── includes/           # Archivos de cabecera
│   ├── minishell.h    # Header principal
│   └── libft.h        # Header de libft
├── libft/             # Biblioteca libft
├── src/               # Código fuente organizado por módulos
│   ├── core/          # 🎯 Archivos principales
│   │   ├── main.c     # Punto de entrada del programa
│   │   ├── prompt.c   # Gestión del prompt y loop principal
│   │   └── utils_main.c # Utilidades de inicialización
│   ├── parsing/       # 📝 Análisis y procesamiento de entrada
│   │   ├── tokenizer.c # Tokenización de comandos
│   │   ├── parser.c   # Análisis sintáctico
│   │   ├── expand.c   # Expansión de variables
│   │   └── utils_*.c  # Utilidades de parsing
│   ├── execution/     # ⚡ Ejecución de comandos
│   │   ├── executor.c # Motor de ejecución
│   │   └── utils_executor.c # Utilidades de ejecución
│   ├── builtins/      # 🔧 Comandos integrados
│   │   ├── builtins.c # Gestión general de builtins
│   │   ├── cd.c       # Comando cd
│   │   ├── echo.c     # Comando echo
│   │   ├── env.c      # Comando env
│   │   ├── pwd.c      # Comando pwd
│   │   ├── export.c   # Comando export
│   │   ├── unset.c    # Comando unset
│   │   └── utils_export.c # Utilidades de export
│   └── utils/         # 🛠️ Utilidades generales
│       ├── errors.c   # Manejo de errores
│       ├── free.c     # Gestión de memoria
│       └── signals.c  # Manejo de señales
├── obj/               # Archivos objeto (estructura espejo de src/)
└── Makefile          # Sistema de construcción
```

## 🛠️ Instalación

### Prerrequisitos
- GCC o Clang
- Make
- Valgrind (para tests de memoria)

### Clonar el repositorio
```bash
git clone [https://github.com/ctaboram/minishell.git]
cd minishell
```

## 🚀 Compilación y Uso

### Compilar el proyecto
```bash
make                    # Compilación completa
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
- **Gestión de memoria** - Sin memory leaks

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
- Escrito en C según la Norma
- Sin segmentation faults o comportamiento indefinido
- Gestión adecuada de memoria (sin leaks)
- Manejo correcto de errores

### ✅ **Estructura**
- Libft en carpeta dedicada
- Headers organizados en `includes/`
- Código fuente modular en `src/`

## 👥 Autores

- **ctaboada** - Contribuidor principal
- **nacuna-g** - Contribuidor principal

## 📝 Licencia

Este proyecto es de código abierto y está disponible bajo los términos del proyecto académico de 42.

## 🤝 Contribuciones

Este proyecto fue desarrollado como parte del cursus de 42. Las contribuciones están limitadas a los autores del proyecto.

---

<div align="center">
  <sub>Desarrollado con ❤️ como parte del cursus de <a href="https://42.fr">42</a></sub>
</div>
