#ifndef MINISHELL_H
# define MINISHELL_H

/* ───── Standard Libraries ───── */
# include <stdlib.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <sys/wait.h>
# include <dirent.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ───── Project Modules ───── */
# include "lexer.h"
# include "gc.h"

/* ───── Prompt ───── */
# define PROMPT "minishell$ "

/* ───── Minishell Context Struct ───── */
typedef struct s_minishell
{
    t_gc		gc;
    t_token		*tokens;
    int			last_exit_status;
    int			is_running;
}	t_minishell;

#endif