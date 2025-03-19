#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MIN_ARRAY_SIZE 50000
#define MAX_ARRAY_SIZE 1000000
#define ARRAY_SIZE_STEP 50000

void FillArr(uint32_t *arr, size_t n) {
    for (size_t i = 0; i < n; i++) {
        arr[i] = rand() % 100001;
    }
}

void copyArray(uint32_t *src, uint32_t *dest, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
    }
}

void mergeSort(uint32_t *arr, size_t n) {
    if (n <= 1) return;

    uint32_t *temp = (uint32_t*)malloc(n * sizeof(uint32_t));
    if (!temp) return;

    size_t mid = n / 2;
    mergeSort(arr, mid);  // Рекурсивная сортировка левой части массива
    mergeSort(arr + mid, n - mid);  // Рекурсивная сортировка правой части массива

    // Слияние отсортированных половин
    size_t left = 0, right = mid, k = 0;
    while (left < mid && right < n) {
        if (arr[left] <= arr[right]) {
            temp[k++] = arr[left++];
        } else {
            temp[k++] = arr[right++];
        }
    }
    
    // Копирование оставшихся элементов из левой или правой части
    while (left < mid) temp[k++] = arr[left++];
    while (right < n) temp[k++] = arr[right++];

    for (size_t i = 0; i < n; i++) {
        arr[i] = temp[i];
    }

    free(temp); // Освобождение памяти
}

// Алгоритм сортировки подсчётом (Counting Sort)
void countingSort(uint32_t *arr, size_t n) {
    uint32_t max = 0;
    // Нахождение максимального элемента в массиве
    for (size_t i = 0; i < n; i++) {
        if (arr[i] > max) max = arr[i];
    }

    // Создание массива для подсчёта частоты элементов
    uint32_t *count = (uint32_t*)calloc(max + 1, sizeof(uint32_t));
    if (!count) return;

    // Подсчёт количества каждого элемента в массиве
    for (size_t i = 0; i < n; i++) count[arr[i]]++;

    // Перезапись массива на основе подсчитанных значений
    size_t index = 0;
    for (uint32_t i = 0; i <= max; i++) {
        while (count[i]--) arr[index++] = i;
    }

    free(count);
}

void bubbleSort(uint32_t *arr, size_t n) {
    for (size_t i = 0; i < n - 1; i++) {
        int swapped = 0;  // Флаг для отслеживания обменов
        for (size_t j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                // Обмен элементов
                uint32_t temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;  // Помечаем, что был обмен
            }
        }
        if (!swapped) break;  // Если обменов не было, массив уже отсортирован
    }
}


void benchmarkSort(const char *sortName, void (*sort)(uint32_t *, size_t), uint32_t *arr, size_t n) {
    uint32_t *tempArr = (uint32_t*)malloc(n * sizeof(uint32_t));
    if (!tempArr) return;

    copyArray(arr, tempArr, n);
    clock_t start = clock();
    sort(tempArr, n);
    clock_t end = clock();

    printf("%s: %zu элементов -> %lf секунд\n", sortName, n, (double)(end - start) / CLOCKS_PER_SEC);
    free(tempArr);
}

int main() {
    srand(time(NULL)); // Без этой строчки псевдослучайные числа будут повторяться
    
    for (size_t n = MIN_ARRAY_SIZE; n <= MAX_ARRAY_SIZE; n += ARRAY_SIZE_STEP) {
        uint32_t *arr = (uint32_t*)malloc(n * sizeof(uint32_t));
        if (!arr) {
            printf("Ошибка выделения памяти\n");
            return 1;
        }

        FillArr(arr, n);
        benchmarkSort("Counting Sort", countingSort, arr, n);
        benchmarkSort("Merge Sort", mergeSort, arr, n);
        benchmarkSort("Bubble Sort", bubbleSort, arr, n);

        free(arr);
    }
    
    return 0;
}
