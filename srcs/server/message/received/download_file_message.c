/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getFileProtocolMessage.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jguyet <jguyet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/06 07:06:54 by jguyet            #+#    #+#             */
/*   Updated: 2017/05/06 07:06:56 by jguyet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftp_server.h"
#include "libfile.h"

BOOLEAN	download_file_message(t_socket_server *server,\
	t_client *client, char *message)
{
	char		**split;
	char		*path;
	t_upload	*upload;

	(void)server;
	split = ft_split_string(message, "|");
	if (array_length(split) == 0)
		return (false);
	if (client->pwd[ft_strlen(client->pwd) - 1] == '/')
		path = ft_strjoin(client->pwd, split[0]);
	else
		path = ft_dstrjoin(ft_strjoin(client->pwd, "/"), split[0], 1);
	if ((upload = loadnewupload(split[0], path)) != NULL)
	{
		client->send(client, client->serialize("%c%s|%s|%d", 103,\
			upload->filename, upload->path, upload->size));
		client->upload = upload;
	}
	else if (!(get_file_mode(path) & S_IRUSR))
		client->send(client, client->serialize("%c%s%s\n", 12, "ft_get: Permission denied: ", split[0]));
	else
		client->send(client, client->serialize("%c%s%s\n", 12, "ft_get: no such file or directory: ", split[0]));
	free_array(split);
	return (true);
}
