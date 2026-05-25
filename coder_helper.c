/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 13:20:03 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 13:24:14 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	coder_left_dongle_id(const t_coder *c)
{
	return ((c->id - 1 + c->sim->args.nb_coders) % c->sim->args.nb_coders);
}

int	coder_right_dongle_id(const t_coder *c)
{
	return (c->id % c->sim->args.nb_coders);
}

void	dongle_put_ordered(t_coder *c, int left, int right)
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

void	interruptible_sleep(t_coder *c, long ms)
{
	long	end;

	end = now_ms() + ms;
	while (now_ms() < end && !c->sim->stop_flag)
		sleep_ms(1);
}

int	take_dongles(t_coder *c, int left, int right)
{
	if (c->id % 2 == 0)
	{
		dongle_take(&c->sim->dongles[right], c, sched_priority(c->sim, c->id));
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
		dongle_take(&c->sim->dongles[left], c, sched_priority(c->sim, c->id));
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
