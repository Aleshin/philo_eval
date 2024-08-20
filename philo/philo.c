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

int	main(int argc, char **argv)
{
	t_philo		*philos;
	t_args		*args;
	int			f;

	args = malloc(sizeof(t_args));
	if (!args)
		return (1);
	if (init_args(argc, argv, args) || one_philo(args))
	{
		free(args);
		return (1);
	}
	philos = malloc(args->number_of_philosophers * sizeof(t_philo));
	if (!philos)
		return (1);
	init_philo(philos, args);
	pthread_mutex_lock(&philos->args->mutex_global);
	f = init_threads(philos, args);
	if (thread_errors(philos, args, f))
	{
		pthread_join(args->thread_monitor, NULL);
		finish_threads(philos, args, args->number_of_philosophers);
	}
	return (0);
}
