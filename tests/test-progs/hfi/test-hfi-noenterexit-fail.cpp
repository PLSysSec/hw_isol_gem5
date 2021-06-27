#include <assert.h>
#include <iostream>
#include <limits>

#include "hfi.h"

int main(int argc, char* argv[])
{
    // exit without enter sandbox should fail
    hfi_exit_sandbox();
}
