/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 12:17:56 by weiyang           #+#    #+#             */
/*   Updated: 2025/10/20 12:24:55 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	write_status_debug(t_philo_status status, t_philo *philo, long escaped)
{
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	escaped = gettime(MILLISECOND) - philo->table->start_simulation;
	if (status == TAKE_FIRST_FORK && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d has taken the 1° fork 🍴"
			"\t\t\tn°" B "[ 🍴 %d 🍴]\n" RST,
			escaped, philo->id + 1, philo->first_fork->fork_id + 1);
	if (status == TAKE_SECOND_FORK && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d has taken the 2° fork 🍴"
			"\t\t\tn°" B "[ 🍴 %d 🍴]\n" RST,
			escaped, philo->id + 1, philo->second_fork->fork_id + 1);
	if ((status == EATING) && !simulation_finished(philo->table))
		printf(W "%-6ld" C " %d is eating 🍝"
			"\t\t\t" Y "[🍝 %ld 🍝]\n" RST,
			escaped, philo->id + 1, philo->meals_counter);
	if ((status == SLEEPING) && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d is sleeping 😴\n", escaped, philo->id + 1);
	if ((status == THINKING) && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d is thinking🤔\n", escaped, philo->id + 1);
	if ((status == DIED) && !simulation_finished(philo->table))
		printf(RED "\t\t💀💀💀%-6ld %d is died\n" RST, escaped, philo->id + 1);
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}

void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	escaped;

	escaped = 0;
	if (philo->full)
		return ;
	if (debug)
		write_status_debug(status, philo, escaped);
	else
	{
		safe_mutex_handle(&philo->table->write_mutex, LOCK);
		escaped = gettime(MILLISECOND) - philo->table->start_simulation;
		if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
			&& !simulation_finished(philo->table))
			printf(W "%-6ld" RST " %d has taken a fork\n", escaped, philo->id + 1);
		if ((status == EATING) && !simulation_finished(philo->table))
			printf(W "%-6ld" C " %d is eating\n" RST, escaped, philo->id + 1);
		if ((status == SLEEPING) && !simulation_finished(philo->table))
			printf(W "%-6ld" RST " %d is sleeping\n", escaped, philo->id + 1);
		if ((status == THINKING) && !simulation_finished(philo->table))
			printf(W "%-6ld" RST " %d is thinking\n", escaped, philo->id + 1);
		if ((status == DIED) && !simulation_finished(philo->table))
			printf(RED "%-6ld" RST " %d is died\n" RST, escaped, philo->id + 1);
		safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
	}
}
