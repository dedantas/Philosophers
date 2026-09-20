#include "philo.h"

static long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result > INT_MAX)
			return (-1);
		str++;
	}
	return (result * sign);
}

bool	parse_arguments(int argc, char **argv, t_table *table)
{
	table->philo_count = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	
	if (argc == 6)
		table->meal_goal = ft_atol(argv[5]);
	else
		table->meal_goal = -1;
	
	// Validations
	if (table->philo_count <= 0 || table->philo_count > 200)
	{
		printf("Error: Philosophers count must be 1-200\n");
		return (false);
	}
	if (table->time_to_die <= 0 || table->time_to_eat <= 0 
		|| table->time_to_sleep <= 0)
	{
		printf("Error: Times must be positive integers\n");
		return (false);
	}
	if (argc == 6 && table->meal_goal <= 0)
	{
		printf("Error: Meal goal must be positive\n");
		return (false);
	}
	
	return (true);
}

bool	initialize_table(t_table *table)
{
	table->simulation_end = false;
	table->all_threads_ready = false;
	table->start_time = 0;
	
	// Allocate memory
	table->philos = ft_calloc(table->philo_count, sizeof(t_philo));
	table->forks = ft_calloc(table->philo_count, sizeof(t_fork));
	
	if (!table->philos || !table->forks)
	{
		error_exit(ERR_MALLOC, table);
		return (false);
	}
	
	// Initialize mutexes
	safe_mutex(&table->write_mutex, 0);
	safe_mutex(&table->table_mutex, 0);
	
	// Initialize forks
	for (int i = 0; i < table->philo_count; i++)
	{
		safe_mutex(&table->forks[i].mutex, 0);
		table->forks[i].id = i;
	}
	
	return (initialize_philosophers(table));
}

bool	initialize_philosophers(t_table *table)
{
	for (int i = 0; i < table->philo_count; i++)
	{
		table->philos[i].id = i + 1;
		table->philos[i].table = table;
		table->philos[i].last_meal_time = 0;
		table->philos[i].meals_eaten = 0;
		table->philos[i].is_full = false;
		
		// Initialize philosopher's personal mutex
		safe_mutex(&table->philos[i].philo_mutex, 0);
		
		// Assign forks
		table->philos[i].left_fork = &table->forks[i];
		if (table->philo_count == 1)
			table->philos[i].right_fork = NULL;
		else
			table->philos[i].right_fork = &table->forks[(i + 1) % table->philo_count];
	}
	
	return (true);
}
