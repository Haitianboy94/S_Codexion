/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:14 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:54:02 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void   *monitor_routine(void *arg)
{
	// Thread entry point (cast arg → t_sim *).
    // Polls every ~1 ms; for each coder checks:
    //   now_ms() - last_compile_start_ms > time_to_burnout
    // If burnout detected: log "X burned out", set stop_flag,
    //   pthread_cond_broadcast all dongle cvars to unblock waiters.
}

static int  all_done(const t_sim *sim)
{
	// Returns 1 if every coder->compile_count >= nb_compiles_required
}