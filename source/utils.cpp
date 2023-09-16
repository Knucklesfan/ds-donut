#include "utils.h"
void Graphics::bresenham(int8_t *buffer, int xa, int ya, int xb, int yb)
{
	bool yLonger=false;
    int shortLen=yb-ya;
    int longLen=xb-xa;
	int temp = 0;
    if (abs(shortLen)>abs(longLen)) {
		temp = shortLen;
		shortLen = longLen;
		longLen = temp;

        yLonger=true;
    }
    int decInc = longLen==0 ?  decInc=0 : ((shortLen << 16) / longLen);

    if (yLonger) {
        ya*=BUFFER_WIDTH;
        yb*=BUFFER_WIDTH;
        if (longLen>0)
            for (int j=0x8000+(xa<<16);ya<=yb;ya+=BUFFER_WIDTH, j+=decInc)
                buffer[ya + (j >> 16)] = '#';  // or a call to your painting method
        else
            for (int j=0x8000+(xa<<16);ya>=yb;ya-=BUFFER_WIDTH, j-=decInc)
                buffer[ya + (j >> 16)] = '#';  // or a call to your painting method
    }
    else
    {
        if (longLen>0)
            for (int j=0x8000+(ya<<16);xa<=xb;++xa, j+=decInc)
                buffer[(j >> 16) * BUFFER_WIDTH + xa] = '#';  // or a call to your painting method
        else
            for (int j=0x8000+(ya<<16);xa>=xb;--xa, j-=decInc)
                buffer[(j >> 16) * BUFFER_WIDTH + xa] =  '#';  // or a call to your painting method
    }
}
