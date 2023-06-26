#if defined(DEBUG)
#define MY_ASSERT(condition)                                                                             \
        if (!(condition))                                                                                \
        {                                                                                                \
            fprintf(stderr,"ERROR IN FILE %s, FUNCTION %s, LINE %d\n", __FILE__, __FUNCTION__, __LINE__);\
            int choice = 0;                                                                              \
            while (choice != 'Y' || choice != 'y' || choice != 'N' || choice != 'n')                     \
            {                                                                                            \
                fprintf(stderr,"Do you want to continue 'Y' - Yes, 'N' - No\n");                         \
                int choice = getchar ();                                                                 \
                if (choice == 'Y' || choice == 'y')                                                      \
                {                                                                                        \
                    break;                                                                               \
                }                                                                                        \
                                                                                                         \
                else if (choice == 'N' || choice == 'n')                                                 \
                {                                                                                        \
                    exit (1);                                                                            \
                }                                                                                        \
                                                                                                         \
                while (getchar() == '\n');                                                               \
            }                                                                                            \
        }                                                                                                \

#else
#define ASSERT(condition);

#endif

