#ifndef GRAPH_CONNECTIVITY_HH
#define GRAPH_CONNECTIVITY_HH

#include "menu_option.hh"
#include <unordered_map>

namespace ga {
        class GraphConnectivity : public MenuOption {
        public:
                typedef typename std::unordered_map<std::string, std::string> Graph_Conn_map;
                GraphConnectivity();
                ~GraphConnectivity();

                void print_args_input_info() override;
                void get_args_input(char split_char) override;
                void init() override;
                void init_for_new_n(int n) override;
                void process() override;
                void process_graph(const Graph* graph) override;
                void post_processing() override;
                void print_args() override;

                void init_base_cases();

                void read_db_from_file();
                bool is_db_present();
                void write_db_to_file();

                void save(const Graph* graph, Graph_Conn_map);
                Graph_Conn_map get(const Graph* graph);
                //- custom stuff

        protected:
                void init_objects() override;

        private:
                std::string fi_db = "./data/d_db_graph_Connectivity_map.csv";
                std::unordered_map<int, std::unordered_map<long long, Graph_Conn_map > > db_gc;

                Graph_Conn_map get(int num_vertices, long long certificate);
                void save(int num_vertices, long long certificate, Graph_Conn_map);
        };
}

#endif /* GRAPH_CONNECTIVITY_HH */

