#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <math.h>

using namespace std;

void matrixmult(int **m1, int **m2, int **m3, int size, int num_threads);
void matrixmultLinear(int **m1, int **m2, int **m3, int size);
void matrixfill( int **m, int row, int col);
void matrixFillAuto(int **arr, int size);
void print_matr(int **m, int row, int col);
void FoxAlgorythm (int **A, int **B, int **C, int n, int nProcNum);



//Главная функция программы
int main()
{
    // cout << "Введите размерность матриц ";
    // cin >> size;
    int thread_max = 32;
    int switcher = 0;
    // srand(time(0)); // инициализация генерации случайных чисел
	// clock_t start, stop;
    for(int size = 500; size <= 1000; size = size + 100){
        cout << endl << "-----------------------------Умножение матриц при n= " + to_string(size) << endl;
        for(int p_index = 2; p_index < 32; p_index = p_index * 2){
            int **arr1 = new int*[size];
            for (int i = 0; i < size; ++i){
                arr1[i] = new int[size];
            }
            matrixFillAuto(arr1, size);
            int **arr2 = new int*[size];
            for (int i = 0; i < size; ++i){
                arr2[i] = new int[size];
            }
            matrixFillAuto(arr2, size);
            int row3 = size;
            int col3 = size;
            int **arr3 = new int*[row3];
            for (int i = 0; i < row3; ++i){
                arr3[i] = new int[col3]; 
            }
            for (int i = 0; i < row3; ++i){
                for (int j = 0; j < col3; ++j){
                    arr3[i][j] = 0;
                }  
            }            
            auto begin = std::chrono::steady_clock::now();
            matrixmult(arr1, arr2, arr3, size, p_index);
            auto end = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
            float time = elapsed_ms.count() / 1000.;

            cout << endl;  
            cout << "Время для р=" + to_string(p_index) + ": " + to_string(time) + " сек."<< endl;
            for(int i = 0; i < size; i++){
                delete[] arr1[i];
            } 
            delete[] arr1;

            for(int i = 0; i < size; i++){
                delete[] arr2[i];
            }    
            delete[] arr2;
    
            for(int i = 0; i < row3 ; i++){
                delete[] arr3[i];
            }
            delete[] arr3;  
        }
         
    }
    return 0;
}
 
 
//Алгоритм Фокса
void matrixmult(int **m1, int **m2, int **m3, int size, int num_threads)
{
    
	omp_set_num_threads(num_threads);	
	int temp = 0;
	int i=0, j=0, k=0;

	#pragma omp parallel 
	{	
		int num = omp_get_thread_num();

		#pragma omp parallel for schedule(static)  private(k) 
		for ( k = 0; k < size; ++k)
			#pragma omp parallel for schedule(static)  private(i)
			for ( i = 0; i < size; ++i)
			#pragma omp parallel for schedule(static) shared(m1, m2, m3) private(j) reduction(+:temp)	
				for ( j = 0; j < size; ++j)
				{
					temp += m1[i][j] * m2[j][k];
					m3[i][k] = temp;
				}					
	}					
}

//Последовательный алгоритм
void matrixmultLinear(int **m1, int **m2, int **m3, int size)
{ 
	int temp = 0;
	int i=0, j=0, k=0;

	for ( k = 0; k < size; ++k){
		for ( i = 0; i < size; ++i){	
			for ( j = 0; j < size; ++j)
			{
				temp += m1[i][j] * m2[j][k];
				m3[i][k] = temp;
			}
        }	
    }								
}


//Ручное заполнение
void matrixfill( int **m, int row, int col)
{
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
        {
            cout << "Введите [ " << i << " ][ " << j <<" ] элемент: ";
            cin >> m[i][j];
        }
}

//Автоматическое заполнение матрицы
void matrixFillAuto(int **arr, int size){
        for (int i = 0; i < size; ++i){
            for (int j = 0; j < size; ++j)
            {
                arr[i][j] = 1 + rand() % 20;
            }
        }
}

//Вывод матрицы на экран
void print_matr(int **m, int row, int col)
{
    for (int i = 0; i < row; ++i)
    {
        for (int j = 0; j < col; ++j)
            cout << setw(3) << m[i][j] << ' ';
 
        cout << endl;
    }
}

//Алгоритма Фокса
void FoxAlgorythm (int **A, int **B, int **C, int n, int nProcNum){
    int stage;
    #pragma omp parallel private(stage) shared(A, B, C) num_threads(nProcNum)
    {
        int i, j, k, i1, j1;
        int GridSize = sqrt(nProcNum);
        //printf("GridSize: %d\n", GridSize);
        int PrNum = omp_get_thread_num();
        //printf("PrNum: %d\n", PrNum);
        i1 = PrNum / GridSize;
        j1 = PrNum % GridSize;
        int *A1;
        int *B1;
        int *C1;
        for (stage = 0; stage < GridSize; stage++){
            A1 = A + (n * i1 + ((i1 + stage) % GridSize)) * (n/GridSize);
            B1 = B + (n * ((i1 + stage) % GridSize) + j1) * (n/GridSize);
            C1 = C + (n * i1 + j1) * (n/GridSize);
            for (i = 0; i < n / GridSize; i++){
                for (j = 0; j < n / GridSize; j++){
                    for (k = 0; k < n / GridSize; k++){
                        C1[i*n+j] += A1[i*n+k] * B1[k*n+j];                    
                    }
                }                           
            }
        }
    }
}