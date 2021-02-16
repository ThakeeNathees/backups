
#ifdef __EMSCRIPTEN__
#include <emscripten.h>

EM_JS(void, call_alert, (), {
  alert('hello world!');
  throw 'all done';
});

void a_func() {
  call_alert();
}

#endif

