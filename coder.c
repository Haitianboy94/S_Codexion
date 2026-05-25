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

static int	coder_compile(t_coder *c, int left, int right)
{
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
	return (!c->sim->stop_flag);
}
 
static int	coder_debug_refactor(t_coder *c)
{
	log_event(c->sim, c->id, "is debugging");
	interruptible_sleep(c, c->sim->args.time_to_debug);
	if (c->sim->stop_flag)
		return (0);
	pthread_mutex_lock(&c->sim->state_mutex);
	c->state = REFACTORING;
	pthread_mutex_unlock(&c->sim->state_mutex);
	log_event(c->sim, c->id, "is refactoring");
	interruptible_sleep(c, c->sim->args.time_to_refactor);
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
		if (!coder_compile(c, left, right))
			break ;
		if (!coder_debug_refactor(c))
			break ;
	}
	return (NULL);
}
