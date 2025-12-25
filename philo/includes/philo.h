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

#define SECOND		0
#define MILLISECOND	1
#define MICROSECOND	2

typedef int				t_timecode;

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
	t_fork		*left_fork;
	t_fork		*right_fork;
	pthread_t	thread_id;
	t_mutex		philo_mutex;
	t_table		*table;
}	t_philo;

typedef struct s_table
{
	bool		all_threads_ready;
	bool		end_simulation;
	t_fork		*forks;
	long		meal_limit;
	pthread_t	monitor;
	long		philosopher_count;
	t_philo		*philos;
	long		running_thread_count;
	long		start_simulation;
	t_mutex		table_mutex;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	t_mutex		write_mutex;
}	t_table;

bool	all_threads_running(t_mutex *mutex, long *thread_count,
			long philosopher_count);
void	dinner_start(t_table *table);
void	error_exit(const char *error);
bool	get_bool(t_mutex *mutex, bool *value);
long	get_long(t_mutex *mutex, long *value);
long	get_time(t_timecode timecode);
void	increment_long(t_mutex *mutex, long *value);
void	*monitor_dinner(void *data);
void	mutex_op(t_mutex *mutex, t_opcode opcode);
void	parse_input(t_table *table, char **av);
void	philo_eat(t_philo *philo);
void	*philo_routine(void *data);
void	philo_think(t_philo *philo, bool pre_simulation);
void	precise_usleep(long usec, t_table *table);
void	print_status(t_philo_status status, t_philo *philo);
void	*safe_malloc(size_t bytes);
void	set_bool(t_mutex *mutex, bool *dest, bool value);
void	set_long(t_mutex *mutex, long *dest, long value);
bool	simulation_finished(t_table *table);
void	*single_philo_routine(void *arg);
void	stagger_start(t_philo *philo);
void	table_cleanup(t_table *table);
void	table_init(t_table *table);
void	thread_op(pthread_t *thread, void *(*start_routine)(void *),
			void *data, t_opcode opcode);
void	wait_all_threads(t_table *table);

#endif
