/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 12:31:42 by weiyang           #+#    #+#             */
/*   Updated: 2025/12/25 00:00:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** =============================================================================
** SECTION 1: Error Handling
** =============================================================================
*/

void	error_exit(const char *error)
{
	printf(RED "%s\n" RST, error);
	exit(EXIT_FAILURE);
}

/*
** =============================================================================
** SECTION 2: Safe Wrappers (malloc, mutex, thread)
** =============================================================================
*/

void	*safe_malloc(size_t bytes)
{
	void	*ret;

	ret = malloc(bytes);
	if (!ret)
		error_exit("Error with the malloc!");
	return (ret);
}

static void	handle_mutex_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	if (status == EINVAL && (opcode == LOCK || opcode == UNLOCK))
		error_exit("The value specified by mutex is invalid");
	else if (status == EINVAL && opcode == INIT)
		error_exit("The value specified by attr is invalid");
	else if (status == EDEADLK)
		error_exit("A deadlock would occurr");
	else if (status == EPERM)
		error_exit("The current thread does not hold a lock en mutex");
	else if (status == ENOMEM)
		error_exit(" The process cannot allocate enough memory");
	else if (status == EBUSY)
		error_exit("Mutex is locked");
}

void	safe_mutex_handle(t_mutex *mutex, t_opcode opcode)
{
	if (opcode == LOCK)
		handle_mutex_error(pthread_mutex_lock(mutex), opcode);
	else if (opcode == UNLOCK)
		handle_mutex_error(pthread_mutex_unlock(mutex), opcode);
	else if (opcode == INIT)
		handle_mutex_error(pthread_mutex_init(mutex, NULL), opcode);
	else if (opcode == DESTROY)
		handle_mutex_error(pthread_mutex_destroy(mutex), opcode);
	else
		error_exit("Opcode for mutex handle is incorrect");
}

static void	handle_thread_error(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	else if (status == EAGAIN)
		error_exit("No resources to create another thread\n");
	else if (status == EPERM)
		error_exit("The caller does not have appropriate permission\n");
	else if (status == EINVAL && opcode == CREATE)
		error_exit("The value specifie par the attr is not valid\n");
	else if (status == EINVAL && (opcode == JOIN || opcode == DETACH))
		error_exit("THe value specified by thread is not valid\n");
	else
		error_exit("Opcode is incorrect");
}

void	safe_thread_handle(pthread_t *thread, void *(*start_routine)(void *),
		void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		handle_thread_error(pthread_create(thread, NULL,
				start_routine, data), opcode);
	else if (opcode == JOIN)
		handle_thread_error(pthread_join(*thread, NULL), opcode);
	else if (opcode == DETACH)
		handle_thread_error(pthread_detach(*thread), opcode);
	else
		error_exit("Wrong opcode for thread handle: Use CREATE, JOIN, DETACH");
}

/*
** =============================================================================
** SECTION 3: Getter/Setter Functions (Thread-safe accessors)
** =============================================================================
*/

void	set_bool(t_mutex *mutex, bool *dest, bool value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

bool	get_bool(t_mutex *mutex, bool *value)
{
	bool	ret;

	safe_mutex_handle(mutex, LOCK);
	ret = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

void	set_long(t_mutex *mutex, long *dest, long value)
{
	safe_mutex_handle(mutex, LOCK);
	*dest = value;
	safe_mutex_handle(mutex, UNLOCK);
}

long	get_long(t_mutex *mutex, long *value)
{
	long	ret;

	safe_mutex_handle(mutex, LOCK);
	ret = *value;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}

/*
** =============================================================================
** SECTION 4: Synchronization Utilities
** =============================================================================
*/

void	wait_all_threads(t_table *table)
{
	while (!get_bool(&table->table_mutex, &table->all_threads_ready))
		;
}

bool	all_threads_running(t_mutex *mutex, long *thread_count,
		long philosopher_count)
{
	bool	ret;

	ret = false;
	safe_mutex_handle(mutex, LOCK);
	if (*thread_count == philosopher_count)
		ret = true;
	safe_mutex_handle(mutex, UNLOCK);
	return (ret);
}

void	increase_long(t_mutex *mutex, long *value)
{
	safe_mutex_handle(mutex, LOCK);
	(*value)++;
	safe_mutex_handle(mutex, UNLOCK);
}

/*
** =============================================================================
** SECTION 5: Time Utilities
** =============================================================================
*/

long	gettime(t_time_code time_code)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("Gettingtimeofday failed");
	if (time_code == SECOND)
		return (tv.tv_sec + (tv.tv_usec / 1e6));
	else if (time_code == MILLISECOND)
		return (tv.tv_sec * 1e3 + (tv.tv_usec / 1e3));
	else if (time_code == MICROSECOND)
		return (tv.tv_sec * 1e6 + tv.tv_usec);
	else
		error_exit("Wrong input for gettime");
	return (1337);
}

void	precise_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = gettime(MICROSECOND);
	while (gettime(MICROSECOND) - start < usec)
	{
		if (simulation_finished(table))
			break ;
		elapsed = gettime(MICROSECOND) - start;
		remaining = usec - elapsed;
		if (remaining > 1e3)
			usleep(remaining / 2);
		else
		{
			while (gettime(MICROSECOND) - start < usec)
				;
		}
	}
}

/*
** =============================================================================
** SECTION 6: Output Functions
** =============================================================================
*/

void	write_status(t_philo_status status, t_philo *philo)
{
	long	elapsed;

	if (philo->full)
		return ;
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	elapsed = gettime(MILLISECOND) - philo->table->start_simulation;
	if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
		&& !simulation_finished(philo->table))
		printf("%ld %d has taken a fork\n", elapsed, philo->id + 1);
	if ((status == EATING) && !simulation_finished(philo->table))
		printf("%ld %d is eating\n", elapsed, philo->id + 1);
	if ((status == SLEEPING) && !simulation_finished(philo->table))
		printf("%ld %d is sleeping\n", elapsed, philo->id + 1);
	if ((status == THINKING) && !simulation_finished(philo->table))
		printf("%ld %d is thinking\n", elapsed, philo->id + 1);
	if ((status == DIED) && !simulation_finished(philo->table))
		printf("%ld %d died\n", elapsed, philo->id + 1);
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}

/*
** =============================================================================
** SECTION 7: Parsing Functions
** =============================================================================
*/

static bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

static bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static const char	*validate_input(const char *str)
{
	const char	*start;
	size_t		len;

	len = 0;
	while (is_space(*str))
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-')
		error_exit("Give me only positive values");
	if (!is_digit(*str))
		error_exit("Not valid input: expected digit");
	start = str;
	while (is_digit(*str))
	{
		len++;
		str++;
	}
	if (len > 10)
		error_exit("The value is too big. INT_MAX is the limit");
	while (is_space(*str))
		str++;
	if (*str != '\0')
		error_exit("Invalid characters after number");
	return (start);
}

static long	ft_atoi(const char *str)
{
	long	num;
	int		digit;

	num = 0;
	str = validate_input(str);
	while (is_digit(*str))
	{
		digit = *str - '0';
		if (num > (INT_MAX - digit) / 10)
			error_exit("The value is too big, the limit is INT_MAX");
		num = num * 10 + digit;
		str++;
	}
	return (num);
}

void	parse_input(t_table *table, char **av)
{
	long	min_time_us;

	min_time_us = 60000;
	table->philosopher_count = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]) * 1000;
	table->time_to_eat = ft_atoi(av[3]) * 1000;
	table->time_to_sleep = ft_atoi(av[4]) * 1000;
	if (table->time_to_die < min_time_us || table->time_to_eat < min_time_us
		|| table->time_to_sleep < min_time_us)
		error_exit("Use timestamps greater than 60ms");
	if (av[5])
		table->meal_limit = ft_atoi(av[5]);
	else
		table->meal_limit = -1;
}

/*
** =============================================================================
** SECTION 8: Initialization Functions
** =============================================================================
*/

static void	assign_forks(t_philo *philo, t_fork *forks, int philo_position)
{
	int	philosopher_count;

	philosopher_count = philo->table->philosopher_count;
	if (philo_position % 2 != 0)
	{
		philo->first_fork = &forks[philo_position];
		philo->second_fork = &forks[(philo_position + 1) % philosopher_count];
	}
	else
	{
		philo->first_fork = &forks[(philo_position + 1) % philosopher_count];
		philo->second_fork = &forks[philo_position];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->philosopher_count)
	{
		philo = table->philos + i;
		philo->id = i;
		philo->full = false;
		philo->meal_count = 0;
		philo->table = table;
		safe_mutex_handle(&philo->philo_mutex, INIT);
		assign_forks(philo, table->forks, i);
		i++;
	}
}

void	data_init(t_table *table)
{
	int	i;

	i = 0;
	table->end_simulation = false;
	table->all_threads_ready = false;
	table->running_thread_count = 0;
	table->philos = safe_malloc(table->philosopher_count * sizeof(t_philo));
	table->forks = safe_malloc(table->philosopher_count * sizeof(t_fork));
	safe_mutex_handle(&table->table_mutex, INIT);
	safe_mutex_handle(&table->write_mutex, INIT);
	while (i < table->philosopher_count)
	{
		safe_mutex_handle(&table->forks[i].mutex, INIT);
		table->forks[i].fork_id = i;
		i++;
	}
	philo_init(table);
}

/*
** =============================================================================
** SECTION 9: Philosopher Actions
** =============================================================================
*/

void	think(t_philo *philo, bool pre_simulation)
{
	long	time_to_eat;
	long	time_to_sleep;
	long	think_duration;

	if (!pre_simulation)
		write_status(THINKING, philo);
	if (philo->table->philosopher_count % 2 == 0)
		return ;
	time_to_eat = philo->table->time_to_eat;
	time_to_sleep = philo->table->time_to_sleep;
	think_duration = time_to_eat * 2 - time_to_sleep;
	if (think_duration < 0)
		think_duration = 0;
	precise_usleep(think_duration * 0.42, philo->table);
}

void	eat(t_philo *philo)
{
	safe_mutex_handle(&philo->first_fork->mutex, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	safe_mutex_handle(&philo->second_fork->mutex, LOCK);
	write_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	safe_mutex_handle(&philo->philo_mutex, LOCK);
	philo->meal_count++;
	safe_mutex_handle(&philo->philo_mutex, UNLOCK);
	write_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meal_limit > 0
		&& philo->meal_count == philo->table->meal_limit)
		set_bool(&philo->philo_mutex, &philo->full, true);
	safe_mutex_handle(&philo->first_fork->mutex, UNLOCK);
	safe_mutex_handle(&philo->second_fork->mutex, UNLOCK);
}

void	desynchronise(t_philo *philo)
{
	if (philo->table->philosopher_count % 2 == 0)
	{
		if (philo->id % 2 == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			think(philo, true);
	}
}

/*
** =============================================================================
** SECTION 10: Monitor Functions
** =============================================================================
*/

static bool	philo_died(t_philo *philo)
{
	long	elapsed;
	long	time_to_die_ms;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	pthread_mutex_lock(&philo->philo_mutex);
	elapsed = gettime(MILLISECOND) - (philo->last_meal_time);
	pthread_mutex_unlock(&philo->philo_mutex);
	time_to_die_ms = (philo->table->time_to_die) / 1000;
	if (elapsed > time_to_die_ms)
		return (true);
	return (false);
}

static void	philo_die(t_table *table, t_philo *philo)
{
	write_status(DIED, philo);
	set_bool(&table->table_mutex, &table->end_simulation, true);
}

void	*monitor_dinner(void *data)
{
	int		i;
	bool	all_full;
	t_table	*table;

	table = (t_table *)data;
	while (!all_threads_running(&table->table_mutex,
			&table->running_thread_count, table->philosopher_count))
		;
	while (!simulation_finished(table))
	{
		all_full = true;
		i = 0;
		while (i < table->philosopher_count)
		{
			if (philo_died(&table->philos[i]))
				philo_die(table, &table->philos[i]);
			if (!get_bool(&table->philos[i].philo_mutex,
					&table->philos[i].full))
				all_full = false;
			i++;
		}
		if (table->meal_limit > 0 && all_full)
			set_bool(&table->table_mutex, &table->end_simulation, true);
	}
	return (NULL);
}

/*
** =============================================================================
** SECTION 11: Dinner Simulation Functions
** =============================================================================
*/

void	*dinner_simulation(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time, gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->running_thread_count);
	desynchronise(philo);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		eat(philo);
		write_status(SLEEPING, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		think(philo, false);
	}
	return (NULL);
}

void	*lone_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		gettime(MILLISECOND));
	increase_long(&philo->table->table_mutex,
		&philo->table->running_thread_count);
	safe_mutex_handle(&philo->first_fork->mutex, LOCK);
	write_status(TAKE_FIRST_FORK, philo);
	precise_usleep(philo->table->time_to_die, philo->table);
	safe_mutex_handle(&philo->first_fork->mutex, UNLOCK);
	return (NULL);
}

static void	multi_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philosopher_count)
	{
		safe_thread_handle(&table->philos[i].thread_id,
			dinner_simulation, &table->philos[i], CREATE);
		i++;
	}
}

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;
	if (table->meal_limit == 0)
		return ;
	if (table->philosopher_count == 1)
		safe_thread_handle(&table->philos[0].thread_id,
			lone_philo, &table->philos[0], CREATE);
	else
		multi_threads(table);
	table->start_simulation = gettime(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	safe_thread_handle(&table->monitor, monitor_dinner, table, CREATE);
	i = 0;
	while (i < table->philosopher_count)
	{
		safe_thread_handle(&table->philos[i].thread_id, NULL, NULL, JOIN);
		i++;
	}
	safe_thread_handle(&table->monitor, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
}

/*
** =============================================================================
** SECTION 12: Cleanup Functions
** =============================================================================
*/

void	clean(t_table *table)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < table->philosopher_count)
	{
		philo = &table->philos[i];
		safe_mutex_handle(&philo->philo_mutex, DESTROY);
		i++;
	}
	safe_mutex_handle(&table->table_mutex, DESTROY);
	safe_mutex_handle(&table->write_mutex, DESTROY);
	free(table->forks);
	free(table->philos);
}

/*
** =============================================================================
** SECTION 13: Main Entry Point
** =============================================================================
*/

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		parse_input(&table, av);
		data_init(&table);
		dinner_start(&table);
		clean(&table);
	}
	else
	{
		error_exit("Wrong input\n" GREEN
			"Correct is ./philo 5 800 200 200 [5]" RST);
	}
	return (0);
}
