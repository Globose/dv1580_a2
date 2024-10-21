#include "linked_list.h"

static pthread_mutex_t lock;

/**
 * @brief  Creates a new node
 * @param  data     Data to be stored
 * @param  next     Pointer to the next node
 * @return Node*    Pointer to new node, returns NULL if failed to allocate memory for it
 */
Node* get_new_node(int data, Node* next){
    Node* new_node = (Node*)mem_alloc(sizeof(Node));
    if (new_node == NULL){
        return NULL;
    }
    *new_node = (Node){data,next};
    return new_node;
}

/**
 * @brief  This function sets up the list and prepares it for operations
 * @param  head     Pointer to pointer to head
 * @param  size     Size of memory pool
 * @note   - If head is NULL, nothing happens
 */
void list_init(Node** head, size_t size){
    if (head == NULL) return;
    mem_init(size);
    *head = NULL;
}

/**
 * @brief  Adds a new node with the specified data to the linked list
 * @param  head     Pointer to pointer to head
 * @param  data     Data to be inserted
 * @note   - If head is NULL, nothing happens
 */
void list_insert(Node** head, int data){
    if (head == NULL) return;
    pthread_mutex_lock(&lock);
    if (*head == NULL){
        Node* nptr = get_new_node(data, NULL);
        if (nptr != NULL) *head = nptr; 
    }
    else{
        Node* node = *head;
        while(node->next != NULL){
            node = node->next;
        } 
        Node* nptr = get_new_node(data, NULL);
        if (nptr != NULL) node->next = nptr;
    }
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  Inserts a new node with the specified data immediately after a given node
 * @param  prev_note    Pointer to previous node
 * @param  data         Data to be inserted
 * @note   - If prev_node is NULL, nothing happens
 */
void list_insert_after(Node* prev_node, int data){
    if (prev_node == NULL) return;
    pthread_mutex_lock(&lock);
    Node* new_node = get_new_node(data, prev_node->next);
    if (new_node != NULL) prev_node->next = new_node;
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  Inserts a new node with the specified data immediately before a given node in the list
 * @param  head     Pointer to pointer to head
 * @param  next_node    Pointer to next node
 * @param  data     Data to be inserted
 * @note   - If head is NULL or next_node doesn't exist, nothing happens
 */
void list_insert_before(Node** head, Node* next_node, int data){
    if (head == NULL) return;
    Node* prev_node = NULL;
    Node* node = *head;
    pthread_mutex_lock(&lock);
    while(node != NULL){
        if (node == next_node){
            Node* new_node = get_new_node(data, node);
            if (new_node == NULL) break;
            if (prev_node == NULL){
                *head = new_node;
            }
            else {
                prev_node->next = new_node;
            }
            break;
        }
        else{
            prev_node = node;
            node = node->next;
        }
    }
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  Removes all nodes with the specified data from the linked list.
 * @param  head     Pointer to pointer to head
 * @param  data     Data value to be deleted
 * @note   - If head is NULL, nothing happens
 */
void list_delete(Node** head, int data){
    if (head == NULL) return;
    pthread_mutex_lock(&lock);
    Node* prev_node = NULL;
    Node* node = *head;
    while(node != NULL){
        if (node->data == data){
            Node* next = node->next;
            mem_free(node);
            if (prev_node == NULL){
                *head = next;
            }
            else{
                prev_node->next = next;
            }
            node = next;
        }
        else{
            prev_node = node;
            node = node->next;
        }
    }
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  Searches for a node with the specified data and returns a pointer to it
 * @param  head     Pointer to pointer to head
 * @param  data     Data to search for
 * @return Node*    Pointer to search result
 * @note   - If head is NULL, NULL is returned.
 *         - If no node with the given value is found, NULL is returned.
 */
Node* list_search(Node** head, int data){
    if (head == NULL) return NULL;
    Node*node = *head;
    pthread_mutex_lock(&lock);
    while(node != NULL && node->data != data){
        node = node->next;
    }
    pthread_mutex_unlock(&lock);
    return node;
}

/**
 * @brief  Prints all the elements in a given range
 * @param  head     Pointer to pointer to head
 * @param  Node*    First node to display
 * @param  Node*    Last node to display
 * @note   - If head is NULL, nothing happens.
 *         - If start_node is NULL, displaying will begin with the first node in the list.
 *         - If end_node is NULL, displaying will end with the last node in the list.
 */
void list_display_range(Node** head, Node* start_node, Node* end_node){
    if (head == NULL) return;
    pthread_mutex_lock(&lock);
    printf("[");
    if (start_node == NULL) {
        start_node = *head;
    }
    while (start_node != NULL){
        printf("%d", start_node->data);
        if (start_node == end_node) break;
        start_node = start_node->next;
        if (start_node != NULL) printf(", ");
    }
    printf("]");
    pthread_mutex_unlock(&lock);
}

/**
 * @brief  Prints all the elements in the linked list
 * @param  head     Pointer to pointer to head
 * @note   - If head is NULL, nothing happens.
 */
void list_display(Node** head){
    if (head == NULL) return;
    list_display_range(head, *head, NULL);
}

/**
 * @brief  Returns the count of nodes
 * @param  head     Pointer to pointer to head
 * @note   - If head is NULL, -1 will be returned.
 */
int list_count_nodes(Node** head){
    if (head == NULL) return -1;
    pthread_mutex_lock(&lock);
    Node* node = *head;
    int counter = 0;
    while (node != NULL){
        node = node->next;
        counter++;
    }
    pthread_mutex_unlock(&lock);
    return counter;
}

/**
 * @brief  Frees all the nodes in the linked list
 * @param  head     Pointer to pointer to head
 * @note   - If head is NULL, nothing happens.
 */
void list_cleanup(Node** head){
    if (head == NULL) return;
    pthread_mutex_lock(&lock);
    Node* node = *head;
    while (node != NULL){
        Node* old_node = node;
        node = node->next;
        mem_free(old_node);
    }
    *head = NULL;
    mem_deinit();
    pthread_mutex_unlock(&lock);
}

