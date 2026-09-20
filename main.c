#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s number_of_philosophers time_to_die ", argv[0]);
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (1);
	}
	
	if (!parse_arguments(argc, argv, &table))
		return (1);
	
	if (!initialize_table(&table))
	{
		error_exit("Failed to initialize table", &table);
		return (1);
	}
	
	start_simulation(&table);
	cleanup_resources(&table);
	
	return (0);
}
