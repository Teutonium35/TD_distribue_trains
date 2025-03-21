typedef struct {
    int client_socket;
    int resource_id;
} ClientRequest;

void* handle_client(void* arg);