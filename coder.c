// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   coder.c                                            :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/05/05 09:36:02 by rulouis           #+#    #+#             */
// /*   Updated: 2026/05/19 00:00:00 by rulouis          ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "codexion.h"

// static int	coder_left_dongle_id(const t_coder *c)
// {
// 	return ((c->id - 1 + c->sim->args.nb_coders) % c->sim->args.nb_coders);
// }

// static int	coder_right_dongle_id(const t_coder *c)
// {
// 	return (c->id % c->sim->args.nb_coders);
// }

// static void	dongle_put_ordered(t_coder *c, int left, int right)
// {
// 	if (c->sim->args.nb_coders == 1)
// 	{
// 		dongle_put(&c->sim->dongles[left], c);
// 		return ;
// 	}
// 	if (c->id % 2 == 0)
// 	{
// 		dongle_put(&c->sim->dongles[left], c);
// 		dongle_put(&c->sim->dongles[right], c);
// 	}
// 	else
// 	{
// 		dongle_put(&c->sim->dongles[right], c);
// 		dongle_put(&c->sim->dongles[left], c);
// 	}
// }

// static int	take_dongles(t_coder *c, int left, int right)
// {
// 	if (c->id % 2 == 0)
// 	{
// 		dongle_take(&c->sim->dongles[right], c,
// 			sched_priority(c->sim, c->id));
// 		if (c->sim->stop_flag)
// 		{
// 			dongle_put(&c->sim->dongles[right], c);
// 			return (0);
// 		}
// 		if (c->sim->args.nb_coders > 1)
// 			dongle_take(&c->sim->dongles[left], c,
// 				sched_priority(c->sim, c->id));
// 	}
// 	else
// 	{
// 		dongle_take(&c->sim->dongles[left], c,
// 			sched_priority(c->sim, c->id));
// 		if (c->sim->stop_flag)
// 		{
// 			dongle_put(&c->sim->dongles[left], c);
// 			return (0);
// 		}
// 		if (c->sim->args.nb_coders > 1)
// 			dongle_take(&c->sim->dongles[right], c,
// 				sched_priority(c->sim, c->id));
// 	}
// 	return (1);
// }

// void	*coder_routine(void *arg)
// {
// 	t_coder	*c;
// 	int		left;
// 	int		right;
// 	int		j;

// 	c = (t_coder *)arg;
// 	left = coder_left_dongle_id(c);
// 	right = coder_right_dongle_id(c);
// 	while (c->sim->stop_flag == 0)
// 	{
// 		if (!take_dongles(c, left, right))
// 			break ;
// 		if (c->sim->stop_flag)
// 		{
// 			dongle_put_ordered(c, left, right);
// 			break ;
// 		}
// 		if (elapsed_ms(c->last_compile_start_ms) >= c->sim->args.time_to_burnout)
// 		{
// 			c->state = BURNED_OUT;
// 			c->sim->stop_flag = 1;
// 			dongle_put_ordered(c, left, right);
// 			j = 0;
// 			while (j < c->sim->args.nb_coders)
// 			{
// 				pthread_cond_broadcast(&c->sim->dongles[j].cond);
// 				j++;
// 			}
// 			return (NULL);
// 		}
// 		pthread_mutex_lock(&c->sim->state_mutex);
// 		c->last_compile_start_ms = now_ms();
// 		pthread_mutex_unlock(&c->sim->state_mutex);
// 		c->state = COMPILING;
// 		log_event(c->sim, c->id, "is compiling");
// 		sleep_ms(c->sim->args.time_to_compile);
// 		pthread_mutex_lock(&c->sim->state_mutex);
// 		c->compile_count++;
// 		pthread_mutex_unlock(&c->sim->state_mutex);
// 		dongle_put_ordered(c, left, right);
// 		pthread_mutex_lock(&c->sim->state_mutex);
// 		if (c->sim->args.nb_compiles_required > 0
// 			&& c->compile_count >= c->sim->args.nb_compiles_required)
// 		{
// 			c->sim->stop_flag = 1;
// 			pthread_mutex_unlock(&c->sim->state_mutex);
// 			j = 0;
// 			while (j < c->sim->args.nb_coders)
// 			{
// 				pthread_cond_broadcast(&c->sim->dongles[j].cond);
// 				j++;
// 			}
// 			break ;
// 		}
// 		pthread_mutex_unlock(&c->sim->state_mutex);
// 		if (c->sim->stop_flag)
// 			break ;
// 		c->state = DEBUGGING;
// 		log_event(c->sim, c->id, "is debugging");
// 		sleep_ms(c->sim->args.time_to_debug);
// 		if (c->sim->stop_flag)
// 			break ;
// 		c->state = REFACTORING;
// 		log_event(c->sim, c->id, "is refactoring");
// 		sleep_ms(c->sim->args.time_to_refactor);
// 		if (c->sim->stop_flag)
// 			break ;
// 	}
// 	return (NULL);
// }

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:02 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 00:00:00 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_left_dongle_id(const t_coder *c)
{
	return ((c->id - 1 + c->sim->args.nb_coders) % c->sim->args.nb_coders);
}

static int	coder_right_dongle_id(const t_coder *c)
{
	return (c->id % c->sim->args.nb_coders);
}

static void	dongle_put_ordered(t_coder *c, int left, int right)
{
	if (c->sim->args.nb_coders == 1)
	{
		dongle_put(&c->sim->dongles[left], c);
		return ;
	}
	if (c->id % 2 == 0)
	{
		dongle_put(&c->sim->dongles[left], c);
		dongle_put(&c->sim->dongles[right], c);
	}
	else
	{
		dongle_put(&c->sim->dongles[right], c);
		dongle_put(&c->sim->dongles[left], c);
	}
}

static void	interruptible_sleep(t_coder *c, long ms)
{
	long	end;

	end = now_ms() + ms;
	while (now_ms() < end && !c->sim->stop_flag)
		sleep_ms(1);
}

static int	take_dongles(t_coder *c, int left, int right)
{
	if (c->id % 2 == 0)
	{
		dongle_take(&c->sim->dongles[right], c,
			sched_priority(c->sim, c->id));
		if (c->sim->stop_flag)
		{
			dongle_put(&c->sim->dongles[right], c);
			return (0);
		}
		if (c->sim->args.nb_coders > 1)
			dongle_take(&c->sim->dongles[left], c,
				sched_priority(c->sim, c->id));
	}
	else
	{
		dongle_take(&c->sim->dongles[left], c,
			sched_priority(c->sim, c->id));
		if (c->sim->stop_flag)
		{
			dongle_put(&c->sim->dongles[left], c);
			return (0);
		}
		if (c->sim->args.nb_coders > 1)
			dongle_take(&c->sim->dongles[right], c,
				sched_priority(c->sim, c->id));
	}
	return (!c->sim->stop_flag);
}

void	*coder_routine(void *arg)
{
	t_coder	*c;
	int		left;
	int		right;

	c = (t_coder *)arg;
	left = coder_left_dongle_id(c);
	right = coder_right_dongle_id(c);
	if (c->id % 2 == 0)
		sleep_ms(1);
	while (!c->sim->stop_flag)
	{
		if (!take_dongles(c, left, right))
			break ;
		if (c->sim->stop_flag)
		{
			dongle_put_ordered(c, left, right);
			break ;
		}
		pthread_mutex_lock(&c->sim->state_mutex);
		c->last_compile_start_ms = now_ms();
		c->state = COMPILING;
		pthread_mutex_unlock(&c->sim->state_mutex);
		log_event(c->sim, c->id, "is compiling");
		interruptible_sleep(c, c->sim->args.time_to_compile);
		pthread_mutex_lock(&c->sim->state_mutex);
		c->compile_count++;
		c->state = DEBUGGING;
		pthread_mutex_unlock(&c->sim->state_mutex);
		dongle_put_ordered(c, left, right);
		if (c->sim->stop_flag)
			break ;
		log_event(c->sim, c->id, "is debugging");
		interruptible_sleep(c, c->sim->args.time_to_debug);
		if (c->sim->stop_flag)
			break ;
		pthread_mutex_lock(&c->sim->state_mutex);
		c->state = REFACTORING;
		pthread_mutex_unlock(&c->sim->state_mutex);
		log_event(c->sim, c->id, "is refactoring");
		interruptible_sleep(c, c->sim->args.time_to_refactor);
	}
	return (NULL);
}