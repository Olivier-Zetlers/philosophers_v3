/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weiyang <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 12:17:56 by weiyang           #+#    #+#             */
/*   Updated: 2025/12/25 00:00:00 by weiyang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Debug output function - verbose mode with additional information.
** This function is only called when DEBUG_MODE is set to 1.
** Note: The debug format does NOT conform to the project specifications.
** It should only be used during development/debugging.
*/
void	write_status_debug(t_philo_status status, t_philo *philo, long escaped)
{
	safe_mutex_handle(&philo->table->write_mutex, LOCK);
	escaped = gettime(MILLISECOND) - philo->table->start_simulation;
	if (status == TAKE_FIRST_FORK && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d has taken the 1st fork "
			"[fork #%d]\n" RST,
			escaped, philo->id + 1, philo->first_fork->fork_id + 1);
	if (status == TAKE_SECOND_FORK && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d has taken the 2nd fork "
			"[fork #%d]\n" RST,
			escaped, philo->id + 1, philo->second_fork->fork_id + 1);
	if ((status == EATING) && !simulation_finished(philo->table))
		printf(W "%-6ld" C " %d is eating "
			"[meal #%ld]\n" RST,
			escaped, philo->id + 1, philo->meals_counter);
	if ((status == SLEEPING) && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d is sleeping\n", escaped, philo->id + 1);
	if ((status == THINKING) && !simulation_finished(philo->table))
		printf(W "%-6ld" RST " %d is thinking\n", escaped, philo->id + 1);
	if ((status == DIED) && !simulation_finished(philo->table))
		printf(RED "%-6ld %d died\n" RST, escaped, philo->id + 1);
	safe_mutex_handle(&philo->table->write_mutex, UNLOCK);
}

/*
** Standard output function - conforms to project specifications.
** Output format: "timestamp_in_ms X <action>"
** Where:
**   - timestamp_in_ms: milliseconds since simulation start
**   - X: philosopher number (1-indexed)
**   - action: one of "has taken a fork", "is eating", "is sleeping",
**             "is thinking", or "died"
*/
void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	elapsed;

	elapsed = 0;
	if (philo->full)
		return ;
	if (debug)
		write_status_debug(status, philo, elapsed);
	else
	{
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
}
