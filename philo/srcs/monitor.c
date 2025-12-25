/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:13:28 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/20 11:34:23 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	philo_died(t_philo *philo)
{
	long	escaped;
	long	t_to_die;

	if (get_bool(&philo->philo_mutex, &philo->full))
		return (false);
	pthread_mutex_lock(&philo->philo_mutex);
	escaped = gettime(MILLISECOND) - (philo->last_meal_time);
	pthread_mutex_unlock(&philo->philo_mutex);
	t_to_die = (philo->table->time_to_die) / 1000;
	if (escaped > t_to_die)
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
			&table->thread_running_nbr, table->philo_nbr))
		;
	while (!simulation_finished(table))
	{
		all_full = true;
		i = 0;
		while (i < table->philo_nbr)
		{
			if (philo_died(&table->philos[i]))
				philo_die(table, &table->philos[i]);
			if (!get_bool(&table->philos[i].philo_mutex,
					&table->philos[i].full))
				all_full = false;
			i++;
		}
		if (table->nbr_limit_meals > 0 && all_full)
			set_bool(&table->table_mutex, &table->end_simulation, true);
	}
	return (NULL);
}
