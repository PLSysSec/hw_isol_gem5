#include <assert.h>
#include <iostream>
#include "hfi.h"

int main(int argc, char* argv[])
{
    uint64_t version = hfi_get_version();
    std::cout << "HFI: version: " << version << "\n";
    assert(version >= 1);
}