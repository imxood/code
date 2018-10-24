#include <robot_driver/ComUtils.h>

namespace robot
{

std::string ComUtils::bytes2HexString(const char *bytes, int bytelength)
{
    std::string str("");
    std::string str2("0123456789abcdef");
    for (int i = 0; i < bytelength; i++)
    {
        int b;
        b = 0x0f & (bytes[i] >> 4);
        char s1 = str2.at(b);
        str.append(1, str2.at(b));
        b = 0x0f & bytes[i];
        str.append(1, str2.at(b));
        char s2 = str2.at(b);
    }
    return str;
}

int ComUtils::hexChar2Int(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    return 0;
}

char *ComUtils::hexString2Bytes(std::string s)
{
    int sz = s.length();
    char *ret = new char[sz / 2];
    for (int i = 0; i < sz; i += 2)
    {
        ret[i / 2] = (char)((hexChar2Int(s.at(i)) << 4) | hexChar2Int(s.at(i + 1)));
    }
    return ret;
}

long ComUtils::hexStr2Dec(std::string s)
{
    int count = s.length();
    long sum = 0;
    for (int i = count - 1; i >= 0; i--)
    {
        if (s[i] >= '0' && s[i] <= '9')
        {
            sum += (s[i] - 48) * pow(16, count - i - 1);
        }
        else if (s[i] >= 'A' && s[i] <= 'F')
        {
            sum += ((s[i] - 65) + 10) * pow(16, count - i - 1);
        }
        else if (s[i] >= 'a' && s[i] <= 'f')
        {
            sum += ((s[i] - 97) + 10) * pow(16, count - i - 1);
        }
    }
    return sum;
}


void ComUtils::hex_to_bin(char *hexdata, unsigned char *bindata)
{
	int i, j, ret(0);

	int len = strlen(hexdata);
	for (i = 0, j = 0; i < len; i++, j++)
	{
		int value(0);
		if (hexdata[i] >= '0' && hexdata[i] <= '9')
			value = (hexdata[i] - '0') << 4;
		else if (hexdata[i] >= 'A' && hexdata[i] <= 'F')
			value = (hexdata[i] - 'A' + 10) << 4;
		else if (hexdata[i] >= 'a' && hexdata[i] <= 'f')
			value = (hexdata[i] - 'a' + 10) << 4;
		else
			return;
		i++;
		if (hexdata[i] >= '0' && hexdata[i] <= '9')
			value = value + (hexdata[i] - '0');
		else if (hexdata[i] >= 'A' && hexdata[i] <= 'F')
			value = value + (hexdata[i] - 'A' + 10);
		else if (hexdata[i] >= 'a' && hexdata[i] <= 'f')
			value = value + (hexdata[i] - 'a' + 10);

		bindata[j] = (unsigned char)value;
		ret++;
	}
}

void ComUtils::dec_to_hex(unsigned long src, unsigned char *dst, size_t len)
{
	int *yushu = (int *)malloc(sizeof(int) * len);
	unsigned long tmp = src;
	for (int i = 0; i < len; i++)
	{
		yushu[i] = tmp % 16;
		tmp = tmp / 16;
	}

	for (int j = 0; j < len; j++)
	{
		dst[len - j - 1] = yushu[j];
	}
	free(yushu);
}

char ComUtils::byte2str(unsigned char *ch)
{
	if (*ch >= 0 && *ch <= 9)
	{
		return *ch + '0';
	}
	else if (*ch >= 10 && *ch <= 15)
	{
		return *ch - 10 + 'a';
	}
}



}
