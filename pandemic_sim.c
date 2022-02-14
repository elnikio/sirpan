#include "pandemic_sim.h"

void add_cluster (_Network* network, float prob) {
	int id = 0;
	struct Cluster* new = malloc(sizeof(struct Cluster));
	struct Cluster* last;

	if (network -> clusters == NULL) {
		network -> clusters = new;
	}
	else {
		id ++;
		last = network -> clusters;
		while (last -> next != NULL) {
			id ++;
			last = last -> next;
		}
		last -> next = new;
	}

	new -> id = id;
	new -> prob = prob;
}

void print_nodes (_Network* network) {
	struct Node* node = network -> nodes;
	while (node != NULL) {
		switch (node -> state){
			case SUSCEPTIBLE:
				printf("\e[0;32m ● \e[0m%d : susceptible", node -> id);
				break;
			case INFECTED:
				printf("\e[0;31m ● \e[0m%d : infected", node -> id);
				break;
			case RECOVERED:
				printf("\e[0;37m ● \e[0m%d : recovered", node -> id);
				break;
		}
		putchar('\n');
		node = node -> next;
	}
}

char p_test (float p) {
	return (((float)rand() / RAND_MAX) < p) ? TRUE : FALSE;
}

struct Node* get_node (_Network* network, int id) {
	struct Node* node = network -> nodes;
	while (node != NULL) {
		if (node -> id == id) return node;
		node = node -> next;
	}
	return NULL;
}

char nodes_clustered (struct Node* node1, struct Node* node2) {
	return (node1 -> clusters) & (node2 -> clusters) ? TRUE : FALSE;
}

_Network* generate_network (
	int clst_count,
	int clst_dist,
	float con,
	float clst_con,
	int order,
	int size
	) {
	_Network* network = malloc(sizeof(_Network));
	int first_unused = 0;
	int i;
	int j;
	int k;
	float clst_size;
	int seed;
	struct Node* node = NULL;
	struct Node* last_node = NULL;
	struct Cluster* cl;
	struct Edge* edge = malloc(sizeof(struct Edge));
	struct Edge* last_edge;
	int node1;
	int node2;
	float prob;

	network -> nodes = NULL;
	network -> edges = NULL;
	network -> time = 0;
	network -> clusters = NULL;
	network -> order = order;
	network -> size = size;
	network -> clst_count = clst_count;

	switch (clst_dist) {
		case CONST:
			for (i = 0; i < clst_count; i ++) {
				clst_size = (float)order / clst_count;
				add_cluster (network, clst_size / order);
			}
			break;
		case LINEAR:
			for (i = 0; i < clst_count; i ++) {
				j = first_unused;
				k = 0;
				first_unused = j + k;
			}
			break;
	}

	srand ((unsigned)&seed);

	for (i = 1; i <= order; i ++) {
		last_node = node;
		node = malloc(sizeof(struct Node));
		if (last_node == NULL)
			network -> nodes = node;
		node -> id = i;
		node -> state = SUSCEPTIBLE;
		node -> next = NULL;
		node -> clusters = 0;
		node -> isolated = TRUE;
		if (last_node != NULL)
			last_node -> next = node;

		cl = network -> clusters;
		while (cl != NULL) {
			if (((float)rand() / RAND_MAX) < (cl -> prob))
				node -> clusters += pow(2, cl -> id);
			cl = cl -> next;
		}
	}

	edge -> node1 = 0;
	edge -> node2 = 0;
	network -> edges = edge;
	for (i = 1; i < size; i ++) {
		last_edge = edge;
		edge = malloc(sizeof(struct Edge));
		edge -> node1 = 0;
		edge -> node2 = 0;
		edge -> next = NULL;
		last_edge -> next = edge;
	}

	edge = network -> edges;
	for (i = 1; i <= size; i ++) {
		node1 = 1 + ((float)rand() / RAND_MAX) * (order - 0);
		node2 = 1 + ((float)rand() / RAND_MAX) * (order - 0);
		if ((node1 != node2)) {
			prob = con;
			if (!nodes_clustered (get_node(network, node1), get_node(network, node2)))
				prob *= clst_con;
			if (p_test (prob)) {
				edge -> node1 = node1;
				edge -> node2 = node2;
				get_node (network, node1) -> isolated = FALSE;
				get_node (network, node2) -> isolated = FALSE;
				edge = edge -> next;
			}
			else i --;
		}
		else i --;
	}
	return network;
}


void visualize_simulation (struct Network_sim* network_sim) {
	
	int count;
	int average;
	float p_sus;
	float p_inf;
	float p_rec;

	struct Network_state* state = network_sim -> timeline;
	int scr_width = network_sim -> time;
	int row;
	int col;

	printf ("\nSusceptible:\n");
	for (row = 9; row >= 0; row --) {
		for (col = 0; col < scr_width; col ++) {
			

			p_sus = (float)(state -> n_sus) / network_sim -> order;
			if (p_sus > row * 0.1 + 0.05)
				printf ("\e[0;32m█\e[0m");
			else
				printf ("\e[0;30m█\e[0m");

			state = state -> next;
		}
		state = network_sim -> timeline;
		putchar('\n');
	}

	printf ("\nInfected:\n");
	for (row = 9; row >= 0; row --) {
		for (col = 0; col < scr_width; col ++) {
			

			p_inf = (float)(state -> n_inf) / network_sim -> order;
			if (p_inf > row * 0.1 + 0.05)
				printf ("\e[0;31m█\e[0m");
			else
				printf ("\e[0;30m█\e[0m");

			state = state -> next;
		}
		state = network_sim -> timeline;
		putchar('\n');
	}

	printf ("\nRecovered:\n");
	for (row = 9; row >= 0; row --) {
		for (col = 0; col < scr_width; col ++) {
			

			p_rec = (float)(state -> n_rec) / network_sim -> order;
			if (p_rec > row * 0.1 + 0.05)
				printf ("\e[0;37m█\e[0m");
			else
				printf ("\e[0;30m█\e[0m");

			state = state -> next;
		}
		state = network_sim -> timeline;
		putchar('\n');
	}
}

void export_simulation (char* dir, struct Network_sim* sim) {
	FILE *fp = fopen (dir, "w+");
	struct Network_state* timeline = sim -> timeline;
	int t = 1;

	fprintf (fp, "simulation time: %d\nnetwork order: %d\n\n", sim -> time, sim -> order);

	while (timeline != NULL) {
		fprintf (fp, "t = %d:\n%d\n%d\n%d\n\n", t, timeline -> n_sus, timeline -> n_inf, timeline -> n_rec);

		timeline = timeline -> next;
		t ++;
	}

	fclose (fp);
}

struct Network_sim* simulate (
		_Network* network,
		int infected_0,
		float p_sus_inf,
		float p_inf_rec,
		float p_rec_inf,
		int time
	) {

	struct Node* node = network -> nodes;
	struct Edge* edge = network -> edges;
	int n_sus = network -> order - infected_0;
	int n_inf = infected_0;
	int n_rec = 0;
	struct Network_state* timeline = malloc (sizeof(struct Network_state));
	int i[1];
	struct Network_state* last_step = timeline;
	struct Node* node1;
	struct Node* node2;
	struct Network_sim* network_sim = malloc(sizeof(struct Network_sim));

	for (i[0] = 0; i[0] < infected_0; i[0]++) {
		node -> state = INFECTED;
		node = node -> next;
	}

	timeline -> n_sus = n_sus;
	timeline -> n_inf = n_inf;
	timeline -> n_rec = n_rec;
	printf ("\n\e[1;37mnodes(t = %d)\e[0m:\n", 1);
	print_nodes (network);

	for (i[0] = 1; i[0] < time; i[0] ++) {
		edge = network -> edges;
		node = network -> nodes;

		last_step -> next = malloc(sizeof (struct Network_state));
		last_step = last_step -> next;

		while (edge != NULL) {
			node1 = get_node (network, edge -> node1);
			node2 = get_node (network, edge -> node2);

			if (node1 -> state == INFECTED) {
				if (node2 -> state == SUSCEPTIBLE) {
			    	if ((node2 -> state = (p_test (p_sus_inf) ? INFECTED : SUSCEPTIBLE)) == INFECTED) {
						n_inf ++;
						n_sus --;
					}
				}
				else if (node2 -> state == RECOVERED) {
			    	if ((node2 -> state = (p_test (p_rec_inf) ? INFECTED : RECOVERED)) == INFECTED) {
						n_inf ++;
						n_rec --;
					}
				}
			}

			if (node2 -> state == INFECTED) {
				if (node1 -> state == SUSCEPTIBLE) {
			    	if ((node1 -> state = (p_test (p_sus_inf) ? INFECTED : SUSCEPTIBLE)) == INFECTED) {
						n_inf ++;
						n_sus --;
					}
				}
				else if (node1 -> state == RECOVERED) {
			    	if ((node1 -> state = (p_test (p_rec_inf) ? INFECTED : RECOVERED)) == INFECTED) {
						n_inf ++;
						n_rec --;
					}
				}
			}
			edge = edge -> next;
		}

		while (node != NULL) {
			if (node -> state == INFECTED) {
				if ((node -> state = (p_test (p_inf_rec) ? RECOVERED : INFECTED)) == RECOVERED) {
					n_inf --;
					n_rec ++;
				}
			}
			node = node -> next;
		}

		last_step -> n_sus = n_sus;
		last_step -> n_inf = n_inf;
		last_step -> n_rec = n_rec;

		printf ("\n\e[1;37mnodes(t = %d)\e[0m:\n", i[0] + 1);
		print_nodes (network);
	}

	network_sim = malloc(sizeof(struct Network_sim));
	network_sim -> timeline = timeline;
	network_sim -> time = time;
	network_sim -> order = network -> order;
	return network_sim;
}
