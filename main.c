#include "pandemic_sim.h"

int main () {
	int clst_count;
	int clst_dist;
	float con;
	float clst_con;
	int order;
	int size;
	struct Network* network;
	int time;
	int infected_0;
	float p_sus_inf;
	float p_inf_rec;
	float p_rec_inf;
	struct Network_sim* sim;
	char filename[32];


	printf ("\n~~~ Social Network Generator ~~~\n");

	printf ("cluster count(0, oo):");
	scanf ("%d", &clst_count);
	printf ("clst_count = %d\n", clst_count);

	printf ("cluster size distribution function{0 = CONST, 1 = LINEAR}:");
	scanf ("%d", &clst_dist);
	printf ("clst_dist = %d\n", clst_dist);

	printf ("connectedness factor[0.0, 1.0]:");
	scanf ("%f", &con);
	printf ("con = %f\n", con);

	printf ("cluster connectedness factor[0.0, 1.0]:");
	scanf ("%f", &clst_con);
	printf ("clst_con = %f\n", clst_con);

	printf ("network order(0, oo):");
	scanf ("%d", &order);
	printf ("order = %d\n", order);

	printf ("network size(0, oo):");
	scanf ("%d", &size);
	printf ("size = %d\n", size);

	printf ("\n~~~ Pandemic Simulator ~~~\n");

	printf ("simulation duration(0, oo):");
	scanf ("%d", &time);
	printf ("time = %d\n", time);

	printf ("initial infected count: (0, network order):");
	scanf ("%d", &infected_0);
	printf ("infected_0 = %d\n", time);

	printf ("marginal chance of infection of susceptible ones: (0.0, 1.0):");
	scanf ("%f", &p_sus_inf);
	printf ("p_sus_inf = %f\n", p_sus_inf);

	printf ("marginal chance of recovery of infected ones: (0.0, 1.0):");
	scanf ("%f", &p_inf_rec);
	printf ("p_inf_rec = %f\n", p_inf_rec);

	printf ("marginal chance of infection of recovered ones: (0.0, 1.0):");
	scanf ("%f", &p_rec_inf);
	printf ("p_rec_inf = %f\n", p_rec_inf);

	network = generate_network (
		clst_count,
		clst_dist,
		con,
		clst_con,
		order,
		size
	);
	sim = simulate (network, infected_0, p_sus_inf, p_inf_rec, p_rec_inf, time);

	printf ("\n~~~ Simulation Visualization ~~~\n");
	visualize_simulation (sim);

	printf ("data export filename:");
	scanf ("%s", filename);
	printf ("filename = %s\n", filename);
	export_simulation (filename, sim);

	return 0;
}
