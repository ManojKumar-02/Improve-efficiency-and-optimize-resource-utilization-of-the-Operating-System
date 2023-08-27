#include "types.h"
#include "user.h"
#include "date.h"

int main()
{
    
    struct rtcdate r ;
    if(mydate(&r) == -1){
    printf(2,"date failed\n");
    exit();
    }
    exit();
}


