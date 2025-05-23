#pragma once

#ifndef C_LIB_H
#define C_LIB_H

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

#ifdef _MSC_VER
#define STRTOK strtok_s
#else
#define STRTOK strtok_r
#endif

#define MAX_CSV_LINE_LENGTH 1000
#define KEY_LEFT	0x25
#define KEY_UP		0x26
#define KEY_RIGHT	0x27
#define KEY_DOWN	0x28

/* ********* TYPES ********* */

typedef enum { FALSE, TRUE } Bool;

typedef struct {
	int* items;
	size_t size;
	size_t capacity;
} Set;

struct Node {
	void* data;
	char* type;
	struct Node* next;
};
typedef struct Node Node;

typedef struct {
	char* val;
	size_t size;
	size_t capacity;
} String;

typedef struct {
	void* arr;
	size_t type_size;
	size_t size;
	size_t capacity;
	char* type;
} Array;

typedef struct {
	void* key;
	void* value;
	char* key_type;
	char* value_type;
} Pair;

typedef struct {
	Pair* data;
	size_t size;
	size_t capacity;
	int (*hash)(void*);
} Map;

typedef struct {
	float** dataset;
	float** normalized_dataset;
	float* mean_features;
	float* std_deviation_features;
	int set_size;
	int vector_size;
} DataSet;

/* ********* SET ********* */

void set_init(Set* s);
int set_insert(Set* s, int item);
int set_remove(Set* s, int item);

/* ********* LINKED LIST ********* */

Node* init_node(void* item, const char* type);
Node* linked_list_append(Node* head, void* item, const char* type);
void linked_list_remove_back(Node* head);
Bool linked_list_contains(Node* head, void* item, const char* item_type, Bool(*comparator)(void* item1, void* item2));
void free_linked_list(Node* head);

/* ********* STRING ********* */

String* init_string();
void clear_string(String* s);
void grow_string(String* s);
void append_string(String* s, char c);
void concat_string(String* s, char* str);
String* create_string(char* str);
void free_string(String* s);
void multi_concat(char* dest, size_t buffer_size, size_t count, ...);

/* ********* DYNAMIC GENERIC ARRAY ********* */

Array* init_arr(size_t type_size, const char* type);
void grow_string_arr(Array* a);
void append_string_arr(Array* a, char* str);
void print_string_array(Array* a);
void free_string_arr(Array* a);
char* string_at(Array* a, int pos);
Array* split(const char* str, char separator);

/* ********* MAP ********* */

Pair* create_pair(void* key, void* value, const char* key_type, const char* value_type);
Map* init_map(int (*hash)(void*));
void grow_map(Map* map);
Bool map_insert(Map* map, void* key, void* value, const char* key_type, const char* value_type);
void* map_get(Map* map, void* key);
Bool map_remove(Map* map, void* key);

/* ********* IO ********* */

Bool is_arrow_key(unsigned long long wParam);
char* read_file(const char* filepath);
DataSet* get_dataset_from_csv(const char* file_path);
void normalize_dataset(DataSet* dataset);

/* ********* LINEAR ALGEBRA ********* */

float* vec_multiply(float scalar, float* vec, int size);
float* vec_diff(float* vec1, float* vec2, int size);
float vec_inner(float* vec1, float* vec2, int size);
float** vec_outer(float* vec1, float* vec2, int vec1_size, int vec2_size);
float* vec_hadamard(float* vec1, float* vec2, int size);
float* get_denormalized_vector(float* normalized_vector, DataSet* dataset);
float** mat_diff(float** matrix1, float** matrix2, int rows, int cols);
float* matmul(float** matrix, float* vec, int vec_size, int output_vec_size, Bool multiply_transposed_matrix);
float** mat_multiply(float scalar, float** matrix, int rows, int cols);
void free_mat(float** matrix, int rows);

/* ********* HELPER FUNCTIONS ********* */

Bool is_equal(void* item1, void* item2, const char* type);
int generate_random_int(int lower_limit, int upper_limit);
Bool is_in(char* str, size_t count, ...);

#endif
