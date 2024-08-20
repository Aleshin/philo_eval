/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saleshin <saleshin@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/14 23:14:49 by saleshin          #+#    #+#             */
/*   Updated: 2024/07/14 23:15:01 by saleshin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

enum e_status
{
	TAKEN_FORK,
	EATING,
	SLEEPING,
	THINKING,
	DEAD
};

typedef struct args
{
	int				number_of_philosophers;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	int				number_of_times_each_philosopher_must_eat;
	int				philos_finished;
	int				end;
	pthread_mutex_t	mutex_global;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	mutex_end;
	pthread_mutex_t	mutex_eat;
	pthread_t		thread_monitor;
}	t_args;

typedef struct philo
{
	int				id;
	pthread_mutex_t	fork;
	pthread_mutex_t	timer_mutex;
	pthread_t		thread_philo;
	size_t			timer_life;
	size_t			timer_current;
	int				status;
	int				eat_count;
	t_args			*args;
}	t_philo;

size_t		f_time(size_t start_time);
void		ft_usleep(int millisec);
int			check_args(int argc, char **argv);
unsigned	int	ft_atoi(char *str);
int			print_status(t_philo *philo, char *status);
int			init_args(int argc, char **argv, t_args *args);
int			init_philo(t_philo *philosophers, t_args *args);
int			one_philo(t_args *args);
int			init_threads(t_philo *philosophers, t_args *args);
int			finish_threads(t_philo *philosophers, t_args *args, int f);
int			take_forks(t_philo *philo, t_philo *next_philo);
int			check_taken_fork(t_philo *philo);
int			free_forks(t_philo *philo, t_philo *next_philo);
int			check_eating(t_philo *philo, t_philo *next_philo);
void		*philo_routine(void *data);
void		*monitor(void *data);
int			thread_errors(t_philo *philo, t_args *args, int f);

#endif
