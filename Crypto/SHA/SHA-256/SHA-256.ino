//this is limited to 55 byte input

#include <avr/pgmspace.h>

uint32_t const roundConstants[] PROGMEM = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

uint32_t overallHashes[] = {
	0x6A09E667,
	0xBB67AE85,
	0x3C6EF372,
	0xA54FF53A,
	0x510E527F,
	0x9B05688C,
	0x1F83D9AB,
	0x5BE0CD19
};

// this will be hashed, max size in bytes - 55
uint8_t input[] = { 97, 98, 99 };

uint8_t paddedInput[64];
uint32_t words[64];

void setup()
{
	Serial.begin(9600);

	uint32_t startMillis = millis();

	//copy input
	for (uint8_t i = 0; i < sizeof(input); i++)
	{
		paddedInput[i] = input[i];
	}

	uint64_t inputLength = sizeof(input) * 8;

	// add 1 after the message
	paddedInput[sizeof(input)] = 128;

	for (uint8_t i = 0; i < 8; i++)
	{
		paddedInput[i + 56] = inputLength >> 56 - (i * 8);
	}

	// convert padded input bytes to words
	for (size_t i = 0; i < 16; i++)
	{
		uint32_t currentWord = 0;

		for (uint8_t j = 0; j < 3; j++)
		{
			currentWord += paddedInput[(i * 4) + j];
			currentWord = currentWord << 8;
		}
		currentWord += paddedInput[(i * 4) + 3];

		words[i] = currentWord;
	}

	for (int i = 16; i < 64; i++)
	{
		uint32_t currentWord = SigmaOne(words[i - 2]) + words[i - 7] + SigmaZero(words[i - 15]) + words[i - 16];
		words[i] = currentWord;
	}

	uint32_t a = overallHashes[0];
	uint32_t b = overallHashes[1];
	uint32_t c = overallHashes[2];
	uint32_t d = overallHashes[3];
	uint32_t e = overallHashes[4];
	uint32_t f = overallHashes[5];
	uint32_t g = overallHashes[6];
	uint32_t h = overallHashes[7];

	for (int i = 0; i < 64; i++)
	{
		uint32_t roundConstant = pgm_read_dword(&roundConstants[i]);
		uint32_t t1 = h + EpsilonOne(e) + Ch(e, f, g) + roundConstant + words[i];
		uint32_t t2 = EpsilonZero(a) + Maj(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	overallHashes[0] += a;
	overallHashes[1] += b;
	overallHashes[2] += c;
	overallHashes[3] += d;
	overallHashes[4] += e;
	overallHashes[5] += f;
	overallHashes[6] += g;
	overallHashes[7] += h;

	uint32_t endMillis = millis();

	// prints the result
	for (uint8_t i = 0; i < 8; i++)
	{
		Serial.print(overallHashes[i], HEX);
	}

	Serial.println();
	Serial.println(endMillis - startMillis);
}

void loop()
{


}

uint32_t SigmaZero(uint32_t x)
{
	uint32_t result = RotateRight(x, 7) ^ RotateRight(x, 18) ^ (x >> 3);
	return result;
}

uint32_t SigmaOne(uint32_t x)
{
	uint32_t result = RotateRight(x, 17) ^ RotateRight(x, 19) ^ (x >> 10);
	return result;
}

uint32_t RotateRight(uint32_t input, byte numberOfRotations)
{
	return (input >> numberOfRotations) | (input << (32 - numberOfRotations));
}

uint32_t Ch(uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t result = (x & y) ^ (~x & z);
	return result;
}

uint32_t Maj(uint32_t x, uint32_t y, uint32_t z)
{
	uint32_t result = (x & y) ^ (x & z) ^ (y & z);
	return result;
}

uint32_t EpsilonZero(uint32_t x)
{
	uint32_t result = RotateRight(x, 2) ^ (RotateRight(x, 13)) ^ (RotateRight(x, 22));
	return result;
}

uint32_t EpsilonOne(uint32_t x)
{
	uint32_t result = RotateRight(x, 6) ^ (RotateRight(x, 11)) ^ (RotateRight(x, 25));
	return result;
}