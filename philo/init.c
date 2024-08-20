/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 23:11:19 by saleshin          #+#    #+#             */
/*   Updated: 2024/07/14 23:11:32 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_args(int argc, char **argv, t_args *args)
{
	if (argc != 5 && argc != 6)
	{
		printf("Wrong # of args! Need:\n"
			"number_of_philosophers\n"
			"time_to_die\n"
			"time_to_eat\n"
			"time_to_sleep\n"
			"<number_of_times_each_philosopher_must_eat>\n");
		return (1);
	}
	if (check_args(argc, argv))
		return (1);
	args->number_of_philosophers = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	args->start_time = 0;
	args->number_of_times_each_philosopher_must_eat = 0;
	args->end = 0;
	args->philos_finished = 0;
	pthread_mutex_init(&args->mutex_global, NULL);
	if (argc == 6)
		args->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
	return (0);
}

int	init_philo(t_philo *philosophers, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		philosophers[i].id = i + 1;
		philosophers[i].status = THINKING;
		philosophers[i].args = args;
		philosophers[i].eat_count = 0;
		pthread_mutex_init(&philosophers[i].fork, NULL);
		pthread_mutex_init(&philosophers[i].timer_mutex, NULL);
		i++;
	}
	return (0);
}

int	init_threads(t_philo *philosophers, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_philosophers)
	{
		if (pthread_create(&philosophers[i].thread_philo,
				NULL, philo_routine, &philosophers[i]))
			return (i);
		i++;
	}
	return (-1);
}

int	one_philo(t_args *args)
{
	if (args->number_of_philosophers == 1)
	{
		printf("%d 1 has taken a fork\n", 0);
		usleep(args->time_to_die * 1000);
		printf("%zu 1 died\n", args->time_to_die);
		return (1);
	}
	return (0);
}

int	finish_threads(t_philo *philosophers, t_args *args, int f)
{
	int	i;

	i = 0;
	while (i < f)
		pthread_join(philosophers[i++].thread_philo, NULL);
	i = 0;
	while (i < f)
	{
		pthread_mutex_destroy(&philosophers[i].fork);
		pthread_mutex_destroy(&philosophers[i].timer_mutex);
		i++;
	}
	pthread_mutex_destroy(&args->mutex_global);
	pthread_mutex_destroy(&args->mutex_print);
	pthread_mutex_destroy(&args->mutex_end);
	pthread_mutex_destroy(&args->mutex_eat);
	free(philosophers);
	free(args);
	return (0);
}
