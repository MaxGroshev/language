#include "language.h"

int main ()
{
    log_init      (TREE_LOGS_HTML_DIR);

    frontend_utils (TREE_REC_DESCENT_DIR);
    backend_utils  ("./tree_stand/std_format.ast");

    log_distr   ();
}
