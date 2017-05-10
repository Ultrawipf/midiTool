#include "Blueprint.h"
#include "util.h"
#include "base64.h"
#include "deflate_wrapper.h"

bool BlueprintToJSON(std::string &output, const std::string &input)
{
	std::string enc = input;
	if (enc.empty() || enc[0] != '0') // invalid version
		return false;

	enc.erase(enc.begin()); // remove version number

	Bytevector vec = base64_decode(enc);

	Bytevector uncompressed;
	if (!Inflate(uncompressed, vec))
		return false;

	output = BytevectorToString(uncompressed);
	return true;
}
bool BlueprintFromJSON(std::string &output, const std::string &input)
{
	Bytevector vec = StringToBytevector(input);
	Bytevector compressed;

	if (!Deflate(compressed, vec))
		return false;

	std::string enc = base64_encode(compressed);

	output = "0" + enc;
	return true;
}