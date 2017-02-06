#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <array>
#include <limits>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

using namespace std;

constexpr char* SALT = "cuanljph";
constexpr unsigned int TARGET_KEY_NUMBER = 64;
constexpr unsigned int HASH_LOOKAHEAD_COUNT = 1000;

void fillHashes(const string& salt, array<string, HASH_LOOKAHEAD_COUNT + 1>& hashes);
string hashKey(const string& key);
bool tryGetRepeatingContiguousCharacter(const string& text, unsigned int numRepeating, char& character);


string day14Solution()
{
    array<string, HASH_LOOKAHEAD_COUNT + 1> hashes;
    fillHashes(SALT, hashes);

    unsigned int numFoundKeys = 0;
    unsigned int keyIndex = 0;
    for (keyIndex; keyIndex < numeric_limits<unsigned int>::max() && numFoundKeys < TARGET_KEY_NUMBER; keyIndex++)
    {
        char repeatingCharacter;
        if (tryGetRepeatingContiguousCharacter(hashes[keyIndex % hashes.size()], 3, repeatingCharacter))
        {
            bool found5Repeating = false;
            for (unsigned int i = keyIndex + 1; i < keyIndex + 1 + HASH_LOOKAHEAD_COUNT && !found5Repeating; i++)
            {
                string nextHash = hashes[i % hashes.size()];
                found5Repeating |= nextHash.find(string(5, repeatingCharacter)) != nextHash.npos;
            }

            if (found5Repeating)
            {
                numFoundKeys++;
            }
        }

        hashes[keyIndex % hashes.size()] = hashKey(SALT + to_string(keyIndex + hashes.size()));
    }

    if (numFoundKeys == TARGET_KEY_NUMBER)
    {
        return to_string(keyIndex - 1);
    }
    else
    {
        throw invalid_argument("Cannot find 64th key for given salt");
    }
}

void fillHashes(const string& salt, array<string, HASH_LOOKAHEAD_COUNT + 1>& hashes)
{
    for (unsigned int i = 0; i < hashes.size(); i++)
    {
        hashes[i] = hashKey(salt + to_string(i));
    }
}

string hashKey(const string& key)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(key.c_str()), key.length(), digest);

    ostringstream digestOutput;
    digestOutput << hex << setfill('0');
    for_each(begin(digest), end(digest), [&digestOutput](const auto& digestDigit)
    {
        digestOutput << setw(2) << static_cast<int>(digestDigit);
    });

    return digestOutput.str();
}


bool tryGetRepeatingContiguousCharacter(const string& text, unsigned int numRepeating, char& character)
{
    char lastCharacter;
    unsigned int currentNumRepeating = 0;

    for (unsigned int i = 0; i < text.size() && currentNumRepeating < numRepeating; i++)
    {
        if (i == 0 || text[i] == lastCharacter)
        {
            currentNumRepeating++;
        }
        else
        {
            currentNumRepeating = 1;
        }

        lastCharacter = text[i];
    }

    character = lastCharacter;
    return currentNumRepeating == numRepeating;
}