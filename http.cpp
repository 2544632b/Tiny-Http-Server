#include <iostream>
#include <dlfcn.h>

typedef struct _string {
    char *buffer;
    size_t size;
    size_t capacity;
} _string;

static size_t _string_get_capacity_with_size(size_t size) {
    if (size == 0) {
        return 16;
    } else if (size < 16) {
        return 32;
    } else if (size < 32) {
        return 48;
    } else {
        return size * 3 / 2;
    }
}

static _string *_string_create_with_capacity(size_t capacity) {
    _string *str = (_string *) malloc(sizeof(_string));
    if (!str) {
        return NULL;
    }
    str->buffer = (char *) calloc(capacity, 1);
    str->capacity = capacity;
    str->size = 0;
    memset(str->buffer, 0, capacity);
    return str;
}

_string *_string_create(const char *literal) {
    size_t size = strlen(literal);
    size_t capacity = _string_get_capacity_with_size(size);
    _string *str = _string_create_with_capacity(capacity);
    if (!str) {
        return NULL;
    }
    memcpy(str->buffer, literal, size);
    str->size = size;
    return str;
}

int main() {
  using std::cout;
  using std::cerr;

  // open the library
  cout << "Opening libhttp...\n";
  void* handle = dlopen("./libhttp.so", RTLD_LAZY);

  if (!handle) {
    cerr << "Cannot open library: " << dlerror() << '\n';
    return 1;
  }

  // load the symbol
  cout << "Loading symbol createHttpServer...\n";
  typedef void (*v_t)(_string* host, double port, _string* msg);

  // reset errors
  dlerror();
  v_t http = (v_t) dlsym(handle, "createHttpServer");
  const char *dlsym_error = dlerror();
  if (dlsym_error) {
    cerr << "Cannot load symbol 'hello': " << dlsym_error << '\n';
    dlclose(handle);
    return 1;
  }

  // use it to do the calculation
  http(_string_create("127.0.0.1"), 8080, _string_create("<h1>Hello!</h1>"));

  // close the library
  cout << "Closing library...\n";
  dlclose(handle);
}
