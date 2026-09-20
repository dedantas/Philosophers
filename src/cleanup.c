#include "philo.h"

void	cleanup_resources(t_table *table)
{
	int	i;
	
	if (!table)
		return ;
	
	// Destroy philosopher mutexes
	if (table->philos)
	{
		for (i = 0; i < table->philo_count; i++)
		{
			safe_mutex(&table->philos[i].philo_mutex, 3);
		}
	}
	
	// Destroy fork mutexes
	if (table->forks)
	{
		for (i = 0; i < table->philo_count; i++)
		{
			safe_mutex(&table->forks[i].mutex, 3);
		}
	}
	
	// Destroy table mutexes
	safe_mutex(&table->write_mutex, 3);
	safe_mutex(&table->table_mutex, 3);
	
	// Free allocated memory
	if (table->philos)
		free(table->philos);
	if (table->forks)
		free(table->forks);
	
	// Zerar ponteiros
	table->philos = NULL;
	table->forks = NULL;
}

void	error_exit(char *message, t_table *table)
{
	printf("%s", message);
	if (table)
		cleanup_resources(table);
	exit(EXIT_FAILURE);
}
