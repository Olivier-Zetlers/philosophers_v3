/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:35:49 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/20 12:05:21 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	error_exit(const char *error)
{
	printf(RED "%s\n" RST, error);
	exit(EXIT_FAILURE);
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
