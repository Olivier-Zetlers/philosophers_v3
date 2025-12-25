
#include "philo.h"

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
