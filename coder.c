/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:35:56 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:53:17 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void   *coder_routine(void *arg)
{
	// Thread entry point (cast arg → t_coder *).
    // Loop: take left dongle → take right dongle → log compiling →
    //        sleep compile → put dongles → log debugging →
    //        sleep debug → log refactoring → sleep refactor → repeat.
    // Exits when sim->stop_flag is set.
}

static int  coder_left_dongle_id(const t_coder *c)
{
	// Returns (c->id - 1 + nb) % nb   (0-indexed internally)
}

static int  coder_right_dongle_id(const t_coder *c)
{
	// Returns c->id % nb
}
