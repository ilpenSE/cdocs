#ifndef TIMELAPSE_H
#define TIMELAPSE_H

int timelapse_int(int (*func)(const char*), const char* str);

double timelapse_double(double (*func)(double x, double y), double x, double y);

long long timelapse_powers(long long (*func)(long long base, long long exp), long long base, long long exp);

#endif // TIMELAPSE_H
