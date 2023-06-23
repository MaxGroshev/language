#if defined(DEBUG)
#define STACK_CHECK                                                                                       \
        if (stack_dump(box,__FILE__, __FUNCTION__, __LINE__, 0) != STACK_IS_OKEY)                         \
        {                                                                                                 \
            exit(1);                                                                                      \
        }                                                                                                 \

#else
#define PRINT_FAIL ;

#endif
