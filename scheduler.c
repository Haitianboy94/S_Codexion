/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:18 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:46:19 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long    sched_priority(const t_sim *sim, int coder_id)
{
	// FIFO → now_ms() at request time
    // EDF  → coder->last_compile_start_ms + args.time_to_burnout
}

int     sched_request(t_dongle *d, int coder_id, long priority)
{
	// Push a request onto d->queue (caller must hold d->mutex)
}

int     sched_next(t_dongle *d)
{
	// Pop the highest-priority coder_id from queue (-1 if empty)
}
