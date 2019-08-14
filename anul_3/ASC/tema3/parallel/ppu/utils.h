//macro for easily getting how much time has passed between two events
#define GET_TIME_DELTA(t1, t2) ((t2).tv_sec - (t1).tv_sec + \
		((t2).tv_usec - (t1).tv_usec) / 1000000.0)

//function headers for utils.c
void* _alloc(int size);
void _read_buffer(int fd, void* buf, int size);
void _write_buffer(int fd, void* buf, int size);
int _open_for_write(char* path);
int _open_for_read(char* path);
void _read_line(int fd, char* path, char* buf, int buf_size);
void* _alloc_align(int size);


