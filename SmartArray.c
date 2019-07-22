// Andrew Gomes - an289864 - SmartArray.c
// 9-11-2017
// COP 3502 FALL (Professor: Sean Szumlanski)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "SmartArray.h"

SmartArray *createSmartArray(int length) {
	int i;
	int size = DEFAULT_INIT_LEN > length ? DEFAULT_INIT_LEN : length;
	SmartArray *array = malloc(sizeof(struct SmartArray));
	if (array == NULL) {
		// Malloc failed for the struct
		return NULL;
	}
	array->array = malloc(sizeof(char*) * size);
	if (array->array == NULL) {
		// Malloc failed for the elements
		return NULL;
	} else {
		// Can now return the created SmartArray, malloc was successful both times
		array->size = 0;
		array->capacity = size;
		for (i = 0; i < size; i++) {
			array->array[i] = NULL;
		}
		printf("-> Created new SmartArray of size %i.\n", size);
		return array;
	}
	return NULL;
}

SmartArray *destroySmartArray(SmartArray *smarty) {
	int i;
	if (smarty == NULL) {
		return NULL;
	}
	free(smarty);
	smarty = NULL;
	return NULL;
}

SmartArray *expandSmartArray(SmartArray *smarty, int length) {
	if (smarty == NULL) {
		return NULL;
	}
	if (smarty->capacity >= length) {
		// if the current capacity is already larger than length param.
		return smarty;
	}
	// Reallocate the memory to be bigger
	char **arrayRealloc = realloc(smarty->array, sizeof(char*) * length);	
	if (arrayRealloc != NULL) {
		smarty->array = arrayRealloc;
		smarty->capacity = length;
		printf("-> Expanded SmartArray to size %i.\n", length);
		return smarty;
	}
	return NULL;
}

SmartArray *trimSmartArray(SmartArray *smarty) {
	int i;
	if (smarty == NULL) {
		return NULL;
	}
	if (smarty->capacity <= 0) {
		return smarty;
	}
	// We only trim the array if the capacity > current size, otherwise, we'll just return the
	// original smarty that was passed in.
	if (smarty->capacity > smarty->size) {
		if (smarty->size <= 0) {
			// Just get rid of the array if trimming to size 0
			free(smarty->array);
			smarty->capacity = 0;
			smarty->array = NULL;
			printf("-> Trimmed SmartArray to size 0.\n");
			return smarty;
		}
		char **arrayRealloc = realloc(smarty->array, sizeof(char*) * smarty->size);
		if (arrayRealloc != NULL) {
			smarty->capacity = smarty->size;
			smarty->array = arrayRealloc;	
			printf("-> Trimmed SmartArray to size %i.\n", smarty->size);
			return smarty;
		}
		// Malloc failed
		return NULL;
	}
	// Otherwise return the original smarty
	return smarty;
}

int removeElement(SmartArray *smarty, int index) {
	int i;
	char *newArray;
	if (smarty == NULL || index > getSize(smarty)) {
		return 0;
	}
	newArray = malloc(sizeof(char*) * (getSize(smarty) - 1));
	if (newArray == NULL) {
		// Malloc failed for the new elements
		return 0;
	}
	// Transfer the old elements to the new array, if there are extra elements
	if (getSize(smarty) > 1) {	
		for (i = 0; i < getSize(smarty); i++) {
			if (i == index) {
				// The element that is being removed
				continue;
			}
			// Puts the elements in the same order if less than the removed element, or
			// shifts the element over by one if it came after the removed element.
			newArray[i > index ? i - 1 : i] = smarty->array[i > index ? i - 1 : i];
		}
	}
	free(smarty->array);
	smarty->array = newArray;
	smarty->size--;
	return 1;
}

char *put(SmartArray *smarty, char *str) {
	if (smarty == NULL || str == NULL) {
		return NULL;
	}
	// Array will need to be expanded if it is currently full
	if (getSize(smarty) >= smarty->capacity) {
		smarty = expandSmartArray(smarty, (smarty->capacity * 2 + 1));
	}
	// +1 for the \0 terminator character
	smarty->array[smarty->size] = malloc(sizeof(char*) * (strlen(str) + 1));
	if (smarty->array[smarty->size] == NULL) {
		return NULL;
	} else {
		smarty->size++;
		strcpy(smarty->array[smarty->size - 1], str);
		return smarty->array[smarty->size - 1];	
	}
	return NULL;
}

char *set(SmartArray *smarty, int index, char *str) {
	char *validString;
	char *newSpace;
	if (smarty == NULL || index >= getSize(smarty) || index < 0) {
		return NULL;
	}	
	if (validString = get(smarty, index) != NULL) {
		// +1 for the \0 terminator character
		free(smarty->array[index]);
		newSpace = (char*) malloc(sizeof(char) * (strlen(str) + 1));
		if (newSpace == NULL) {
			return NULL;
		}
		smarty->array[index] = newSpace;
		strcpy(smarty->array[index], str);
		return smarty->array[index];
	} else {
		return NULL;
	}
	return NULL;
}

char *insertElement(SmartArray *smarty, int index, char *str) {
	int i;
	char *newElement = (char*) malloc(sizeof(char) * (strlen(str) + 1));
	if (smarty == NULL || index < 0 || newElement == NULL) {
		return NULL;
	}
	if (getSize(smarty) >= smarty->capacity) {
		// Need to expand if array is full
		smarty = expandSmartArray(smarty, smarty->capacity * 2 + 1);
	}
	if (index > getSize(smarty)) {
		// Trying to insert at an index that is far away from the rest of the elements,
		// instead the string just goes to the first empty space!
		// For example, attempting to insert at index 11 on a Smarty that only has 3 strings in it.
		index = getSize(smarty);
	}
	for (i = getSize(smarty); i > index; i--) {
		// Shift all elements from the index to the end of the array 
		// one position to the right (working backwards)
		smarty->array[i] = smarty->array[i - 1];
	}
	smarty->size++;
	
	// For malloc purposes
	smarty->array[index] = newElement;
	
	// Then finally put the new element inside the array with the proper text
	newElement = str;
	strcpy(smarty->array[index], newElement);
	return smarty->array[index];
}

int getSize(SmartArray *smarty) {
	return smarty == NULL ? -1 : smarty->size;
}

void printSmartArray(SmartArray *smarty) {
	int i;
	if (smarty == NULL || getSize(smarty) <= 0) {
		printf("(empty array)\n");
		return;
	}
	for (i = 0; i < getSize(smarty); i++) {
		printf("%s\n", smarty->array[i]);
	}
}

char *get(SmartArray *smarty, int index) {
	if (smarty == NULL || index >= getSize(smarty) || index < 0) {
		// Safely allow callers of this method to get a string
		// without having to worry about going out of bounds
		return NULL;
	}
	return smarty->array[index];
}

double difficultyRating(void) {
	return 3.75;
}

double hoursSpent(void) {
	return 14.0;
}
