#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "chatOpener.h"

int main(void)
{
    openChat("224.0.0.45", 5555, "Ariel", "Group Name");
}