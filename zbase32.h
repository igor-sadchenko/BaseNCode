	int divceil(int a, int b) {
		int c;
		if (a>0) {
			if (b>0) c = a + b - 1;
			else c = a;
		}
		else {
			if (b>0) c = a;
			else c = a + b + 1;
		}
		return c / b;
	}


	class ZBase32 {

	public:


		/**
		* Compute the number of base32 encoded characters given the
		* number of bits.
		*
		* @param lengthInBits
		*      The length of the data in bits
		* @return
		*      The length of the base-32 encoding of the data in characters
		*/
		static size_t const GetLengthEncode(const size_t lengthInBits) {
			return divceil(lengthInBits, 5);
		};

		/**
		* Decodes a string with base32 presentation into binary data.
		*
		* a2b_l() will return a result big enough to hold lengthinbits bits.  So
		* for example if cs is 4 characters long (encoding at least 15 and up to
		* 20 bits) and lengthinbits is 16, then a2b_l() will return a string of
		* length 2 (since 2 bytes is sufficient to store 16 bits).  If cs is 4
		* characters long and lengthinbits is 20, then a2b_l() will return a
		* string of length 3 (since 3 bytes is sufficient to store 20 bits). Note
		* that `b2a_l()' does not mask off unused least-significant bits, so for
		* example if cs is 4 characters long and lengthinbits is 17, then you
		* must ensure that all three of the unused least-significant bits of cs
		* are zero bits or you will get the wrong result. This precondition is
		* tested by assertions if assertions are enabled. (Generally you just
		* require the encoder to ensure this consistency property between the
		* least significant zero bits and value of `lengthinbits', and reject
		* strings that have a length-in-bits which isn't a multiple of 8 and yet
		* don't have trailing zero bits, as improperly encoded.)
		*
		* @param cs
		*    The data to be decoded
		* @param size
		*    The length of the input data buffer. Usually divceil(length in bits, 5).
		* @param lengthinbits
		*    The number of bits of data in <code>cs</code> to be decoded
		*/
		static unsigned char* Decode(const std::string& cs, size_t size, int* resultLength);

		/**
		* Encodes binary to to base32 presentation.
		*
		* b2a_l() will generate a base-32 encoded string big enough to encode
		* lengthinbits bits.  So for example if os is 2 bytes long and
		* lengthinbits is 15, then b2a_l() will generate a 3-character- long
		* base-32 encoded string (since 3 quintets is sufficient to encode 15
		* bits). If os is 2 bytes long and lengthinbits is 16 (or None), then
		* b2a_l() will generate a 4-character string.  Note that `b2a_l()' does
		* not mask off unused least-significant bits, so for example if os is 2
		* bytes long and lengthinbits is 15, then you must ensure that the unused
		* least-significant bit of os is a zero bit or you will get the wrong
		* result.  This precondition is tested by assertions if assertions are
		* enabled.
		*
		* Warning: if you generate a base-32 encoded string with `b2a_l()', and
		* then someone else tries to decode it by calling `a2b()' instead of
		* `a2b_l()', then they will (probably) get a different string than the
		* one you encoded!  So only use `b2a_l()' when you are sure that the
		* encoding and decoding sides know exactly which `lengthinbits' to use.
		* If you do not have a way for the encoder and the decoder to agree upon
		* the lengthinbits, then it is best to use `b2a()' and `a2b()'.  The only
		* drawback to using `b2a()' over `b2a_l()' is that when you have a number
		* of bits to encode that is not a multiple of 8, `b2a()' can sometimes
		* generate a base-32 encoded string that is one or two characters longer
		* than necessary.
		*
		* @param cs
		*     Pointer to binary data.
		* @param noOfBits
		*    The number of bits of data in encoded into `cs'
		*/
		static std::string Encode(const unsigned char* cs, const size_t noOfBits);


	};

	//                                         1         2         3
	//                               01234567890123456789012345678901
	static const char* const charsZBase = "ybndrfg8ejkmcpqxot1uwisza345h769";

	/*
	* revcharsZBase: index into this table with the ASCII value of the char.
	* The result is the value of that quintet.
	*/
	static const unsigned char revcharsZBase[] = {
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 18, 255, 25, 26, 27, 30, 29,
		7, 31, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 24, 1, 12, 3, 8, 5, 6,
		28, 21, 9, 10, 255, 11, 2, 16,
		13, 14, 4, 22, 17, 19, 255, 20,
		15, 0, 23, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255,
		255, 255, 255, 255, 255, 255, 255, 255
	};

	std::string ZBase32::Encode(const unsigned char* os, const size_t lengthinbits) {

		int len = (lengthinbits + 7) / 8;
		/* if lengthinbits is not a multiple of 8 then this is allocating
		* space for 0, 1, or 2 extra quintets that will be truncated at the
		* end of this function if they are not needed
		*/
		std::string result(divceil(len * 8, 5), ' ');

		/* index into the result buffer, initially pointing to the
		* "one-past-the-end" quintet
		*/
		int resp = result.size();

		/* pointer into the os buffer, initially pointing to the
		* "one-past-the-end" octet
		*/
		const unsigned char* osp = os + len;

		/* Now this is a real live Duff's device.  You gotta love it. */

		unsigned long x = 0;	// to hold up to 32 bits worth of the input
		switch ((osp - os) % 5) {

		case 0:
			do {
				x = *--osp;
				result[--resp] = charsZBase[x % 32]; /* The least sig 5 bits go into the final quintet. */
				x /= 32;	/* ... now we have 3 bits worth in x... */
		case 4:
			x |= ((unsigned long)(*--osp)) << 3; /* ... now we have 11 bits worth in x... */
			result[--resp] = charsZBase[x % 32];
			x /= 32; /* ... now we have 6 bits worth in x... */
			result[--resp] = charsZBase[x % 32];
			x /= 32; /* ... now we have 1 bits worth in x... */
		case 3:
			x |= ((unsigned long)(*--osp)) << 1; /* The 8 bits from the 2-indexed octet.
												 So now we have 9 bits worth in x... */
			result[--resp] = charsZBase[x % 32];
			x /= 32; /* ... now we have 4 bits worth in x... */
		case 2:
			x |= ((unsigned long)(*--osp)) << 4; /* The 8 bits from the 1-indexed octet.
												 So now we have 12 bits worth in x... */
			result[--resp] = charsZBase[x % 32];
			x /= 32; /* ... now we have 7 bits worth in x... */
			result[--resp] = charsZBase[x % 32];
			x /= 32; /* ... now we have 2 bits worth in x... */
		case 1:
			x |= ((unsigned long)(*--osp)) << 2; /* The 8 bits from the 0-indexed octet.
												 So now we have 10 bits worth in x... */
			result[--resp] = charsZBase[x % 32];
			x /= 32; /* ... now we have 5 bits worth in x... */
			result[--resp] = charsZBase[x];
			} while (osp > os);
		} /* switch ((osp - os.buf) % 5) */

		/* truncate any unused trailing zero quintets */
		return result.substr(0, divceil(lengthinbits, 5));
	}

	unsigned char* ZBase32::Decode(const std::string& cs, size_t size, int* resultLength) {
		unsigned long x = 0;	// to hold up to 32 bits worth of the input

		int len = divceil(size * 5, 8);

		/* if lengthinbits is not a multiple of 5 then this is
		* allocating space for 0 or 1 extra octets that will be
		* truncated at the end of this function if they are
		* not needed
		*/

		unsigned char* binaryResult;
		if (len < 128) {
			binaryResult = new unsigned char[128];
		}
		else {
			binaryResult = new unsigned char[len];
		}

		/* pointer into the result buffer, initially pointing to
		* the "one-past-the-end" octet
		*/
		unsigned char* resp = binaryResult + len;

		/* index into the input buffer, initially pointing to the
		* "one-past-the-end" character
		*/
		int csp = size;

		/* Now this is a real live Duff's device.  You gotta love it. */
		switch (csp % 8) {
		case 0:
			do {
				x = revcharsZBase[cs[--csp]]; /* 5 bits... */
		case 7:
			x |= revcharsZBase[cs[--csp]] << 5; /* 10 bits... */
			*--resp = x % 256;
			x /= 256; /* 2 bits... */
		case 6:
			x |= revcharsZBase[cs[--csp]] << 2; /* 7 bits... */
		case 5:
			x |= revcharsZBase[cs[--csp]] << 7; /* 12 bits... */
			*--resp = x % 256;
			x /= 256; /* 4 bits... */
		case 4:
			x |= revcharsZBase[cs[--csp]] << 4; /* 9 bits... */
			*--resp = x % 256;
			x /= 256; /* 1 bit... */
		case 3:
			x |= revcharsZBase[cs[--csp]] << 1; /* 6 bits... */
		case 2:
			x |= revcharsZBase[cs[--csp]] << 6; /* 11 bits... */
			*--resp = x % 256;
			x /= 256; /* 3 bits... */
		case 1:
			x |= revcharsZBase[cs[--csp]] << 3; /* 8 bits... */
			*--resp = x % 256;
			} while (csp);
		} /* switch ((csp - cs.buf) % 8) */

		/* truncate any unused trailing zero octets */
		*resultLength = divceil((size * 5 / 8) * 8, 8) + 1;

		return binaryResult;
	}
