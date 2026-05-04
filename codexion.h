/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:27:53 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/04 15:41:08 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CODEXION_H
# define CODEXION_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct s_thread
{
	long 			data2;
	long			data1;
	struct s_thread	*next;
}				 t_thread;


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#endif