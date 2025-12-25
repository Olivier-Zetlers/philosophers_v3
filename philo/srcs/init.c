/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 11:01:46 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/20 11:05:46 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int philo_position)
{
	int	philo_nbr;

	philo_nbr = philo->table->philo_nbr;
	if (philo_position % 2 != 0)
	{
		philo->first_fork = &forks[philo_position];
		philo->second_fork = &forks[(philo_position + 1) % philo_nbr];
	}
	else
	{
		philo->first_fork = &forks[(philo_position + 1) % philo_nbr];
		philo->second_fork = &forks[philo_position];
	}
}

static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i;
		philo->full = false;
		philo->meals_counter = 0;
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
	table->thread_running_nbr = 0;
	table->philos = safe_malloc(table->philo_nbr * sizeof(t_philo));
	table->forks = safe_malloc(table->philo_nbr * sizeof(t_fork));
	safe_mutex_handle(&table->table_mutex, INIT);
	safe_mutex_handle(&table->write_mutex, INIT);
	while (i < table->philo_nbr)
	{
		safe_mutex_handle(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i;
		i++;
	}
	philo_init(table);
}
