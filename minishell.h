#ifndef MINISHELL_H
# define MINISHELL_H

/* ───── Standard Libraries ───── */
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <dirent.h>
# include <fnmatch.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ───── Project Modules ───── */
# include "lexer/lexer.h"
# include "gc/gc.h"
# include "utils/utils.h"
# include "parser/parser.h"
# include "debugging/debugging_tools.h"
# include "executor/executor.h"
# include "signals/signals.h"

/* ───── Constants ───── */
# define PROMPT "minishell$ "
# ifndef DEBUG_MODE
#  define DEBUG_MODE 0
# endif

/* ───── Global Variable ───── */
extern volatile sig_atomic_t	g_in_prompt;

/* ───── Minishell Context Struct ───── */
typedef struct s_minishell
{
	t_gc		gc;
	t_gc		env_gc;
	t_token		*tokens;
	int			last_exit_status;
	char		**envp;
	int			in_logical_or_pipe;
}	t_minishell;

#endif