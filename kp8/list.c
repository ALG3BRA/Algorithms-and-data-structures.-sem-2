#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Element {
    double value;
    int next;
    int prev;
} Element;

typedef struct List {
    size_t capacity, size;
    int first_empty_element;
    Element *elements;
} List;

typedef struct Iter {
    List *list;
    int current_id;
} Iter;


List *list_init() {
    List *list = malloc(sizeof(List));
    if (list) {
        list->capacity = 2;
        list->size = 0;
        list->first_empty_element = -1; // No empty elements initially
        list->elements = malloc(list->capacity * sizeof(Element));
        if (list->elements) {
            // Initialize barrier element
            list->elements[0].prev = 0;
            list->elements[0].next = 0;
            list->elements[0].value = 0.0;
        }
    }
    return list;
}

void list_destroy(List *list) {
    if (list) {
        free(list->elements);
        free(list);
    }
}

Iter *list_begin(List *list) {
    Iter *iter = malloc(sizeof(Iter));
    if (iter) {
        iter->list = list;
        iter->current_id = list->elements[0].next; // First element after the barrier
    }
    return iter;
}

Iter *list_end(List *list) {
    Iter *iter = malloc(sizeof(Iter));
    if (iter) {
        iter->list = list;
        int id = list->elements[0].next;
        while (list->elements[id].next != 0) {
            id = list->elements[id].next;
        }
        iter->current_id = id; // Last active element
    }
    return iter;
}

double list_iter_val(Iter *iter) {
    if (iter && iter->current_id != 0) {
        return iter->list->elements[iter->current_id].value;
    }
    return 0.0;
}

Iter *list_iter_next(Iter *iter) {
    if (iter && iter->current_id != 0) {
        iter->current_id = iter->list->elements[iter->current_id].next;
    }
    return iter;
}

Iter *list_iter_null(List *list) {
    return list_end(list);
}

bool list_iter_equal(Iter *iter1, Iter *iter2) {
    return (iter1->list == iter2->list) && (iter1->current_id == iter2->current_id);
}

void list_resize(List *list, size_t new_capacity) {
    list->elements = realloc(list->elements, new_capacity * sizeof(Element));
    if (list->elements) {
        for (int i = list->capacity; i < new_capacity; ++i) {
            list->elements[i].next = i + 1;
            list->elements[i].prev = -1;
        }
        list->elements[new_capacity - 1].next = -1; // End of new empty elements
        list->capacity = new_capacity;
    }
}

void list_insert_after(Iter *iter, double val) {
    if (!iter) return;
    List *list = iter->list;
    int current = iter->current_id;

    if (list->size + 1 >= list->capacity) {
        list_resize(list, list->capacity * 2);
    }

    int new_element = list->first_empty_element;
    if (new_element == -1) { // No empty elements available
        new_element = list->size + 1; // Assume elements are tightly packed
    } else {
        list->first_empty_element = list->elements[new_element].next;
    }

    list->elements[new_element].value = val;
    list->elements[new_element].next = list->elements[current].next;
    list->elements[new_element].prev = current;

    list->elements[list->elements[current].next].prev = new_element;
    list->elements[current].next = new_element;

    list->size++;
}

void list_insert_before(Iter *iter, double val) {
    if (!iter) return;
    List *list = iter->list;
    int current = iter->current_id;

    if (list->size + 1 >= list->capacity) {
        list_resize(list, list->capacity * 2);
    }

    int new_element = list->first_empty_element;
    if (new_element == -1) { // No empty elements available
        new_element = list->size + 1; // Assume elements are tightly packed
    } else {
        list->first_empty_element = list->elements[new_element].next;
    }

    list->elements[new_element].value = val;
    list->elements[new_element].prev = list->elements[current].prev;
    list->elements[new_element].next = current;

    list->elements[list->elements[current].prev].next = new_element;
    list->elements[current].prev = new_element;

    list->size++;
}

double list_remove(Iter *iter) {
    if (!iter || iter->current_id == 0) return 0; // Can't remove barrier element

    List *list = iter->list;
    int current = iter->current_id;
    double removed_value = list->elements[current].value;

    list->elements[list->elements[current].prev].next = list->elements[current].next;
    list->elements[list->elements[current].next].prev = list->elements[current].prev;

    // Add the removed element to the free list
    list->elements[current].next = list->first_empty_element;
    list->elements[current].prev = -1; // Mark as free
    list->first_empty_element = current;

    list->size--;

    return removed_value;
}

void list_push_back(List *list, double val) {
    Iter *iter = list_end(list);
    list_insert_before(iter, val);
    free(iter);
}

void list_push_front(List *list, double val) {
    Iter *iter = list_begin(list);
    list_insert_before(iter, val);
    free(iter);
}

bool list_is_empty(List *list) {
    return list->size == 0;
}

bool list_is_sorted(List *list) {
    if (list_is_empty(list) || list->size == 1) {
        return true;
    }
    int current = list->elements[0].next;
    while (list->elements[current].next != 0) {
        if (list->elements[current].value > list->elements[list->elements[current].next].value) {
            return false;
        }
        current = list->elements[current].next;
    }
    return true;
}

void print_list(List *list) {
    if (list_is_empty(list)) {
        printf("List is empty.\n");
        return;
    }
    int current = list->elements[0].next;
    printf("List elements: ");
    while (current != 0) {
        printf("%f ", list->elements[current].value);
        current = list->elements[current].next;
    }
    printf("\n");
}

void print_internal_structure(List *list) {
    printf("Internal structure of the list:\n");
    for (int i = 0; i < list->capacity; ++i) {
        printf("Index: %d, Value: %f, Next: %d, Prev: %d\n", i, list->elements[i].value, list->elements[i].next, list->elements[i].prev);
    }
    printf("\n");
}

int main() {
    List *list = list_init();

    // Basic operations
    printf("Basic operations:\n");
    list_push_back(list, 10.0);
    list_push_back(list, 20.0);
    list_push_front(list, 5.0);
    list_push_front(list, 2.5);

    print_list(list);
    print_internal_structure(list);

    // Insert in the middle
    printf("Insert in the middle:\n");
    Iter *iter = list_begin(list);
    iter = list_iter_next(iter); // Points to the element with value 5.0
    list_insert_after(iter, 7.5);
    free(iter);

    print_list(list);
    print_internal_structure(list);

    // Remove from the middle
    printf("Remove from the middle:\n");
    iter = list_begin(list);
    iter = list_iter_next(iter); // Points to the element with value 5.0
    list_remove(iter);
    free(iter);

    print_list(list);
    print_internal_structure(list);

    // Remove from the front
    printf("Remove from the front:\n");
    iter = list_begin(list);
    list_remove(iter);
    free(iter);

    print_list(list);
    print_internal_structure(list);

    // Remove from the back
    printf("Remove from the back:\n");
    iter = list_end(list);
    list_remove(iter);
    free(iter);

    print_list(list);
    print_internal_structure(list);

    // Check if sorted
    if (list_is_sorted(list)) {
        printf("List is sorted.\n");
    } else {
        printf("List is not sorted.\n");
    }

    // More insertions
    printf("More insertions:\n");
    list_push_back(list, 30.0);
    list_push_front(list, 1.0);
    list_push_back(list, 40.0);

    print_list(list);
    print_internal_structure(list);

    // Mixed operations
    printf("Mixed operations:\n");
    list_push_back(list, 50.0);
    list_push_front(list, 0.5);
    iter = list_begin(list);
    iter = list_iter_next(iter); // Points to the element with value 1.0
    list_insert_after(iter, 1.5);
    free(iter);

    print_list(list);
    print_internal_structure(list);

    iter = list_end(list);
    list_remove(iter);
    free(iter);

    print_list(list);
    print_internal_structure(list);

    // Check if sorted
    if (list_is_sorted(list)) {
        printf("List is sorted.\n");
    } else {
        printf("List is not sorted.\n");
    }

    list_destroy(list);
    return 0;
}
