
#include "philo.h"

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

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		parse_input(&table, av);
		data_init(&table);
		dinner_start(&table);
		clean(&table);
	}
	else
	{
		error_exit("Wrong input\n" 
			"Correct is ./philo 5 800 200 200 [5]" );
	}
	return (0);
}
