#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

using namespace std;

constexpr char* DOOR_ID = "ffykfhsq";

string day5Solution()
{
    string password;

    for (unsigned int i = 0; i < numeric_limits<unsigned int>::max() && password.length() < 8; i++)
    {
        string doorIdAndIndex = DOOR_ID + to_string(i);
        unsigned char digest[MD5_DIGEST_LENGTH];
        MD5(reinterpret_cast<const unsigned char*>(doorIdAndIndex.c_str()), doorIdAndIndex.length(), digest);

        ostringstream digestOutput;
        digestOutput << hex << setfill('0');
        for_each(begin(digest), end(digest), [&digestOutput](const auto& digestDigit)
        {
            digestOutput << setw(2) << static_cast<int>(digestDigit);
        });
        
        if (digestOutput.str().compare(0, 5, "00000") == 0)
        {
            password += digestOutput.str()[5];
        }
    }

    return password;
}