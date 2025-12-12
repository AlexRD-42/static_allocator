static
void	stt_update_ptrs(char *optr, char **envp, size_t count)
{
	size_t	i;
	char	*wptr;

	i = 0;
	wptr = optr;
	while (i < count)
	{
		envp[i++] = wptr;
		wptr += (1 + (ft_strlen(wptr) + 1) / FT_BLOCK_SIZE) * FT_BLOCK_SIZE;
	}
	envp[i] = NULL;
}

static
void	stt_compact(t_env *env)
{
	size_t	i;
	size_t	length;
	size_t	num_blocks;
	size_t	meta_index;
	char	buffer[FT_ENV_SIZE];

	i = 0;
	meta_index = 0;
	ft_memset(env->metadata, 0, FT_ENV_COUNT);
	while (i < env->count)
	{
		length = ft_strlen(env->ptr[i]) + 1;
		ft_memcpy(buffer + meta_index * FT_BLOCK_SIZE, env->ptr[i], length);
		num_blocks = 1 + length / FT_BLOCK_SIZE;
		stt_assign_blocks(env, meta_index, num_blocks, num_blocks);
		meta_index += num_blocks;
		i++;
	}
	ft_memcpy(env->optr, buffer, meta_index * FT_BLOCK_SIZE);
	stt_update_ptrs(env->optr, env->ptr, env->count);
}