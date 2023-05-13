#include "language.h"

int main ()
{
    log_init      (TREE_LOGS_HTML_DIR);

    frontend_utils (TREE_REC_DESCENT_DIR);

    log_distr   ();
}
