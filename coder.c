/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:02 by rulouis           #+#    #+#             */
/*   Updated: 2026/06/01 16:32:02 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_compile(t_coder *c, int left, int right)
{
	pthread_mutex_lock(&c->sim->state_mutex);
	c->last_compile_start_ms = now_ms();
	c->state = COMPILING;
	pthread_mutex_unlock(&c->sim->state_mutex);
	log_event(c->sim, c->id, GREEN "is compiling" RESET);
	interruptible_sleep(c, c->sim->args.time_to_compile);
	pthread_mutex_lock(&c->sim->state_mutex);
	c->compile_count++;
	c->state = DEBUGGING;
	pthread_mutex_unlock(&c->sim->state_mutex);
	dongle_put_ordered(c, left, right);
	return (!sim_is_stopped(c->sim));
}

static int	coder_debug_refactor(t_coder *c)
{
	log_event(c->sim, c->id, BLUE "is debugging" RESET);
	interruptible_sleep(c, c->sim->args.time_to_debug);
	if (sim_is_stopped(c->sim))
		return (0);
	pthread_mutex_lock(&c->sim->state_mutex);
	c->state = REFACTORING;
	pthread_mutex_unlock(&c->sim->state_mutex);
	log_event(c->sim, c->id, PURPLE "is refactoring" RESET);
	interruptible_sleep(c, c->sim->args.time_to_refactor);
	return (!sim_is_stopped(c->sim));
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
	while (!sim_is_stopped(c->sim))
	{
		if (!take_dongles(c, left, right))
			break ;
		if (sim_is_stopped(c->sim))
		{
			dongle_put_ordered(c, left, right);
			break ;
		}
		if (!coder_compile(c, left, right))
			break ;
		if (!coder_debug_refactor(c))
			break ;
	}
	return (NULL);
}
