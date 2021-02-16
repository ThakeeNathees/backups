
rem Reference: https://emscripten.org/docs/porting/connecting_cpp_and_javascript/Interacting-with-code.html

rem RUN env.bat before running this.

if not exist "out" ( md "./out" )

emcc main.c -o out/index.html -s "EXPORTED_FUNCTIONS=['_a_func']" -s "EXPORTED_RUNTIME_METHODS=['ccall','cwrap']"
