#include <cstdio>
typedef unsigned uint;
unsigned int bytes2uInt(const char *bytes)
{
    uint b0 = (uint)bytes[0] & 0xFFu;
    uint b1 = (uint)bytes[1] & 0xFFu;
    uint b2 = (uint)bytes[2] & 0xFFu;
    uint b3 = (uint)bytes[3] & 0xFFu;
    return (b0 << 24) | (b1 << 16) | (b2 << 8) | b3;
}

void uInt2Bytes(unsigned int _n, char *buf)
{
    buf[0] = 0xFFu & (_n >> 24);
    buf[1] = 0xFFu & (_n >> 16);
    buf[2] = 0xFFu & (_n >> 8);
    buf[3] = 0xFFu & (_n);
}
int main()
{
    printf("%d\n", bytes2uInt("\x00\xF7\xBD\x31"));
    char buf[6];
    uInt2Bytes(0x49505152, buf);
    printf("%s\n", buf);
}