#include <stdio.h>
#include <stdlib.h>
#include <math.h>

enum {
	CONST,
	LINEAR
};

enum {
	FALSE,
	TRUE
};

enum state {
	SUSCEPTIBLE,
	INFECTED,
	RECOVERED
};

typedef struct Network {
	struct Node* nodes;
	struct Edge* edges;
	struct Cluster* clusters;
	int time;
	int order;
	int size;
	int clst_count;
} _Network;

struct Node {
	int id;
	char state;
	struct Node* next;
	int clusters;
	char isolated;
};

struct Edge {
	int node1;
	int node2;
	struct Edge* next;
};

struct Cluster {
	int id;
	float prob;
	struct Cluster* next;
};

struct Network_sim {
	struct Network_state* timeline;
	int time;
	int order;
};

struct Network_state {
	int n_sus;
	int n_inf;
	int n_rec;
	struct Network_state* next;
};

void add_cluster (struct Network* network, float prob);

void print_nodes (struct Network* network);

char p_test (float p);

struct Node* get_node (struct Network* network, int id);

char nodes_clustered (struct Node* node1, struct Node* node2);

struct Network* generate_network (
	int clst_count,
	int clst_dist,
	float con,
	float clst_con,
	int order,
	int size
	);

void visualize_simulation (struct Network_sim* network_sim);

void export_simulation (char* dir, struct Network_sim* sim);

struct Network_sim* simulate (
		struct Network* network,
		int infected_0,
		float p_sus_inf,
		float p_inf_rec,
		float p_rec_inf,
		int time
	);
