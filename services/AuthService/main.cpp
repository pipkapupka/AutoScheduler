#include "AuthService.h"

int main(){
    AuthService::setApiKey("ddf4e7925ac8e9ebd0229797ce21c4d8");
    AuthService::startServer(8081);
    return 0;
}
