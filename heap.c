/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rulouis <rulouis@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 09:36:05 by rulouis           #+#    #+#             */
/*   Updated: 2026/05/05 09:44:10 by rulouis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int     heap_init(t_heap *h, int capacity)
{
	// Allocates h->data; returns 0 or -1
}

void    heap_free(t_heap *h)
{
	
}

int     heap_push(t_heap *h, t_heapnode node)
{
	// Min-heap insert by node.priority; returns 0 or -1
}


t_heapnode heap_pop(t_heap *h)
{
	// Removes & returns minimum node; UB if empty — check heap_empty first
}

int     heap_empty(const t_heap *h)
{
	
}

t_heapnode heap_peek(const t_heap *h)
{
	// Returns minimum node without removing
}

/* internal helpers */
static void     heap_sift_up(t_heap *h, int i)
{
	
}
static void     heap_sift_down(t_heap *h, int i)
{
	
}