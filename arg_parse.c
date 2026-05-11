/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_parse.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:35:46 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/11 16:51:36 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	parse_args(int argc, char **argv, t_args *out)
{
	if (argv[1] >= '0' && argv[1] <= '9')
		out -> nb_coders = atoi(argv[1]);
	if (argv[2] >= '0' && argv[2] <= '9')
		out -> time_to_burnout = atoi(argv[2]);
	if (argv[3] >= '0' && argv[3] <= '9')
		out -> time_to_compile = atoi(argv[3]);
	if (argv[4] >= '0' && argv[4] <= '9')
		out -> time_to_debug = atoi(argv[4]);
	if (argv[5] >= '0' && argv[5] <= '9')
		out -> time_to_refactor = atoi(argv[5]);
	if (argv[6] >= '0' && argv[6] <= '9')
		out -> nb_compiles_required = atoi(argv[6]);
	if (argv[7] >= '0' && argv[7] <= '9')
		out -> dongle_cooldown = atoi(argv[7]);
	if (argv[8] >= '0' && argv[8] <= '9')
		out -> scheduler = atoi(argv[8]);

	// Returns 0 on success, -1 on invalid input.
    // Validates: all positive integers, scheduler == "fifo"|"edf"
}
	



int	validate_args(const t_args *a)
{
	// Secondary check — returns 0 if consistent, -1 otherwise
}
