#include "philo.h"

// ========== SIMULATION CONTROL ==========
void	start_simulation(t_table *table)
{
	pthread_t	monitor;
	int			i;
	
	// Special case: only one philosopher
	if (table->philo_count == 1)
	{
		printf("0 1 has taken a fork\n");
		usleep(table->time_to_die * 1000);
		printf("%ld 1 died\n", table->time_to_die);
		return ;
	}
	
	// CORREÇÃO: Definir start_time ANTES de threads
	table->start_time = get_time_ms();
	
	// Inicializar last_meal_time de cada filósofo
	for (i = 0; i < table->philo_count; i++)
	{
		set_long(&table->philos[i].philo_mutex, 
				&table->philos[i].last_meal_time, 
				table->start_time);
	}
	
	// Start philosopher threads
	for (i = 0; i < table->philo_count; i++)
	{
		safe_thread(&table->philos[i].thread, philosopher_routine,
					&table->philos[i], 0);
	}
	
	// Start monitor thread
	safe_thread(&monitor, monitor_routine, table, 0);
	
	// Signal threads to start
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	
	// Wait for monitor to finish
	safe_thread(&monitor, NULL, NULL, 1);
	
	// Wait for all philosophers to finish
	for (i = 0; i < table->philo_count; i++)
	{
		safe_thread(&table->philos[i].thread, NULL, NULL, 1);
	}
}

// ========== PHILOSOPHER ROUTINE ==========
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;
	
	philo = (t_philo *)arg;
	
	// Wait for all threads to be ready
	while (!get_bool(&philo->table->table_mutex, 
					&philo->table->all_threads_ready))
		usleep(100);
	
	// If odd ID, wait a bit to avoid deadlock
	if (philo->id % 2 == 1)
		usleep(philo->table->time_to_eat * 500);
	
	// Main routine loop
	while (!is_simulation_over(philo->table))
	{
		if (get_bool(&philo->philo_mutex, &philo->is_full))
			break ;
		
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	
	return (NULL);
}

// ========== MONITOR ROUTINE ==========
void	*monitor_routine(void *arg)
{
	t_table	*table;
	int		i;
	bool	all_full;
	
	table = (t_table *)arg;
	
	// Wait for simulation to start
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		usleep(100);
	
	// Monitor loop
	while (!get_bool(&table->table_mutex, &table->simulation_end))
	{
		usleep(1000); // Check every 1ms (within 10ms requirement)
		
		// Check each philosopher
		i = -1;
		all_full = true;
		while (++i < table->philo_count)
		{
			if (philo_should_die(&table->philos[i]))
			{
				write_status(DIED, &table->philos[i]);
				set_bool(&table->table_mutex, &table->simulation_end, true);
				return (NULL);
			}
			
			// Check if all are full (if meal goal specified)
			if (table->meal_goal != -1)
			{
				if (!get_bool(&table->philos[i].philo_mutex, 
							&table->philos[i].is_full))
					all_full = false;
			}
		}
		
		// If all philosophers have reached meal goal
		if (table->meal_goal != -1 && all_full)
		{
			set_bool(&table->table_mutex, &table->simulation_end, true);
			break ;
		}
	}
	
	return (NULL);
}

// ========== STATE CHECKERS ==========
bool	is_simulation_over(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->simulation_end));
}

bool	philo_should_die(t_philo *philo)
{
	long	current_time;
	long	last_meal;
	
	current_time = get_time_ms();
	last_meal = get_long(&philo->philo_mutex, &philo->last_meal_time);
	
	if (current_time - last_meal >= philo->table->time_to_die)
		return (true);
	
	return (false);
}

bool	all_philos_full(t_table *table)
{
	int	i;
	
	i = -1;
	while (++i < table->philo_count)
	{
		if (!get_bool(&table->philos[i].philo_mutex, &table->philos[i].is_full))
			return (false);
	}
	return (true);
}
