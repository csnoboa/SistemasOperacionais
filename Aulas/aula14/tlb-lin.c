unsigned char buffer[4096][4096];

int main()
{
	int i, j, k;

	for (k = 0; k < 50; k++)
	{
		for (i = 0; i < 4096; i++)
		{
			for (j = 0; j < 4096; j++)
			{
				buffer[i][j] = (i+j) % 256;
			}
		}
	}
	return 0;
}

