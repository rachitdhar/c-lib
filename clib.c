#include "clib.h"

Node* init_node(void* item, const char* type)
{
	Node* new_node = (Node*)malloc(sizeof(Node));

	new_node->data = item;
	new_node->type = type;
	new_node->next = NULL;
	return new_node;
}

Node* linked_list_append(Node* head, void* item, const char* type)
{
	Node* new_node = init_node(item, type);

	Node* old_head = head;
	head = new_node;
	new_node->next = old_head;

	return head;
}

void linked_list_remove_back(Node* head)
{
	Node* l = head, * r = head->next;
	if (r == NULL) return;

	while (r->next != NULL) {
		l = r;
		r = r->next;
	}
	free(r->data);
	free(r);
	l->next = NULL;
}

Bool linked_list_contains(Node* head, void* item, const char* item_type, Bool(*comparator)(void* item1, void* item2))
{
	Node* n = head;
	do {
		if (
			n->type == item_type &&
			(is_equal(n->data, item, n->type) || comparator != NULL ? comparator(n->data, item) : FALSE)
			)
			return TRUE;
		n = n->next;
	} while (n != NULL);

	return FALSE;
}

Bool is_equal(void* item1, void* item2, const char* type)
{
	if (type == "int") {
		return *(int*)item1 == *(int*)item2;
	}
	else if (type == "float") {
		return *(float*)item1 == *(float*)item2;
	}
	else if (type == "char") {
		return *(char*)item1 == *(char*)item2;
	}
	else if (type == "string" || type == "char*" || type == "const char*") {
		return !strcmp((char**)item1, (char**)item2);
	}
	else return FALSE;
}

void free_linked_list(Node* head)
{
	Node* n = head;
	do {
		Node* temp = n->next;
		free(n->data);
		free(n);
		n = temp;
	} while (n != NULL);
}

void set_init(Set* s)
{
	s->items = (int*)malloc(2 * sizeof(int));
	s->size = 0;
	s->capacity = 2;
}

int set_insert(Set* s, int item)
{
	// check if item already exists
	for (int i = 0; i < s->size; i++)
		if (s->items[i] == item) return 1;

	if (s->size == s->capacity) {
		int* items_new = (int*)malloc(2 * s->capacity * sizeof(int));

		for (int i = 0; i < s->size; i++)
			items_new[i] = s->items[i];

		free(s->items);
		s->items = items_new;
		s->capacity *= 2;
	}
	s->items[s->size++] = item;
	return 0;
}

int set_remove(Set* s, int item)
{
	int index = -1;
	for (int i = 0; i < s->size; i++)
	{
		if (s->items[i] == item) {
			index = i;
			break;
		}
	}

	if (index != -1) {
		for (int i = index; i < s->size - 1; i++)
			s->items[i] = s->items[i + 1];

		s->size--;
		return 0;
	}
	return 1;
}

void multi_concat(char* dest, size_t buffer_size, size_t count, ...)
{
	va_list args;
	va_start(args, count);

	char* curr = va_arg(args, char*);
	for (size_t c = 0; c < count; c++)
	{
		strcat_s(dest, buffer_size, curr);
		curr = va_arg(args, char*);
	}
	va_end(args);
}

Bool is_arrow_key(unsigned long long wParam)
{
	return (wParam == KEY_LEFT || wParam == KEY_RIGHT || wParam == KEY_UP || wParam == KEY_DOWN);
}

int generate_random_int(int lower_limit, int upper_limit)
{
	if (lower_limit > upper_limit) return -1;
	return lower_limit + rand() % (upper_limit - lower_limit + 1);
}

String* init_string()
{
	String* s = (String*)malloc(sizeof(String));
	s->size = 0;
	s->capacity = 2;
	s->val = (char*)malloc(2 * sizeof(char));
	s->val[0] = '\0';
	return s;
}

void clear_string(String* s)
{
	free(s->val);
	s->size = 0;
	s->capacity = 2;
	s->val = (char*)malloc(2 * sizeof(char));
	s->val[0] = '\0';
}

void grow_string(String* s)
{
	char* new_val = (char*)malloc(2 * s->capacity * sizeof(char));

	for (int i = 0; i < s->size; ++i) {
		new_val[i] = s->val[i];
	}
	new_val[s->size] = '\0';
	free(s->val);
	s->val = new_val;
	s->capacity *= 2;
}

void append_string(String* s, char c)
{
	if (s->size == s->capacity - 1) grow_string(s);
	s->val[s->size++] = c;
	s->val[s->size] = '\0';
}

void concat_string(String* s, char* str)
{
	size_t append_len = strlen(str);
	while (s->size + append_len >= s->capacity - 1) grow_string(s);

	for (int i = 0; i < append_len; ++i)
		s->val[s->size + i] = str[i];

	s->size += append_len;
	s->val[s->size] = '\0';
}

String* create_string(char* str)
{
	String* s = init_string();
	concat_string(s, str);
	return s;
}

void free_string(String* s)
{
	free(s->val);
	free(s);
}

Array* init_arr(size_t type_size, const char* type)
{
	Array* a = (Array*)malloc(sizeof(Array));
	a->size = 0;
	a->capacity = 2;
	a->arr = (void*)malloc(2 * type_size);
	a->type = type;
	a->type_size = type_size;
	return a;
}

void grow_string_arr(Array* a)
{
	assert(a->type == "String" && a->type_size == sizeof(String));

	String* new_arr = (String*)malloc(2 * a->capacity * a->type_size);

	for (int i = 0; i < a->size; ++i) {
		new_arr[i] = ((String*)(a->arr))[i];
	}
	free(a->arr);
	a->arr = new_arr;
	a->capacity *= 2;
}

void append_string_arr(Array* a, char* str)
{
	if (a->size == a->capacity) grow_string_arr(a);

	String* s = create_string(str);
	((String*)(a->arr))[a->size++] = *s;
}

void print_string_array(Array* a)
{
	assert(a->type == "String" && a->type_size == sizeof(String));

	String* sarr = (String*)(a->arr);
	for (int i = 0; i < a->size; ++i) {
		printf("%s\n", (sarr[i]).val);
	}
}

void free_string_arr(Array* a)
{
	assert(a->type == "String" && a->type_size == sizeof(String));

	String* sarr = (String*)(a->arr);
	for (int i = 0; i < a->size; ++i) {
		free((sarr[i]).val);
	}
	free(a->arr);
}

char* string_at(Array* a, int pos)
{
	assert(a->type == "String" && a->type_size == sizeof(String) && pos < a->size);

	String* sarr = (String*)(a->arr);
	return (sarr[pos]).val;
}

// returns an array of strings split by the separator
Array* split(const char* str, char separator)
{
	Array* a = init_arr(sizeof(String), "String");
	String* curr = init_string();

	for (int i = 0; i < strlen(str); ++i)
	{
		if (str[i] == separator || str[i] == '\0') {
			append_string_arr(a, curr->val);
			clear_string(curr);
			continue;
		}
		append_string(curr, str[i]);
	}

	if (curr->size > 0) {
		append_string_arr(a, curr->val);
		clear_string(curr);
	}
	free(curr);
	return a;
}

char* read_file(const char* filepath)
{
	FILE* f = fopen(filepath, "r");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	size_t length = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* content = (char*)malloc(length + 1);
	size_t len_read = fread(content, 1, length, f);
	content[len_read] = '\0';
	fclose(f);
	return content;
}

Pair* create_pair(void* key, void* value, const char* key_type, const char* value_type)
{
	Pair* p = (Pair*)malloc(sizeof(Pair));
	p->key = key;
	p->value = value;
	p->key_type = key_type;
	p->value_type = value_type;
	return p;
}

Map* init_map(int (*hash)(void*))
{
	Map* map = (Map*)malloc(sizeof(Map));
	map->size = 0;
	map->capacity = 2;
	map->hash = hash;
	map->data = (Pair*)malloc(2 * sizeof(Pair));

	for (int i = 0; i < 2; ++i) {
		Pair* p = create_pair(NULL, NULL, "void*", "void*");
		map->data[i] = *p;
	}
	return map;
}

void grow_map(Map* map)
{
	Pair* new_data = (Pair*)malloc(2 * map->capacity * sizeof(Pair));

	for (int i = 0; i < map->size; ++i) {
		new_data[i] = map->data[i];
	}

	for (int i = map->size; i < 2 * map->capacity; ++i) {
		new_data[i] = *create_pair(NULL, NULL, "void*", "void*");
	}
	free(map->data);
	map->data = new_data;
	map->capacity *= 2;
}

Bool map_insert(Map* map, void* key, void* value, const char* key_type, const char* value_type)
{
	if (map->size == map->capacity) grow_map(map);

	int index = map->hash(key);
	for (int i = 0; i < map->capacity; ++i) {
		int pos = (index + i) % map->capacity;

		if (map->data[pos].key == NULL) {
			map->data[pos].key = key;
			map->data[pos].value = value;
			map->data[pos].key_type = key_type;
			map->data[pos].value_type = value_type;
			return TRUE;
		}
	}
	return FALSE;
}

void* map_get(Map* map, void* key)
{
	int index = map->hash(key);

	for (int i = 0; i < map->capacity; ++i) {
		int pos = (index + i) % map->capacity;

		if (map->data[pos].key != NULL) {
			return map->data[pos].value;
		}
	}
	return NULL;
}

Bool map_remove(Map* map, void* key)
{
	int index = map->hash(key);

	for (int i = 0; i < map->capacity; ++i) {
		int pos = (index + i) % map->capacity;

		if (map->data[pos].key != NULL) {
			map->data[pos].key = NULL;
			map->data[pos].value = NULL;
			map->data[pos].key_type = "void*";
			map->data[pos].value_type = "void*";
			return TRUE;
		}
	}
	return FALSE;
}

Bool is_in(char* str, size_t count, ...)
{
	va_list args;
	va_start(args, count);

	char* curr = va_arg(args, char*);
	for (size_t c = 0; c < count; c++)
	{
		if (!strcmp(str, curr)) return TRUE;
	}
	va_end(args);
	return FALSE;
}
