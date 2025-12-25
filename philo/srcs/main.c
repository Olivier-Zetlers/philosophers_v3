#include "philo.h"

static void	spawn_philo_threads(t_table *table);

void	dinner_start(t_table *table)
{
	int	i;

	i = 0;
	if (table->meal_limit == 0)
		return ;
	if (table->philosopher_count == 1)
		thread_op(&table->philos[0].thread_id,
			single_philo_routine, &table->philos[0], CREATE);
	else
		spawn_philo_threads(table);
	table->start_simulation = get_time(MILLISECOND);
	set_bool(&table->table_mutex, &table->all_threads_ready, true);
	thread_op(&table->monitor, monitor_dinner, table, CREATE);
	i = 0;
	while (i < table->philosopher_count)
	{
		thread_op(&table->philos[i].thread_id, NULL, NULL, JOIN);
		i++;
	}
	thread_op(&table->monitor, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
}

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		parse_input(&table, av);
		table_init(&table);
		dinner_start(&table);
		table_cleanup(&table);
	}
	else
	{
		error_exit("usage: ./philo num_philos time_to_die time_to_eat "
			"time_to_sleep [meals]");
	}
	return (0);
}

static void	spawn_philo_threads(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philosopher_count)
	{
		thread_op(&table->philos[i].thread_id,
			philo_routine, &table->philos[i], CREATE);
		i++;
	}
}
