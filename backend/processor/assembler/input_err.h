#define INPUT_ERR(format, ...)                                                                                                                \
        {                                                                                                                                     \
            fprintf (stderr,"Error of input of commands: \033[91m ");                                                                         \
            fprintf (stderr, format __VA_OPT__(,) __VA_ARGS__);                                                                               \
            fprintf (stderr,"\033[0m\nPlese check FILE: %s, LINE %d\n", NAME_OF_PROG, (commands[cur_elem].num_of_line + 1));                  \
            exit (-1);                                                                                                                        \
        }                                                                                                                                     \
;
