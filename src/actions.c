#include "philo.h"

static void	take_forks_in_order(t_philo *philo, t_fork **first, t_fork **second)
{
	// Sempre pegar o garfo com menor ID primeiro
	if (philo->left_fork->id < philo->right_fork->id)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

void	philo_eat(t_philo *philo)
{
	t_fork	*first_fork;
	t_fork	*second_fork;
	
	// Caso especial: apenas 1 filósofo
	if (!philo->right_fork)
	{
		safe_mutex(&philo->left_fork->mutex, 1);
		write_status(FORK_TAKEN, philo);
		precise_sleep(philo->table->time_to_die, philo->table);
		safe_mutex(&philo->left_fork->mutex, 2);
		return ;
	}
	
	// Determinar ordem para evitar deadlock
	take_forks_in_order(philo, &first_fork, &second_fork);
	
	// Take first fork
	safe_mutex(&first_fork->mutex, 1);
	write_status(FORK_TAKEN, philo);
	
	// Take second fork
	safe_mutex(&second_fork->mutex, 1);
	write_status(FORK_TAKEN, philo);
	
	// Eating
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time_ms());
	write_status(EATING, philo);
	precise_sleep(philo->table->time_to_eat, philo->table);
	
	// Update meal count
	philo->meals_eaten++;
	if (philo->table->meal_goal != -1 && 
		philo->meals_eaten >= philo->table->meal_goal)
	{
		set_bool(&philo->philo_mutex, &philo->is_full, true);
	}
	
	// Release forks (ordem reversa não é necessária para deadlock)
	safe_mutex(&second_fork->mutex, 2);
	safe_mutex(&first_fork->mutex, 2);
}

void	philo_sleep(t_philo *philo)
{
	if (is_simulation_over(philo->table))
		return ;
	
	write_status(SLEEPING, philo);
	precise_sleep(philo->table->time_to_sleep, philo->table);
}

void	philo_think(t_philo *philo)
{
	if (is_simulation_over(philo->table))
		return ;
	
	write_status(THINKING, philo);
	precise_sleep(1, philo->table);
}
