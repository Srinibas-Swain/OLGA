from typing import List, Dict

from .coloration_order import ColorationOrder
from .dsatur import DSatur
from h2py.generic.graph_types import GraphTypes

Color = GraphTypes.Color
Colors = GraphTypes.Colors
ColorSet = GraphTypes.ColorSet
ColorSets = List[ColorSet]  # List of Set of Colors
Index = GraphTypes.Index
UGraphT = GraphTypes.UGraph
Vertex = GraphTypes.Vertex
Vertices = GraphTypes.Vertices

# vx => vertex
# vix => vertex index


class Brelaz:
    """Rank of a vertex: Position of the vertex in the coloration order """
    def __init__(self, graph, verbose=False):
        self.graph: UGraphT = graph
        self.N: int = graph.num_vertices
        self.verbose = verbose

        # get the coloration order
        self.coloration_order: Vertices = ColorationOrder.get(graph)
        self.log('coloration order: {}', self.coloration_order)

        # set ranks: reverse map of coloration_order
        # rank of a vertex: position of the vertex in the coloration order
        self.ranks: List[int] = [None for _ in range(self.N)]
        for rank, vx in enumerate(self.coloration_order):
            self.ranks[vx] = rank

        # get the initial coloration and approximate max-clique
        coloration_apprx, max_clique = DSatur(graph, verbose=False).compute()
        self.coloration_apprx: Colors = coloration_apprx
        self.max_clique: Vertices = max_clique
        self.log('coloration: {} max-clique: {}', self.coloration_apprx, self.max_clique)

        # set w, r, q
        self.max_clique_size_w: int = len(self.max_clique)         # variable w in the paper: size of the clique
        self.greedy_coloration_r: int = len(set(self.coloration_apprx))  # variable r in the paper: num colors
        self.best_coloration_q: Color = -1  # best (min) number of colors so far which can color the graph

        #  per-vertex possible colors - init
        # This is used in the backtracking step to reinitiliaze the possible_colors for higher order vertices
        self.possible_colors_init: ColorSets = [set(range(self.N)) for _ in range(self.N)]

        # per-vertex possible colors
        self.possible_colors: ColorSets = [color_set.copy() for color_set in self.possible_colors_init]

        self.colors: Colors = [None for _ in range(self.N)]  # per-vertex coloration
        self.labels = [-1 for _ in range(self.N)]  # labels as used in the paper

        self.log('w: {} r: {}', self.max_clique_size_w, self.greedy_coloration_r)

    def compute(self) -> Colors:
        # step 1: w == r:  couldn't go below this, so return
        if self.max_clique_size_w == self.greedy_coloration_r:
            return self.coloration_apprx

        # step 2
        self.color_clique()
        self.update_possible_colors()

        # step 3
        vix_k: Index = self.color_init_step_3()

        # step 4
        vix_k, vx_k, u_xk = self.increment_k_step_4(vix_k)

        return self.color_vertex_step_5(vix_k, vx_k, u_xk)

    def color_clique(self) -> None:
        """Color the vertices of the clique with colors 0 .. len(self.max_clique)-1"""
        for vix, vx in enumerate(self.max_clique):
            self.colors[vx] = vix

    def update_possible_colors(self) -> None:
        """Update the possible colors of the non-clique vertices.
           U(x_k) = U(x_k) \setdiff j, where j is the color of a K vertex (clique vertex),
           which is adjacent to x_k"""
        is_edge = self.graph.is_edge
        non_clique_vertices = set(range(self.N)).difference(set(self.max_clique))  # non-clique vertices
        for vx_clique in self.max_clique:
            color_vx_clique: Color = self.colors[vx_clique]
            for vx_non_clique in non_clique_vertices:
                if not is_edge(vx_clique, vx_non_clique):
                    continue
                if color_vx_clique in self.possible_colors[vx_non_clique]:
                    self.possible_colors[vx_non_clique].remove(color_vx_clique)

    def color_init_step_3(self) -> Index:
        vix_k: Index = self.max_clique_size_w + 1          # k = w + 1
        self.best_coloration_q = self.greedy_coloration_r  # q = r

        vx_xk: Vertex = self.coloration_order[vix_k]
        color_k: Color = min(self.possible_colors[vx_xk])  # color x_k with min possible color
        self.colors[vx_xk] = color_k
        self.possible_colors[vx_xk].remove(color_k)        # and remove this color from U(x_k)
        # TODO: Prohibit this color for all the vertices which are adjacent to xk until
        #       there is a modification of the x_k color.

        self.log('[3] colors[{}] = {}', vx_xk, color_k)

        return vix_k

    def increment_k_step_4(self, vix_k: Index) -> (Index, Vertex, ColorSet):
        """Increment vertex_k and compute U(x_k)
           :returns: vertex_index_k, vertex_xk, U_xk (possible_colors for vertex_xk)"""
        vix_k += 1  # index k (into self.coloration_order)
        vx_xk: Vertex = self.coloration_order[vix_k]

        u_xk = self.compute_u_xk(vx_xk)
        self.log("[4] vix: {} vx: {} u_xk: {}", vix_k, vx_xk, u_xk)
        return vix_k, vx_xk, u_xk

    def compute_u_xk(self, vx_xk):
        nbr_colors: ColorSet = set(self.colors[nbr]
                                   for nbr in self.graph.neighbors(vx_xk)
                                   if self.colors[nbr] is not None)
        u_xk: ColorSet = nbr_colors.intersection(self.possible_colors[vx_xk])
        return u_xk

    def color_vertex_step_5(self, vix_k: Index, vx_k: Vertex, u_xk: ColorSet) -> Colors:
        if len(u_xk) == 0:  # no (possible) colors available
            self.log("[5] vix: {} vx: {} len(u_xk) == 0", vix_k, vx_k)
            return self.backtrack_step_8(vix_k, vx_k)  # go to 10, which goes to 8

        color_i = min(u_xk)
        self.colors[vx_k] = color_i
        self.possible_colors[vx_k].remove(color_i)
        # TODO: prohibit this color for the vertices which are adjacent to x_k (with greater rank) until
        # there is a modification of the x_k color
        # self.update_higher_nbrs(vx_k, color_i)

        if color_i >= self.best_coloration_q:  # is this a worse (partial) coloration?
            self.log("[5] WORSE vix: {} vx: {} color: {} q: {}", vix_k, vx_k, color_i, self.best_coloration_q)
            return self.backtrack_step_8(vix_k, vx_k)  # backtrack: go to 8

        # step 6: not a worse coloration
        if vx_k == (self.N - 1):  # k == n: complete coloration
            self.log("[5] COMPLETE vix: {} vx: {}", vix_k, vx_k)
            return self.complete_coloration_step_7()
        else:
            self.log("[5] INCOMPLETE vix: {} vx: {}", vix_k, vx_k)
            vix_k, vx_k, u_xk = self.increment_k_step_4(vix_k)
            return self.color_vertex_step_5(vix_k, vx_k, u_xk)

    # got a complete coloration, now figure what to do...
    def complete_coloration_step_7(self) -> Colors:
        self.best_coloration_q: Color = len(set(self.colors))  # q = L
        if self.best_coloration_q == self.max_clique_size_w:  # min chromatic number reached?
            self.log("[7] STOP q: {} w: {}", self.best_coloration_q, self.max_clique_size_w)
            return self.colors  # STOP

        vix_j, vx_j = self.q_colored_vertex_with_minimal_rank(self.best_coloration_q)  # ?? what is a q-colored vertex?
        if self.ranks[vx_j] == self.max_clique_size_w + 1:  # ?? why is this required?
            self.log("[7] STOP rank[x_j = {},{}] == (w+1 = {})", vix_j, vx_j, self.max_clique_size_w+1)
            return self.colors  # STOP

        vix_j_m_1: Index = vix_j - 1  # ugly
        self.log("[7] GOOD k = j-1: {}", vix_j_m_1)
        return self.color_vertex_step_5(vix_j_m_1, self.coloration_order[vix_j_m_1], self.possible_colors[vix_j_m_1])

    def q_colored_vertex_with_minimal_rank(self, color_q: Color) -> (Index, Vertex):
        """Returns the vertex of color_q with the minimal rank (according to coloration_order)"""
        for rank, vx in enumerate(self.coloration_order):
            if self.colors[vx] == color_q:
                return rank, vx
        raise Exception("No vertex of color {} found".format(color_q))

    # sth is bad, so backtrack ...
    def backtrack_step_8(self, vix_k: Index, vx_k: Vertex = None) -> Colors:
        # TODO: reset the possible colors for higher order vertices
        if vix_k == self.max_clique_size_w + 1:
            self.log("[8] STOP k ({}) == w+1 ({})", vix_k, self.max_clique_size_w+1)
            return self.colors  # STOP

        vx_k: Vertex = self.coloration_order[vix_k] if vx_k is None else vx_k
        rank_k: int = self.ranks[vx_k]

        nbrs_k_by_color = self.get_nbrs_by_color(vx_k)

        vxs_labelled = []
        for rank, vx in self.coloration_order[:rank_k]:  # consider vertices with lesser rank
            color_vx: Color = self.colors[vx]
            # minimal rank among all the vertices of their color, which are adjacent to k.
            rank_min = min(self.ranks[nbr_k_color_vx] for nbr_k_color_vx in nbrs_k_by_color[color_vx])

            if (self.graph.is_edge(vx_k, vx)) and (self.ranks[vx] == rank_min):
                vxs_labelled.append(vx)
                self.labels[vx] = vix_k
                # TODO: once we obtain a rank k or more in a partial coloration, should remove this label
                # do this in increment_k_step_4

        # step 9
        vix_v: Index = vix_k
        max_rank_t: Index = max(self.ranks[vx_labelled] for vx_labelled in vxs_labelled)
        vix_k: Index = max_rank_t

        # reset colors: for k < i <= k_old, U(x_i) is the set of colors defined in step 3
        for vix in range(vix_k+1, vix_v+1):
            vx = self.coloration_order[vix]
            self.colors[vx] = None
            self.possible_colors[vx] = self.possible_colors_init[vx].copy()

        vx_k: Vertex = self.coloration_order[vix_k]
        self.log("[8] v=k_old: {} k_new: {} vxs_labelled: {} vx_k: {}", vix_v, vix_k, vxs_labelled, vx_k)

        return self.color_vertex_step_5(vix_k, vx_k, self.compute_u_xk(vx_k))

    def get_nbrs_by_color(self, vx_k) -> Dict[Color, Vertices]:
        nbrs_k = [vx for vx in range(self.N) if self.graph.is_edge(vx_k, vx)]
        nbrs_k_by_color: Dict[Color, Vertices] = dict()
        for nbr_k in nbrs_k:
            color_nbr: Color = self.colors[nbr_k]
            if color_nbr not in nbrs_k_by_color:
                nbrs_k_by_color[color_nbr] = []
            nbrs_k_by_color[color_nbr].append(nbr_k)
        return nbrs_k_by_color

    def log(self, print_string, *args):
        print_string = "brelaz: " + print_string
        if self.verbose:
            if len(args) == 0:
                print(print_string)
            else:
                print(print_string.format(*args))
