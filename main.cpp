#include <iostream>
#include <Windows.h>

#define LOW_VELOCITY '8'
#define HIGH_VELOCITY 't'

using namespace std;

HKL layout = GetKeyboardLayout(0);

bool keyDown;

unsigned int getScanCode(int code) {
    return MapVirtualKeyA(code, MAPVK_VK_TO_VSC);
}

unsigned int getScanCodeChar(char c) {
    return MapVirtualKeyA(VkKeyScanExA(c, layout), MAPVK_VK_TO_VSC);
}

bool windowIsRoblox() {
    if(GetForegroundWindow() == FindWindowA(NULL, "Roblox"))
        return true;
    else
        return false;
}

void setVelocity(char c) {
    INPUT input[4]; 

    for (int i = 0; i < 4; i++) {
        // Have to do this so we get the actual input so we can mutate it
        INPUT * ip = &input[i];
        ip->type = INPUT_KEYBOARD;
        ip->ki.time = 0;
        ip->ki.dwExtraInfo = 0;

        // Push down on first 2 inputs, release on last 2
        ip->ki.dwFlags = (i < 2 ? KEYEVENTF_SCANCODE : KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP);

        // Alt on 0 and 3, key on 1 and 4
        ip->ki.wScan = (i % 2 == 0 ? getScanCode(VK_LMENU) : getScanCodeChar(c));
    }    

    for(int i = 0; i < 4; i++){
        SendInput(1, &input[i], sizeof(INPUT));
    }

    //SendInput(4, input, (sizeof(INPUT) * 4));

    cout << "Sent input on " << c << endl;

}

void holdSpace(){
    INPUT input[1];
    INPUT * ip = &input[1];

    ip->type = INPUT_KEYBOARD;
    ip->ki.time = 0;
    ip->ki.dwExtraInfo = 0;
    ip->ki.dwFlags = KEYEVENTF_SCANCODE;
    ip->ki.wScan = getScanCode(VK_SPACE);

    cout << "Now holding space!" << endl;
    SendInput(1, input, sizeof(INPUT));
}

int main(){

    while(true){

        if(GetAsyncKeyState(VK_SPACE) && !keyDown){
            keyDown = true;
            // Lower velocity
            if(windowIsRoblox()){
                //holdSpace();
                setVelocity(LOW_VELOCITY);
            }
        }

        else if (!GetAsyncKeyState(VK_SPACE) && keyDown){
            keyDown = false;
            // Return velocity
            if(windowIsRoblox()){
                setVelocity(HIGH_VELOCITY);
            }
        }

    }

    cout << "Program execution has stopped";
    cin.get();
    return 0;
}