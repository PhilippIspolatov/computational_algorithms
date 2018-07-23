#include <stdio.h>
#include <stdlib.h>

typedef struct tabl t_tabl;

struct tabl
{
	double x;
	double y;
};

t_tabl* get_tabl (FILE *f, int n)
{
	t_tabl *mas = NULL;
	mas = malloc(n * sizeof(t_tabl));
	if (mas != NULL)
	{
		for (int i = 0; i < n; i++)
		{
			//printf("Введите координаты %d точки через пробел\n", i);
			fscanf(f, "%lf %lf", &mas[i].x, &mas[i].y);
			//printf("%d %d\n", (mas[i]).x, (mas[i]).y);
		}
	}
	return mas;

}

t_tabl* search_tabl(t_tabl *mas, int n, int k, double x)
{
	//printf("In Func\n");
	t_tabl *n_mas = malloc((k + 1) * sizeof(t_tabl));;

	if (x < mas[0].x)
	{
		//printf("111\n");
		x = mas[0].x;
	}
	if (x > mas[n - 1].x)
	{
		//printf("222\n");
		x = mas[n - 1].x;
	}
	//printf("!!!\n");
	for (int i = 0; i < n - 1; i++)
	{
		//printf ("%lf %lf\n", mas[i].x, mas[i+1].x);
		if ((mas[i].x <= x) && (mas[i + 1].x >= x))
		{
			//printf("X is find\n");
			if ((i + 1 > k / 2) && (n - i > k / 2))
			{
				//printf("Sered\n");
				for (int j = 0; j < k + 1; j++)
				{
					n_mas[j] = mas[i - k / 2 + j];
				}
			}
			else if (i + 1 <= k / 2)
			{
				//printf("Nachalo\n");
				for (int j = 0; j < k + 1; j++)
				{
					//printf("$\n");
					n_mas[j].x = mas[j].x;
					n_mas[j].y = mas[j].y;
					//printf("->%lf\n", n_mas[j].x);
					//printf("$$$\n");
				}
			}
			else if (n - i <= k / 2)
			{
				//printf("Conec\n");
				for (int j = 0; j < k + 1; j++)
					n_mas[j] = mas[n - k + j - 1];
			}
			break;
		}
	}
	// for (int i = 0; i < k + 1; i++)
	// {
	// 	printf("x/y = %lf %lf\n", n_mas[i].x, n_mas[i].y);
	// }	

	 return n_mas;
}

void free_alloc(float **data, int n)
{

	for (int i = 0; i < n; i++)
	{
		free(data[i]);
	}
	free(data);
}



float **allocate(int k)
{
	float **data = calloc(k, sizeof(float*));
	if (!data)
		return NULL;
	for (int i = 0; i < k; i++)
	{
		data[i] = malloc((k - i) * sizeof(float));
		if (!data[i])
		{
			free_alloc(data, k);
			return NULL;
		}
	}
	return data;
}

int get_pol(float **data, int k, t_tabl *mas)
{
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k - i; j++)
		{
			if (i == 0)
			{
				data[i][j] = (mas[j + 1].y - mas[j].y)/((mas[j + 1].x - mas[j].x));
			}
			else
			{
				//printf("%f - %f / %lf - %lf\n", data[i - 1][j + 1], data[i - 1][j], mas[j + i + 1].x, mas[j].x);
				data[i][j] = (data[i - 1][j + 1] - data[i - 1][j]) / (mas[j + i + 1].x - mas[j].x);
			}
		}
	}

	// for (int i = 0; i < k; i++)
	// {
	// 	for (int j = 0; j < k - i; j++)
	// 		{
	// 			printf("f[%d][%d]: %lf\n\n", i + 1, j, data[i][j]);
	// 		}
	// }
	return 0;
}

t_tabl* retake(t_tabl *mas, int n)
{
	//int tmp;
	t_tabl *n_mas = malloc(n * sizeof(t_tabl));
	for (int i = 0; i < n; i++)
	{
		n_mas[i].x = mas[i].y;
		n_mas[i].y = mas[i].x;
	}

	// for (int i = 0; i < n; i++)
	// {
	// 	printf("%d : %d\n", n_mas[i].x, n_mas[i].y);
	// }
	return n_mas;
}

double f_x(double x, float **data, t_tabl *mas, int k)
{
	double y = mas[0].y;
	double y_1 = 1;
	for (int i = 0; i < k; i++)
	{
		y_1 = 1;
		for (int j = 0; j < i + 1; j++)
		{
			y_1 *= (x - mas[j].x);
		}
		//printf("y_1 = %f\n", y_1);
		//printf("data[i][0] = %f\n", data[i][0]);
		y += data[i][0] * y_1;
		//printf("y = %f\n", y);
	}

	return y;
	
}



int main()
{
	FILE *f;
	int n;
	int k;
	double y = 0.0;
	double x = 0.0;
	double y_q = 0.0;
	double x_a = 0.0;
	float **data = NULL;
	t_tabl *mas = NULL;
	t_tabl *n_mas = NULL;
	t_tabl *new_mas = NULL;
	t_tabl *nnw_mas = NULL;

	setbuf(stdout, NULL);

	f = fopen("test.txt","r");
	if (!f)
		return -1;

	//printf("Введите количество координат\n");
	fscanf(f, "%d", &n);
	printf("Введите степень многочлена: \n");
	scanf("%d", &k);
	if (n < k + 1)
		printf("Вычисление невозможно\n");
	else
	{
		printf("Введите х:\n");
		scanf("%lf", &x);
		printf("Введите y (для обратной интерполяции):\n");
		scanf("%lf", &y_q);
		mas = get_tabl(f, n);
		if (n > k + 1)
		{
			//printf("n > k\n");
			n_mas = search_tabl(mas, n, k, x);
			//printf("Get_Ok");
			// printf("x | y\n");
			// for (int i = 0; i < k; i++)
			// 	printf("%lf | %lf\n", n_mas[i].x, n_mas[i].y);

			data = allocate(k);
			get_pol(data, k, n_mas);
			y = f_x(x, data, n_mas, k);
			printf("Y = %f\n", y);
			new_mas = retake(mas, n);
			nnw_mas = search_tabl(new_mas, n, k, y_q);
			get_pol(data, k, nnw_mas);
			x_a = f_x(y_q, data, nnw_mas,k);
			printf("Корень  = %lf\n", x_a);


			// Дописать вызовы второй части !!!
		}
		else 
		{
			printf("x | y\n");
			for (int i = 0; i < n; i++)
				printf("%lf | %lf\n", mas[i].x, mas[i].y);
			data = allocate(k);
			get_pol(data, k, mas);
			y = f_x(x, data, mas, k);
			printf("Y = %f\n", y);
			new_mas = retake(mas, n);
			get_pol(data, k, new_mas);
			x_a = f_x(y_q, data, new_mas,k);
			printf("Корень  = %lf\n", x_a);
		}
	}
	return 0;
}