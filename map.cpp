#include <map>
#include <string>
#include <stdio.h>
#include <assert.h>

typedef std::map<std::string, void *> Map;

int main() {
    Map m;
    int num[100];
    int i;
    char buf[256];

    for (i = 0; i < 100; ++i) {
        num[i] = 100 * i;
    }
    for (i = 0; i < 1000000; ++i) {
        sprintf(buf, "HAHA%d", i);
        m.insert(std::make_pair(buf, &num[i % 100]));
    }
    sprintf(buf, "HAHA9981");
    std::string s = buf;
    for (i = 0; i < 1000000; ++i) {
        m.find(s);
        if ((i & 1023) == 0) printf("%d/%d\n", i, 100000);
    }
}

