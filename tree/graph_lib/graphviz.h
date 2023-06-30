#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include "../tree.h"
#include "my_ASSERT.h"

//---------------------------------------------------------------------------------------------------------

static FILE* graphviz = NULL;

//---------------------------------------------------------------------------------------------------------

struct node_t
{
    const char* shape     = "Mrecord";
    const char* fillcolor = "#eed8cd";
    const char* style     = "filled";
    const char* fontcolor = "#31032A";
    const char* fontname  = "Calibri";
    const char* color     = "white";
    const char* margin    = "";
    const char* label     = "";
};

struct edge_t
{
    const char* constraint = "true";
    const char* fillcolor  = "#8DB6CD";
    const char* style      = "";
    const char* fontcolor  = "";
    const char* fontname   = "Calibri";
    const char* color      = "#8DB6CD";
    const char* label      = "";
};

struct data_stat_t
{
    size_t capacity;
    size_t size;
    double*   head;
    size_t tail;
    const char* save_pic_to = NULL;
};

struct dump_graph_t
{
    const char*         orientation = "";
    const char*         splines = "true";
    struct node_t*      nodes;
    struct edge_t*      edges;
    struct data_stat_t  info;
    int                 print_log = 0;
    size_t              node_size;
    size_t              node_capacity;
    size_t              edge_size;
    size_t              edge_capacity;
};

//--------------------------------------------------------------------------------------------------------------------------

void init_graph      (dump_graph_t* graph_dump_set, const char* dot_dir);

void print_val_node  (dump_graph_t* graph_dump_set, double* node_address, struct node_t nodes, double* right, double* left, double value);
void print_char_node (dump_graph_t* graph_dump_set, double* node_address, struct node_t nodes, double* right, double* left, char value);
void print_str_node  (dump_graph_t* graph_dump_set, double* node_address, struct node_t nodes, double* right, double* left, const char* value);
void make_edge       (dump_graph_t* graph_dump_set, double* node_from, double* node_to, struct edge_t edges);
void resize_struct   (dump_graph_t* graph_dump_set);
void print_def_info  (dump_graph_t* graph_dump_set);
int  run_graphviz    (dump_graph_t* graph_dump_set, const char* dot_dir, const char* pic_dir);
void call_system     (const char* command);

