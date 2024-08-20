/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 23:11:19 by saleshin          #+#    #+#             */
/*   Updated: 2024/07/14 23:11:32 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	f_time(size_t start_time)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000 - start_time);
}

void	ft_usleep(int millisec)
{
	size_t	end;

	end = f_time(0) + millisec;
	while (f_time(0) < end)
		usleep(200);
}

//returns 0 if number is 0, negative or not number
unsigned	int	ft_atoi(char *str)
{
	int	res;

	res = 0;
	while (*str == ' ' || *str == '\n' || *str == '\t')
		str++;
	if (*str == '-')
		return (0);
	if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res = res * 10 + (*str - '0');
		str++;
	}
	if (*str != '\0')
		return (0);
	return (res);
}

int	check_args(int argc, char **argv)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) == 0)
		{
			printf("Wrong arg \"%s\". Need positive int\n", argv[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

int	print_status(t_philo *philo, char *status)
{
	size_t	time;
	int		end;

	pthread_mutex_lock(&philo->args->mutex_print);
	time = f_time(philo->args->start_time);
	pthread_mutex_lock(&philo->args->mutex_end);
	end = philo->args->end;
	pthread_mutex_unlock(&philo->args->mutex_end);
	if (end == 0)
		printf("%zu %d %s\n", time, philo->id, status);
	pthread_mutex_unlock(&philo->args->mutex_print);
	return (0);
}
