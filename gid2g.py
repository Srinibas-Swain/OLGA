import graphviz as gv
import math
import numpy as np
def gid_to_adj_matrix(num_vertices, graph_id):
    adj_matrix = [[False for _ in range(num_vertices)] for _ in range(num_vertices)]
    graph_id_orig = graph_id
    for col in reversed(range(1,num_vertices)):
        for row in reversed(range(col)):
            remainder = graph_id % 2
            print('({0},{1}): {2},{3}'.format(row,col,graph_id,remainder))
            adj_matrix[row][col] = (remainder == 1)
            graph_id //= 2
    return adj_matrix
def adj_to_gv(adj_matrix):
    num_vertices = len(adj_matrix)
    g = gv.Graph(engine='neato')
    for ix in range(num_vertices):
        g.node(str(ix))
    for row in range(num_vertices):
        for col in range(row, num_vertices):
            if(adj_matrix[row][col]):
                g.edge(str(row),str(col))
    return g

def gid_to_gv(num_vertices, graph_id):
    return adj_to_gv(gid_to_adj_matrix(num_vertices, graph_id))

print(gid_to_gv(4,13))
