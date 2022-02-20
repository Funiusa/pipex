/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilityes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:55:59 by tevelyne          #+#    #+#             */
/*   Updated: 2021/06/29 16:55:46 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_ft(char **mass)
{
	int		i;

	i = 0;
	while (mass[i])
	{
		free(mass[i]);
		mass[i] = NULL;
		i++;
	}
	free(mass);
}

void	error_message(char *message)
{
	ft_putstr_fd(message, 2);
	exit(errno);
}
