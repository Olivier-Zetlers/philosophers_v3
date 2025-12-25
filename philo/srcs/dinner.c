/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 10:25:52 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/22 11:20:12 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
