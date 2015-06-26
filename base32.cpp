// Base32 implementation
//

#include "base32.h"

std::string base32_encode(const std::string &data)
{
	std::string res;
	res.reserve((size_t)(data.size() * 1.6 + 1));

	int count = 0;
	if (!data.empty())
	{
		int buffer = data[0];
		size_t next = 1;
		int bitsLeft = 8;
		while ((bitsLeft > 0 || next < data.length()))
		{
			if (bitsLeft < 5)
			{
				if (next < data.length())
				{
					buffer <<= 8;
					buffer |= data[next++] & 0xFF;
					bitsLeft += 8;
				}
				else
				{
					int pad = 5 - bitsLeft;
					buffer <<= pad;
					bitsLeft += pad;
				}
			}
			size_t index = 0x1F & (buffer >> (bitsLeft - 5));
			bitsLeft -= 5;

			res += '\0';
			res[count++] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567"[index];
		}
	}

	return res;
}

std::string base32_decode(const std::string &encoded)
{
	std::string res;
	res.reserve(encoded.size());

	int buffer = 0;
	int bitsLeft = 0;
	int count = 0;
	for (size_t i = 0; i < encoded.length(); ++i)
	{
		uint8_t ch = encoded[i];

		if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' || ch == '-')
		{
			continue;
		}
		buffer <<= 5;

		// Deal with commonly mistyped characters
		if (ch == '0')
		{
			ch = 'O';
		}
		else if (ch == '1')
		{
			ch = 'L';
		}
		else if (ch == '8')
		{
			ch = 'B';
		}

		// Look up one base32 digit
		if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
		{
			ch = (ch & 0x1F) - 1;
		}
		else if (ch >= '2' && ch <= '7')
		{
			ch -= '2' - 26;
		}
		else
		{
			throw std::exception("Bad base32 symbol");
		}

		buffer |= ch;
		bitsLeft += 5;
		if (bitsLeft >= 8)
		{
			res += '\0';
			res[count++] = (char)(buffer >> (bitsLeft - 8));
			bitsLeft -= 8;
		}
	}

	return res;
}

std::string GroupStringByHyphens(const std::string &s, unsigned int symbols_in_group)
{
	std::string res;

	for (size_t curr_offset = 0; curr_offset < s.length(); curr_offset += symbols_in_group)
	{
		if (!res.empty()) res += "-";
		res += s.substr(curr_offset, symbols_in_group);
	}

	return res;
}
