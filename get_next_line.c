#include "get_next_line.h"

//read file stores string in the buffer && join it to the stash
static int	read_buffer(int fd, char **stash, char *buffer)
{
	char	*tmp;
	int		bytes;

	ft_memset(buffer, 0, BUFFER_SIZE + 1);
	bytes = read(fd, buffer, BUFFER_SIZE);
	if (bytes < 0 || buffer == NULL)
	{
		free(*stash);
		free(buffer);
		*stash = NULL;
		return (-1);
	}
	if (bytes == 0)
		return (bytes);
	tmp = ft_strjoin(*stash, buffer);
	if (!tmp)
		return (-1);
	free(*stash);
	*stash = tmp;
	return (bytes);
}

//get/extract line with \n character from stash

static void	copy_line(char **stash, char **result)
{
	char	*new_line;
	size_t	len;
	size_t	i;

	new_line = ft_strchr(*stash, '\n');
	len = ft_strlen(*stash) - ft_strlen(new_line) + 2;
	*result = (char *)malloc(sizeof(char) * len);
	if (!result)
		return ;
	i = 0;
	while (i < len - 1)
	{
		(*result)[i] = (*stash)[i];
		i++;
	}
	(*result)[i] = '\0';
}

static void	remove_line(char **stash)
{
	char	*new_line;
	char	*tmp;
	size_t	i;
	size_t	j;
	size_t	line_length;

	if (!stash)
		return ;
	new_line = ft_strchr(*stash,'\n');
	if (!new_line)
	{
		free(*stash);
		*stash = NULL;
		return ;
	}
	line_length = ft_strlen(*stash) - ft_strlen(new_line) + 1;
	tmp = malloc(sizeof(char) * line_length);
	if (!tmp)
		return ;
	i = 0;
	j = ft_strlen(*stash) - ft_strlen(new_line) + 1;
	while (j < ft_strlen(*stash))
		tmp[i++] = (*stash)[j++];
	tmp[i] = '\0';
	free(*stash);
	*stash = tmp;
//	if (**stash == 0)
//	{
//		free(*stash);
//		*stash = NULL;
//	}
}
// 
char	*get_next_line(int fd)
{
	static char	*stash;
	char		*result;
	char		*buffer;
	int			bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		free(stash);
		return (NULL);
	}
	bytes_read = 1;
	while (ft_strchr(stash, '\n') == NULL && bytes_read > 0)
	{
		bytes_read = read_buffer(fd, &stash, buffer);
		if (bytes_read <= 0)
		{
			free(buffer);
			return (NULL);
		}
	}
	free(buffer);
	if (!stash || ft_strlen(stash) == 0)
		return (NULL);
	copy_line(&stash, &result);
	remove_line(&stash);
	return (result);
}

int main()
{
	int	fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("error opening file");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("line: %s", line);
		free(line);
	}
	close(fd);
	return (0);
}
