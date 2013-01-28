#include <stdlib.h>

typedef struct Array2D
{
	int **Array;
	size_t Width;
	size_t Height;
};

Array2D *CreateArray2D(Array2D *previous, int w, int h)
{
	Array2D *array = previous;
	
	if (!array)
	{
		array->Array = NULL;
		array->Width = 0;
		array->Height = 0;
	}
	
	array->Array = (int **)realloc(array->Array, w * sizeof(int *));
	
	for (int x = 0; x < w; x++)
	{
		if (x < array->Width)
			array->Array[x] = (int *)realloc(array, h * sizeof(int));
		else
			array->Array[x] = (int *)malloc(h * sizeof(int))
	}
	
	array->Width = w;
	array->Height = h;
	
	return array;
}

void DeleteArray2D(Array2D *array)
{
	if (!array)
		return;
	
	for (int x = 0; x < array->Width; x++)
		free(array->Array[x])
	
	free(array);
}