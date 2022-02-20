/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tevelyne <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/17 18:55:40 by tevelyne          #+#    #+#             */
/*   Updated: 2021/06/29 17:07:08 by tevelyne         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./src/pipex.h"

char	**command_path(char **env)
{
	int		i;
	char	*tmp;
	char	**bin;

	i = 0;
	tmp = NULL;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			tmp = ft_substr(env[i], 5, ft_strlen(env[i]));
			break ;
		}
		i++;
	}
	bin = ft_split(tmp, ':');
	free(tmp);
	return (bin);
}

void	ft_path(char **path, char **bin, char *com)
{
	char	*tmp;
	int		i;
	int		fd;

	i = 0;
	while (bin[i])
	{
		tmp = ft_strjoin(bin[i], "/");
		*path = ft_strjoin(tmp, com);
		free(tmp);
		fd = open(*path, O_RDONLY);
		if (fd > 0)
		{
			close(fd);
			break ;
		}
		close(fd);
		free(*path);
		i++;
	}
	if (fd < 0)
		error_message("ERROR: command not found!\n");
}

void	first_process(char **argv, int fd[2], char **com1, char **env)
{
	int		file;
	int		process_id;
	char	**bin;
	char	*path;

	bin = command_path(env);
	ft_path(&path, bin, com1[0]);
	free_ft(bin);
	process_id = fork();
	if (process_id < 0)
		error_message("Could not copy the process!\n");
	if (process_id == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		file = open(argv[1], O_RDONLY, 0777);
		if (file == -1)
			error_message("Error: fist file not created!\n");
		dup2(file, 0);
		execve(path, com1, env);
		perror(strerror(errno));
	}
	free(path);
	waitpid(process_id, NULL, 0);
}

void	second_process(char **argv, int fd[2], char **com2, char **env)
{
	int		file;
	int		process_id;
	char	**bin;
	char	*path;

	bin = command_path(env);
	ft_path(&path, bin, com2[0]);
	free_ft(bin);
	process_id = fork();
	if (process_id < 0)
		error_message("Could not copy the process!\n");
	if (process_id == 0)
	{
		file = open(argv[4], O_RDWR | O_CREAT | O_TRUNC, 0777);
		close(fd[1]);
		dup2(file, STDOUT_FILENO);
		dup2(fd[0], 0);
		close(fd[0]);
		close(file);
		execve(path, com2, env);
		perror(strerror(errno));
	}
	close(fd[1]);
	free(path);
	waitpid(process_id, NULL, 0);
}

int	main(int argc, char **argv, char **env)
{
	char	**com1;
	char	**com2;
	int		fd[2];

	if (argc != 5)
		error_message("ERROR: wrong number of arguments!\n");
	if (pipe(fd) == -1)
		error_message("ERROR: bad file descriptor!\n");
	com1 = ft_split(argv[2], ' ');
	com2 = ft_split(argv[3], ' ');
	first_process(argv, fd, com1, env);
	free_ft(com1);
	second_process(argv, fd, com2, env);
	free_ft(com2);
	return (0);
}
