#include <stdio.h>
#include <stdlib.h>

typedef struct tabl t_tabl;

struct tabl
{
	double x;
	double y;
};

void free_alloc(double **data, int n)
{

	for (int i = 0; i < n; i++)
	{
		free(data[i]);
	}
	free(data);
}



double **allocate_k(int k)
{
	double **data = calloc(k, sizeof(double*));
	if (!data)
		return NULL;
	for (int i = 0; i < k; i++)
	{
		data[i] = malloc((k - i) * sizeof(double));
		if (!data[i])
		{
			free_alloc(data, k);
			return NULL;
		}
	}
	return data;
}


double **allocate(int n, int m)
{
	double **data = calloc(n, sizeof(double*));
	if (!data)
		return NULL;
	for (int i = 0; i < n; i++)
	{
		data[i] = malloc(m*sizeof(double));
		if (!data[i])
		{
			free_alloc(data, n);
			return NULL;
		}
	}
	return data;
}

int search(double *mas_x, int m, int k_x, double x, int *j_b, int *j_e)
{
	if (x < mas_x[0])
		x = mas_x[0];
	if (x > mas_x[m - 1])
		x = mas_x[m - 1];
	for (int i = 0; i < m - 1; i++)
	{
		//printf ("%lf %lf\n", mas[i].x, mas[i+1].x);
		if ((mas_x[i] <= x) && (mas_x[i + 1] >= x))
		{
			//printf("X is find\n");
			if ((i + 1 > k_x / 2) && (m - i > k_x / 2))
			{
				if (k_x % 2 == 0)
				{
					*j_b = i - k_x / 2 ;
					*j_e = i + k_x / 2;
				}
				else
				{
					*j_b = i - k_x / 2 ;
					*j_e = i + k_x / 2 + 1;
				}
				//printf("Sered\n");
				// for (int j = 0; j < k + 1; j++)
				// {
				// 	n_mas[j] = mas[i - k / 2 + j];
				// }
			}
			else if (i + 1 <= k_x / 2)
			{
				//printf("Nachalo\n");
				*j_b = 0;
				*j_e = k_x + 1;
				//for (int j = 0; j < k + 1; j++)
				//{
					//printf("$\n");
					// n_mas[j].x = mas[j].x;
					// n_mas[j].y = mas[j].y;
					//printf("->%lf\n", n_mas[j].x);
					//printf("$$$\n");
				//}
			}
			else if (m - i <= k_x / 2)
			{
				printf("Conec\n");
				*j_b = m - k_x - 1;
				*j_e = m;
				// for (int j = 0; j < k + 1; j++)
				// 	n_mas[j] = mas[n - k + j - 1];
			}
			break;
		}
	}
	return 0;
}

double get_pol(double **data, int k, double *mas_x, double *mas_y, int x_b)
{
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k - i; j++)
		{
			if (i == 0)
			{
				//printf("-> %lf - %lf / %lf - %lf\n", mas_y[x_b + j + 1], mas_y[x_b + j], mas_x[x_b + j + 1], mas_x[x_b + j]);
				data[i][j] = (mas_y[x_b + j + 1] - mas_y[x_b + j])/((mas_x[x_b + j + 1] - mas_x[x_b + j]));
				//printf("The 1st rr\n");
			}
			else
			{
				//printf("%f - %f / %lf - %lf\n", data[i - 1][j + 1], data[i - 1][j], mas_x[j + i + 1], mas_x[j]);
				data[i][j] = (data[i - 1][j + 1] - data[i - 1][j]) / (mas_x[j + i + 1] - mas_x[j]);
				//printf("The no 1st rr \n");
			}
		}
	}

	/*printf("pr in f\n");
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k - i; j++)
			{
				printf("f[%d][%d]: %lf\n\n", i + 1, j, data[i][j]);
			}
	*/	
	return 0;
}

int create_mas(FILE *f, int n, int m, double *mas_x, double *mas_y, double **matr_z)
{

	for (int i = 0; i < m; i++)
		fscanf(f, "%lf ", &mas_x[i]);
	for (int i = 0; i < n; i++)
		fscanf(f, "%lf ", &mas_y[i]);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
			fscanf(f, "%lf ", &matr_z[i][j]);
	}

	return 0;
}

double f_x(double x, double **data, double *mas_x, double *mas_y, int k, int x_b)
{
	double y = mas_y[x_b];
	double y_1 = 1;
	for (int i = 0; i < k; i++)
	{
		y_1 = 1;
		for (int j = 0; j < i + 1; j++)
		{
			//printf("->y_1 = %f\n", y_1);
			y_1 *= (x - mas_x[x_b + j]);
			//printf("-->y_1 = %f\n", y_1);
		}
		//printf("y_before = %lf\n", y);
		//printf("y_1 = %f\n", y_1);
		//printf("data[i][0] = %f\n", data[i][0]);
		y += data[i][0] * y_1;
		//printf("y = %f\n", y);
	}

	return y;
	
}

double ok_res(double x, double y)
{
	double z = 0;
	z = x*x + y*y + 2;
	return z;
}


int main()
{
	FILE *f1;
	int n = 0;
	int m = 0;
	double x = 0;
	double y = 0;
	double res = 0;
	double f_res = 0;
	int k_x = 0;
	int k_y = 0;
	int x_b = 0;
	int x_e = 0;
	int y_b = 0;
	int y_e = 0;

	//t_tabl *mas = NULL;
	double **data = NULL;
	double **data_2 = NULL;
	double **matr_z;
	double *mas_x = NULL;
	double *mas_y = NULL;
	double *mas_res = NULL;
	f1 = fopen("test.txt", "r");
	if (fscanf(f1 ,"%d %d", &n, &m) != 2)
		return -1;
	matr_z = allocate(n, m);
	mas_x = calloc(m, sizeof(double));
	mas_y = calloc(n, sizeof(double));
	create_mas(f1, n, m, mas_x, mas_y, matr_z);
	printf("Введите степень многочлена по х:\n");
	scanf("%d", &k_x);
	printf("Введите степень многочлена по y:\n");
	scanf("%d", &k_y);
	if ((k_x > m + 1) || (k_y > n + 1))
		printf("Вычисление невозможно.\n");
	else
	{
		printf("Введите х и y:\n");
		scanf("%lf %lf", &x, &y);
		search(mas_x, m, k_x, x, &x_b, &x_e);
		//printf("x_b = %d\n", x_b);
		//printf("x_e = %d\n", x_e);
		search(mas_y, n, k_y, y, &y_b, &y_e);
		//printf("y_b = %d\n", y_b);
		//printf("y_e = %d\n", y_e);


		/*for (int i = 0; i < m; i++)
			printf("%lf ", mas_x[i]);
		printf("\n");
		for (int i = 0; i < n; i++)
			printf("%lf ", mas_y[i]);
		printf("\n");
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
				printf("%lf ", matr_z[i][j]);
			printf("\n");
		}*/

		mas_res = calloc(k_y, sizeof(double));

		data = allocate_k(k_x);
		//printf("OK0\n");
		for (int i = y_b; i <= y_e; i++)
		{
			//printf("ooo\n");
			//for (int j = x_b; j <= x_e; j++)
			//{
				//printf("111\n");
				data = allocate_k(k_y);
				get_pol(data, k_x, mas_x, matr_z[i], x_b);
				//printf("222\n");
				mas_res[i] = f_x(x, data, mas_x, matr_z[i], k_x, x_b);
				free_alloc(data, k_y);
				//printf("mas_res[i] = %lf\n", mas_res[i]);
				//printf("333\n");
			//}
		}
		//printf("OK1\n");
		data_2 = allocate_k(k_y);
		get_pol(data_2, k_y, mas_y, mas_res, y_b);
		res = f_x(y, data_2, mas_y, mas_res, k_y, y_b);
		f_res = ok_res(x, y);
		printf("res = %lf\n", res);
		printf("ok_res = %lf\n", f_res);



	}

	fclose(f1);
}
