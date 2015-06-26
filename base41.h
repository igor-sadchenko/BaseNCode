	namespace BASE41
	{
		static const uint8_t alphabet_length = 41;
		static const uint8_t blockBitsCount = 16;
		static const uint8_t blockCharsCount = 3;
		static const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0?!123456789&$@";
		static const int inv_alphabet_length = 'Z' + 1; // 'Z' - max from alphabet
		static const char inv_alphabet[inv_alphabet_length] =
		{
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
			-1, -1, -1, -1, 28, -1, -1, 39, -1, 38, -1, -1, -1, -1, -1, -1, -1, -1, -1, 26, 29, 30, 31, 32, 33, 34, 35, 36, 37, -1,
			-1, -1, -1, -1, 27, 40, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
		};

		static const unsigned long long _powN[3] = { 1681ull, 41ull, 1ull };

		unsigned long long get_bits64(const std::string &data, int bitPos, int bitsCount)
		{
			unsigned long long result = 0ull;

			int currentBytePos = bitPos / 8;
			int currentBitInBytePos = bitPos % 8;
			int xLength = min(bitsCount, 8 - currentBitInBytePos);
			if (xLength != 0)
			{
				result = ((unsigned long long)(unsigned char)data[currentBytePos] << (56 + currentBitInBytePos)) >> (64 - xLength) << (bitsCount - xLength);

				currentBytePos += (currentBitInBytePos + xLength) / 8;
				currentBitInBytePos = (currentBitInBytePos + xLength) % 8;

				int x2Length = bitsCount - xLength;
				if (x2Length > 8)
					x2Length = 8;

				while (x2Length > 0)
				{
					xLength += x2Length;
					result |= ((unsigned long long)(unsigned char)data[currentBytePos] >> (8 - x2Length) << (bitsCount - xLength));

					currentBytePos += (currentBitInBytePos + x2Length) / 8;
					currentBitInBytePos = (currentBitInBytePos + x2Length) % 8;

					x2Length = bitsCount - xLength;
					if (x2Length > 8)
						x2Length = 8;
				}
			}

			return result;
		}

		void bits_to_chars(std::string &chars, int ind, int count, unsigned long long block)
		{
			for (int i = 0; i < count; i++)
			{
				chars[ind + i] = alphabet[(int)(block % 41)];
				block /= 41;
			}
		}

		unsigned long long chars_to_bits(const std::string& data, int ind, int count)
		{
			unsigned long long result = 0;
			for (int i = 0; i < count; ++i)
				result += (unsigned long long)inv_alphabet[data[ind + i]] * _powN[blockCharsCount - 1 - i];
			return result;
		}


		void add_bits64(std::string& data, unsigned long long value, int bitPos, int bitsCount)
		{
			int currentBytePos = bitPos / 8;
			int currentBitInBytePos = bitPos % 8;

			int xLength = min(bitsCount, 8 - currentBitInBytePos);
			if (xLength != 0)
			{
				byte x1 = (byte)(value << (64 - bitsCount) >> (56 + currentBitInBytePos));
				data[currentBytePos] |= x1;

				currentBytePos += (currentBitInBytePos + xLength) / 8;
				currentBitInBytePos = (currentBitInBytePos + xLength) % 8;

				int x2Length = bitsCount - xLength;
				if (x2Length > 8)
					x2Length = 8;

				while (x2Length > 0)
				{
					xLength += x2Length;
					byte x2 = (byte)(value >> (bitsCount - xLength) << (8 - x2Length));
					data[currentBytePos] |= x2;

					currentBytePos += (currentBitInBytePos + x2Length) / 8;
					currentBitInBytePos = (currentBitInBytePos + x2Length) % 8;

					x2Length = bitsCount - xLength;
					if (x2Length > 8)
						x2Length = 8;
				}
			}
		}

		std::string Encode(const std::string &data)
		{

			int mainBitsLength = (data.length() * 8 / blockBitsCount) * blockBitsCount;
			int tailBitsLength = data.length() * 8 - mainBitsLength;
			int mainCharsCount = mainBitsLength * blockCharsCount / blockBitsCount;
			int tailCharsCount = (tailBitsLength * blockCharsCount + blockBitsCount - 1) / blockBitsCount;
			int globalCharsCount = mainCharsCount + tailCharsCount;
			int iterationCount = mainCharsCount / blockCharsCount;

			std::string result_data;
			result_data.resize(globalCharsCount);

			for (int ind = 0; ind < iterationCount; ++ind)
			{
				int bitInd = ind * blockBitsCount;
				unsigned long long bits = get_bits64(data, bitInd, blockBitsCount);

				int charInd = ind * blockCharsCount;
				bits_to_chars(result_data, charInd, blockCharsCount, bits);

			}

			if (tailBitsLength != 0)
			{
				unsigned long long bits = get_bits64(data, mainBitsLength, tailBitsLength);
				bits_to_chars(result_data, mainCharsCount, tailCharsCount, bits);
			}

			return result_data;
		}

		std::string Decode(const std::string &data)
		{
			if (!data.size())
				return std::string();

			int globalBitsLength = ((data.length() - 1) * blockBitsCount / blockCharsCount + 8) / 8 * 8;
			int mainBitsLength = globalBitsLength / blockBitsCount * blockBitsCount;
			int tailBitsLength = globalBitsLength - mainBitsLength;
			int mainCharsCount = mainBitsLength * blockCharsCount / blockBitsCount;
			int tailCharsCount = (tailBitsLength * blockCharsCount + blockBitsCount - 1) / blockBitsCount;
			unsigned long long tailBits = chars_to_bits(data, mainCharsCount, tailCharsCount);

			if ((tailBits >> tailBitsLength) != 0)
			{
				globalBitsLength += 8;
				mainBitsLength = globalBitsLength / blockBitsCount * blockBitsCount;
				tailBitsLength = globalBitsLength - mainBitsLength;
				mainCharsCount = mainBitsLength * blockCharsCount / blockBitsCount;
				tailCharsCount = (tailBitsLength * blockCharsCount + blockBitsCount - 1) / blockBitsCount;
			}
			int iterationCount = mainCharsCount / blockCharsCount;

			std::string result;
			result.resize(globalBitsLength / 8);

			for (int ind = 0; ind < iterationCount; ++ind)
			{
				int charInd = ind * (int)blockCharsCount;
				unsigned long long bits = chars_to_bits(data, charInd, (int)blockCharsCount);

				int bitInd = ind * blockBitsCount;
				add_bits64(result, bits, bitInd, blockBitsCount);
			}

			if (tailCharsCount != 0)
			{
				unsigned long long  bits = chars_to_bits(data, mainCharsCount, tailCharsCount);
				add_bits64(result, bits, mainBitsLength, tailBitsLength);
			}

			return result;
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

	}
