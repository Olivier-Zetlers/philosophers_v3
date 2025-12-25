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
