#include "language.h"


int main ()
{
    log_init      (TREE_LOGS_HTML_DIR);

    struct tree_t pine = {};
    constructor (&pine);

    pine.root = rec_descent (TREE_REC_DESCENT_DIR);
    //graph_dump (pine.root);

    tree_delete (pine.root);
    log_distr   ();
}
