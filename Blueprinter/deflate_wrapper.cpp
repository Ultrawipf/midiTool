#include "deflate_wrapper.h"
#include "zlib/zlib.h"

const unsigned ChunkSize = 4096;

bool Deflate(Bytevector &output, const Bytevector &input)
{
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree = 0;
	stream.opaque = 0;

	stream.avail_in = input.size();
	stream.next_in = const_cast<Byte *>(&input[0]);

    int res = deflateInit(&stream, 9);
	if (res != Z_OK)
		return false;

	Byte ChunkOut[ChunkSize];

	do
	{
		stream.avail_out = sizeof(ChunkOut);
		stream.next_out = ChunkOut;
		res = deflate(&stream, Z_FINISH);
		unsigned compressed = sizeof(ChunkOut) - stream.avail_out;
		unsigned oldsize = output.size();
		output.resize(oldsize + compressed);
		memcpy(&output[oldsize], ChunkOut, compressed);
	}
	while (stream.avail_out == 0);
	deflateEnd(&stream);

	return res == Z_STREAM_END;
}
bool Inflate(Bytevector &output, const Bytevector &input)
{
	z_stream stream;
	stream.zalloc = 0;
	stream.zfree = 0;
	stream.opaque = 0;

	stream.avail_in = input.size();
	stream.next_in = const_cast<Byte *>(&input[0]);

	int res = inflateInit(&stream);
	if (res != Z_OK)
		return false;

	Byte ChunkOut[ChunkSize];

	do
	{
		stream.avail_out = sizeof(ChunkOut);
		stream.next_out = ChunkOut;
		res = inflate(&stream, Z_FINISH);
		unsigned compressed = sizeof(ChunkOut) - stream.avail_out;
		unsigned oldsize = output.size();
		output.resize(oldsize + compressed);
		memcpy(&output[oldsize], ChunkOut, compressed);
	}
	while (stream.avail_out == 0);
	inflateEnd(&stream);

	return res == Z_STREAM_END;
}
std::string BytevectorToString(const Bytevector &vec)
{
	return std::string(reinterpret_cast<const char *>(&vec[0]), vec.size());
}
Bytevector StringToBytevector(const std::string &str)
{
	return Bytevector(str.begin(), str.end());
}
