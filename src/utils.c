#include "philo.h"

// ========== TIME FUNCTIONS ==========
long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000000) + tv.tv_usec);
}

void	precise_sleep(long milliseconds, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time_ms();
	while (get_time_ms() - start < milliseconds)
	{
		if (is_simulation_over(table))
			break ;
		
		elapsed = get_time_ms() - start;
		remaining = milliseconds - elapsed;
		
		if (remaining > 10)
			usleep(remaining * 600); // 0.5ms
		else if (remaining > 1)
			usleep(remaining * 1000); // 0.1ms
		else
			usleep(1000); // 0.1ms mínimo
	}
}

// ========== MUTEX/THREAD SAFETY ==========
void	safe_mutex(pthread_mutex_t *mutex, int operation)
{
	if (operation == 0) // INIT
	{
		if (pthread_mutex_init(mutex, NULL) != 0)
			error_exit(ERR_MUTEX, NULL);
	}
	else if (operation == 1) // LOCK
	{
		if (pthread_mutex_lock(mutex) != 0)
			error_exit(ERR_MUTEX, NULL);
	}
	else if (operation == 2) // UNLOCK
	{
		if (pthread_mutex_unlock(mutex) != 0)
			error_exit(ERR_MUTEX, NULL);
	}
	else if (operation == 3) // DESTROY
	{
		if (pthread_mutex_destroy(mutex) != 0)
			error_exit(ERR_MUTEX, NULL);
	}
}

void	safe_thread(pthread_t *thread, void *(*start)(void *), 
					void *arg, int operation)
{
	if (operation == 0) // CREATE
	{
		if (pthread_create(thread, NULL, start, arg) != 0)
			error_exit(ERR_THREAD, NULL);
	}
	else if (operation == 1) // JOIN
	{
		if (pthread_join(*thread, NULL) != 0)
			error_exit(ERR_THREAD, NULL);
	}
	else if (operation == 2) // DETACH
	{
		if (pthread_detach(*thread) != 0)
			error_exit(ERR_THREAD, NULL);
	}
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	total;

	if (count == 0 || size == 0)
		return (NULL);
	total = count * size;
	if (total / count != size)
		return (NULL);
	ptr = malloc(total);
	if (!ptr)
		return (NULL);
	memset(ptr, 0, total);
	return (ptr);
}

// ========== GETTERS/SETTERS ==========
bool	get_bool(pthread_mutex_t *mutex, bool *value)
{
	bool	ret;

	safe_mutex(mutex, 1);
	ret = *value;
	safe_mutex(mutex, 2);
	return (ret);
}

void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value)
{
	safe_mutex(mutex, 1);
	*dest = value;
	safe_mutex(mutex, 2);
}

long	get_long(pthread_mutex_t *mutex, long *value)
{
	long	ret;

	safe_mutex(mutex, 1);
	ret = *value;
	safe_mutex(mutex, 2);
	return (ret);
}

void	set_long(pthread_mutex_t *mutex, long *dest, long value)
{
	safe_mutex(mutex, 1);
	*dest = value;
	safe_mutex(mutex, 2);
}

// ========== WRITE STATUS ==========
void	write_status(char *status, t_philo *philo)
{
	long	timestamp;

	if (is_simulation_over(philo->table))
		return ;
	if (get_bool(&philo->table->table_mutex, &philo->is_full))
		return ;
	
	safe_mutex(&philo->table->write_mutex, 1);
	
	if (!is_simulation_over(philo->table))
	{
		timestamp = get_time_ms() - philo->table->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	
	safe_mutex(&philo->table->write_mutex, 2);
}
