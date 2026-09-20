#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>

// ========== DEFINES ==========
# define FORK_TAKEN "has taken a fork"
# define EATING "is eating"
# define SLEEPING "is sleeping"
# define THINKING "is thinking"
# define DIED "died"

# define ERR_ARGS "Error: Invalid arguments\n"
# define ERR_MALLOC "Error: Memory allocation failed\n"
# define ERR_THREAD "Error: Thread creation failed\n"
# define ERR_MUTEX "Error: Mutex operation failed\n"

// ========== STRUCTURES ==========
typedef struct s_table t_table;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				id;
}	t_fork;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	
	t_fork			*left_fork;
	t_fork			*right_fork;
	
	long			last_meal_time;
	int				meals_eaten;
	bool			is_full;
	
	t_table			*table;
	pthread_mutex_t	philo_mutex;
}	t_philo;

struct s_table
{
	int				philo_count;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meal_goal;
	
	bool			simulation_end;
	bool			all_threads_ready;
	long			start_time;
	
	t_philo			*philos;
	t_fork			*forks;
	
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	table_mutex;
};

// ========== TIME FUNCTIONS ==========
long	get_time_ms(void);
long	get_time_us(void);
void	precise_sleep(long milliseconds, t_table *table);

// ========== INITIALIZATION ==========
bool	parse_arguments(int argc, char **argv, t_table *table);
bool	initialize_table(t_table *table);
bool	initialize_philosophers(t_table *table);
void	assign_forks(t_table *table);

// ========== THREAD & MUTEX SAFETY ==========
void	safe_mutex(pthread_mutex_t *mutex, int operation);
void	safe_thread(pthread_t *thread, void *(*start)(void *), 
					void *arg, int operation);
void	*ft_calloc(size_t count, size_t size);

// ========== SIMULATION ==========
void	start_simulation(t_table *table);
void	*philosopher_routine(void *arg);
void	*monitor_routine(void *arg);

// ========== ACTIONS ==========
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

// ========== STATE CHECKERS ==========
bool	is_simulation_over(t_table *table);
bool	philo_should_die(t_philo *philo);
bool	all_philos_full(t_table *table);

// ========== UTILS ==========
void	write_status(char *status, t_philo *philo);
void	cleanup_resources(t_table *table);
void	error_exit(char *message, t_table *table);

// ========== GETTERS/SETTERS (THREAD-SAFE) ==========
bool	get_bool(pthread_mutex_t *mutex, bool *value);
void	set_bool(pthread_mutex_t *mutex, bool *dest, bool value);
long	get_long(pthread_mutex_t *mutex, long *value);
void	set_long(pthread_mutex_t *mutex, long *dest, long value);

#endif
