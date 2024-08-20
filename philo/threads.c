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

int	init_timer(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->mutex_global);
	pthread_mutex_unlock(&philo->args->mutex_global);
	pthread_mutex_lock(&philo->timer_mutex);
	philo->timer_life = f_time(philo->args->start_time);
	philo->timer_current = philo->timer_life;
	pthread_mutex_unlock(&philo->timer_mutex);
	return (0);
}

void	*philo_routine(void *data)
{
	t_philo	*philo;
	t_philo	*next_philo;
	int		end;

	philo = (t_philo *)data;
	if (philo->id == philo->args->number_of_philosophers)
		next_philo = (philo - philo->args->number_of_philosophers + 1);
	else
		next_philo = (philo + 1);
	init_timer(philo);
	if (philo->id % 2 == 0)
		ft_usleep(1);
	pthread_mutex_lock(&philo->args->mutex_end);
	end = philo->args->end;
	pthread_mutex_unlock(&philo[0].args->mutex_end);
	while (end == 0)
	{
		take_forks(philo, next_philo);
		check_taken_fork(philo);
		check_eating(philo, next_philo);
		pthread_mutex_lock(&philo->args->mutex_end);
		end = philo->args->end;
		pthread_mutex_unlock(&philo[0].args->mutex_end);
	}
	return (NULL);
}

int	check_dead(t_philo *philo, int i)
{
	pthread_mutex_lock(&philo[i].timer_mutex);
	if (f_time(philo[0].args->start_time) - philo[i].timer_life
		>= philo[0].args->time_to_die)
	{
		print_status(philo, "died");
		pthread_mutex_unlock(&philo[i].timer_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo[i].timer_mutex);
	return (0);
}

void	*monitor(void *data)
{
	t_philo	*philo;
	int		i;

	philo = (t_philo *)data;
	i = 0;
	pthread_mutex_lock(&philo->args->mutex_global);
	pthread_mutex_unlock(&philo->args->mutex_global);
	while (1)
	{
		pthread_mutex_lock(&philo->args->mutex_eat);
		if (philo->args->philos_finished
			== philo->args->number_of_philosophers)
		{
			pthread_mutex_unlock(&philo->args->mutex_eat);
			break ;
		}
		pthread_mutex_unlock(&philo->args->mutex_eat);
		if (check_dead(philo, i))
			break ;
		i = (i + 1) % philo->args->number_of_philosophers;
	}
	pthread_mutex_lock(&philo->args->mutex_end);
	philo->args->end++;
	pthread_mutex_unlock(&philo->args->mutex_end);
	return (NULL);
}

int	thread_errors(t_philo *philo, t_args *args, int f)
{
	if (f != -1)
	{
		printf ("Threads creating error\n");
		args->end++;
		pthread_mutex_unlock(&philo->args->mutex_global);
		finish_threads(philo, args, f);
		return (0);
	}
	if (pthread_create(&args->thread_monitor, NULL, monitor, &philo[0]))
	{
		printf ("Monitor creating error\n");
		args->end++;
		pthread_mutex_unlock(&philo->args->mutex_global);
		finish_threads(philo, args, args->number_of_philosophers);
		return (0);
	}
	args->start_time = f_time(args->start_time);
	pthread_mutex_unlock(&philo->args->mutex_global);
	return (1);
}
