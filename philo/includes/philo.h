#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>

typedef enum e_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
	DETACH,
}	t_opcode;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}	t_time_code;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DIED,
}	t_philo_status;

typedef pthread_mutex_t			t_mutex;
typedef struct s_table			t_table;

typedef struct s_fork
{
	t_mutex	mutex;
	int		fork_id;
}	t_fork;

typedef struct s_philo
{
	int			id;
	long		meal_count;
	bool		full;
	long		last_meal_time;
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;
	t_mutex		philo_mutex;
	t_table		*table;
}	t_philo;

struct	s_table
{
	long		philosopher_count;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		meal_limit;
	long		start_simulation;
	bool		end_simulation;
	bool		all_threads_ready;
	t_mutex		table_mutex;
	t_mutex		write_mutex;
	t_fork		*forks;
	t_philo		*philos;
	pthread_t	monitor;
	long		running_thread_count;
};

void	error_exit(const char *error);
void	parse_input(t_table *table, char **av);
void	table_init(t_table *table);
void	*safe_malloc(size_t bytes);
void	mutex_op(t_mutex *mutex, t_opcode opcode);
void	thread_op(pthread_t *thread, void *(*start_routine)(void *),
			void *data, t_opcode opcode);
void	set_bool(t_mutex *mutex, bool *dest, bool value);
bool	get_bool(t_mutex *mutex, bool *value);
void	set_long(t_mutex *mutex, long *dest, long value);
long	get_long(t_mutex *mutex, long *value);
bool	simulation_finished(t_table *table);
void	dinner_start(t_table *table);
void	wait_all_threads(t_table *table);
long	get_time(t_time_code time_code);
void	precise_usleep(long usec, t_table *table);
void	print_status(t_philo_status status, t_philo *philo);
bool	all_threads_running(t_mutex *mutex, long *thread_count,
			long philosopher_count);
void	increment_long(t_mutex *mutex, long *value);
void	*monitor_dinner(void *data);
void	table_cleanup(t_table *table);
void	philo_think(t_philo *philo, bool pre_simulation);
void	philo_eat(t_philo *philo);
void	stagger_start(t_philo *philo);
void	*philo_routine(void *data);
void	*single_philo_routine(void *arg);

#endif
