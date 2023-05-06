#include "graphviz.h"

void init_graph (dump_graph_t* graph_dump_set, const char* dot_dir)
{
    graphviz = fopen (dot_dir,  "w");
    MY_ASSERT (graphviz != NULL)

    graph_dump_set->edge_capacity = 10;
    graph_dump_set->node_capacity = 10;
    graph_dump_set->node_size     = 0;
    graph_dump_set->edge_size     = 0;
    graph_dump_set->nodes = (struct node_t*) calloc (graph_dump_set->node_capacity, sizeof (struct node_t)); //add different types
    graph_dump_set->edges = (struct edge_t*) calloc (graph_dump_set->edge_capacity, sizeof (struct edge_t));
    MY_ASSERT (graph_dump_set->nodes != NULL && graph_dump_set->edges != NULL);

    struct node_t init_struct_node;
    struct edge_t init_struct_edge;
    for (int i = 0; i < graph_dump_set->node_capacity; i++)
    {
        graph_dump_set->nodes[i] = init_struct_node;
    }
    for (int i = 0; i < graph_dump_set->edge_capacity; i++)
    {
        graph_dump_set->edges[i] = init_struct_edge;
    }

    print_def_info (graph_dump_set);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void print_int_node (dump_graph_t* graph_dump_set, double* node_address, struct node_t nodes, double* right, double* left, int value)
{
    if (graph_dump_set->node_capacity <= graph_dump_set->node_size + 1) resize_struct (graph_dump_set);

    fprintf (graphviz, "node%p [shape = \"%s\", fillcolor = \"%s\", style = \"%s\", fontcolor = \"%s\", fontname = \"%s\", color = \"%s\", label = \"{%s\\n (%p)| {%d |left = %p |right = %p}}\"]\n",
                node_address, nodes.shape,  nodes.fillcolor,    nodes.style,    nodes.fontcolor,    nodes.fontname,    nodes.color,   nodes.label , node_address, value,     left,      right      );
    graph_dump_set->node_size++;
}

void print_char_node (dump_graph_t* graph_dump_set, double* node_address, struct node_t nodes, double* right, double* left, char value)
{
    if (graph_dump_set->node_capacity <= graph_dump_set->node_size + 1) resize_struct (graph_dump_set);

    fprintf (graphviz, "node%p [shape = \"%s\", fillcolor = \"%s\", style = \"%s\", fontcolor = \"%s\", fontname = \"%s\", color = \"%s\", label = \"{%s\\n (%p)| {%c |left = %p |right = %p}}\"]\n",
                node_address, nodes.shape,  nodes.fillcolor,    nodes.style,    nodes.fontcolor,    nodes.fontname,    nodes.color,   nodes.label , node_address, value,     left,      right      );
    graph_dump_set->node_size++;
}

void print_str_node (dump_graph_t* graph_dump_set, double* node_address, struct node_t nodes, double* right, double* left, const char* value)
{
    if (graph_dump_set->node_capacity <= graph_dump_set->node_size + 1) resize_struct (graph_dump_set);

    fprintf (graphviz, "node%p [shape = \"%s\", fillcolor = \"%s\", style = \"%s\", fontcolor = \"%s\", fontname = \"%s\", color = \"%s\", label = \"{%s\\n (%p)| {%s |left = %p |right = %p}}\"]\n",
                node_address, nodes.shape,  nodes.fillcolor,    nodes.style,    nodes.fontcolor,    nodes.fontname,    nodes.color,   nodes.label , node_address, value,     left,      right      );
    graph_dump_set->node_size++;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void make_edge (dump_graph_t* graph_dump_set, double* node_from, double* node_to, struct edge_t edges)
{
    if (graph_dump_set->edge_capacity <= graph_dump_set->edge_size + 1) resize_struct (graph_dump_set);

    fprintf (graphviz, "node%p -> node%p [color = \"%s\", style = \"%s\", constraint = %s, fillcolor = \"%s\",   fontcolor = \"%s\", fontname = \"%s\", label = \"%s\"];\n",
                        node_from, node_to, edges.color,   edges.style,    edges.constraint, edges.fillcolor, edges.fontcolor,    edges.fontname,     edges.label);
    graph_dump_set->edge_size++;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void print_def_info (dump_graph_t* graph_dump_set)
{
    fprintf (graphviz, "digraph   \n{\n");
    fprintf (graphviz, "rankdir = \"%s\"\n",   graph_dump_set->orientation);
    fprintf (graphviz, "splines = \"%s\"\n\n", graph_dump_set->splines);

    fprintf (graphviz, "node_stat [margin = \"0.3*0.3\", style = \"filled\", shape = \"record\", fillcolor = \"#8DB6CD\" label = \" size = %ld | root point = %p\"]\n",
    graph_dump_set->info.size, graph_dump_set->info.head);
    fprintf (graphviz, "{rank = source; node_stat}\n\n");
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

void resize_struct (dump_graph_t* graph_dump_set)
{
    if (graph_dump_set->edge_capacity <= graph_dump_set->edge_size + 1)
    {

        size_t prev_capacity = graph_dump_set->edge_capacity ;
        graph_dump_set->edge_capacity *= 2;
        edge_t* edges_resize = (edge_t*) realloc (graph_dump_set->edges,  graph_dump_set->edge_capacity * sizeof (struct edge_t));

        MY_ASSERT (edges_resize != NULL);
        graph_dump_set->edges = edges_resize;
        struct edge_t init_struct_edge;
        for (int i = prev_capacity; i < graph_dump_set->edge_capacity; i++)
        {
            graph_dump_set->edges[i] = init_struct_edge;
        }
    }

    if (graph_dump_set->node_capacity <= graph_dump_set->node_size + 1)
    {
        size_t prev_capacity = graph_dump_set->node_capacity;
        graph_dump_set->node_capacity *= 2;
        node_t* node_resize = (node_t*) realloc (graph_dump_set->nodes,  graph_dump_set->node_capacity * sizeof (struct node_t));

        MY_ASSERT (node_resize != NULL);
        graph_dump_set->nodes = node_resize;
        struct node_t init_struct_node;
        for (int i = prev_capacity; i < graph_dump_set->node_capacity; i++)
        {
            graph_dump_set->nodes[i] = init_struct_node;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

int run_graphviz  (dump_graph_t* graph_dump_set, const char* dot_dir, const char* pic_dir)
{
    MY_ASSERT (dot_dir != NULL && pic_dir != NULL)
    static int num_of_print = 0;
    num_of_print++;

    fprintf (graphviz, "}\n");
    free    (graph_dump_set->nodes);
    free    (graph_dump_set->edges);
    fclose  (graphviz);

    const int buf_size = (strlen (dot_dir) + strlen (pic_dir) + 64) * sizeof (char);
    char buf [buf_size];

    sprintf (buf, "dot %s -T png -o %s/tree_d%d.png", dot_dir, pic_dir, num_of_print);
    call_system (buf);

    sprintf (buf, ".%s/tree_d%d.png", pic_dir, num_of_print);
    pic_log ("Here is the print of your tree", buf);

    return 0;
}

void call_system (const char* command)
{
    int dupper = 0;
    dupper = (2);
    close (2);
    system (command);
    dup2 (dupper, 2);
    close (dupper);
}
