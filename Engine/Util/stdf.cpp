#include <cmath>
#include <cstring>

void* NWmemcpy(void* dst, void const* src, size_t s) {
    return memcpy(dst, src, s);
}
void* NWmalloc(size_t s) {
    return malloc(s);
}
void    NWfree(void* ptr) {
    free(ptr);
}
double  NWtan(double val) {
    return tan(val);
}
double  NWcos(double val) {
    return cos(val);
}
double  NWsin(double val) {
    return sin(val);
}
double  NWpow(double x, double p) {
    return pow(x,p);
}
