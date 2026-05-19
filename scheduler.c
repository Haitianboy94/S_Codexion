/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:18 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/19 11:34:43 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	sched_priority(const t_sim *sim, int coder_id)
{
	if (sim->args.scheduler == FIFO)
		return (now_ms());
	else if (sim->args.scheduler == EDF)
		return (sim->coders[coder_id - 1].last_compile_start_ms
			+ sim->args.time_to_burnout);
	return (now_ms());
}

int	sched_request(t_dongle *d, int coder_id, long priority)
{
	return (heap_push(&d->queue, (t_heapnode){coder_id, priority}));
}

int	sched_next(t_dongle *d)
{
	t_heapnode	node;

	if (d->queue.size == 0)
		return (-1);
	node = heap_pop(&d->queue);
	return (node.coder_id);
}