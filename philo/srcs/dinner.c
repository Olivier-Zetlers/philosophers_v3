#include "philo.h"

void	stagger_start(t_philo *philo);

void	philo_eat(t_philo *philo)
{
	mutex_op(&philo->first_fork->mutex, LOCK);
	print_status(TAKE_FIRST_FORK, philo);
	mutex_op(&philo->second_fork->mutex, LOCK);
	print_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	mutex_op(&philo->philo_mutex, LOCK);
	philo->meal_count++;
	mutex_op(&philo->philo_mutex, UNLOCK);
	print_status(EATING, philo);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meal_limit > 0
		&& philo->meal_count == philo->table->meal_limit)
		set_bool(&philo->philo_mutex, &philo->full, true);
	mutex_op(&philo->first_fork->mutex, UNLOCK);
	mutex_op(&philo->second_fork->mutex, UNLOCK);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECOND));
	increment_long(&philo->table->table_mutex,
		&philo->table->running_thread_count);
	stagger_start(philo);
	while (!simulation_finished(philo->table))
	{
		if (philo->full)
			break ;
		philo_eat(philo);
		print_status(SLEEPING, philo);
		precise_usleep(philo->table->time_to_sleep, philo->table);
		philo_think(philo, false);
	}
	return (NULL);
}

void	*single_philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->table);
	set_long(&philo->philo_mutex, &philo->last_meal_time,
		get_time(MILLISECOND));
	increment_long(&philo->table->table_mutex,
		&philo->table->running_thread_count);
	mutex_op(&philo->first_fork->mutex, LOCK);
	print_status(TAKE_FIRST_FORK, philo);
	precise_usleep(philo->table->time_to_die, philo->table);
	mutex_op(&philo->first_fork->mutex, UNLOCK);
	return (NULL);
}

void	stagger_start(t_philo *philo)
{
	if (philo->table->philosopher_count % 2 == 0)
	{
		if (philo->id % 2 == 0)
			precise_usleep(3e4, philo->table);
	}
	else
	{
		if (philo->id % 2)
			philo_think(philo, true);
	}
}
