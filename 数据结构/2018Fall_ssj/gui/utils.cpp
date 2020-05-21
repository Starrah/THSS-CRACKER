#include "utils.h"
bool ischar(wchar_t c)
{
    char c1 = (char)c;
    if (0 <= c && c < 128)
        return true;
    else
        return false;
}

bool isBlank(wchar_t c)
{
    return c == (wchar_t)'\n' || c == (wchar_t)'\r' || c == (wchar_t)'\t' || c == (wchar_t)' ' || c == (wchar_t)'\v' || c == (wchar_t)12288;
}

void bubblesort(info* p, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (p[j].num < p[j + 1].num)
            {
                info temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
        }
    }
}



