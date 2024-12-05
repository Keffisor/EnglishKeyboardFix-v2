#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;

void sendKey(INPUT input[], int size) {
    INPUT* inputs = new INPUT[size];
    ZeroMemory(inputs, size * sizeof(INPUT));

    for (int i = 0; i < size; ++i) {
        inputs[i] = input[i];
    }

    UINT uSent = SendInput(size, inputs, sizeof(INPUT));
    if (uSent != size)
    {
        cout << "There was an error on the send input!";
    }

    delete[] inputs;
}

void triggerKey(bool left)
{
    if (left) {
		INPUT inputs[4] = {};

        inputs[0].type = INPUT_KEYBOARD;
        inputs[0].ki.wVk = VK_CONTROL;
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_OEM_102;

        inputs[2].type = INPUT_KEYBOARD;
        inputs[2].ki.wVk = VK_OEM_102;
        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[3].type = INPUT_KEYBOARD;
        inputs[3].ki.wVk = VK_CONTROL;

		sendKey(inputs, 4);
    }
    else {
        INPUT inputs[6] = {};

		inputs[0].type = INPUT_KEYBOARD;
		inputs[0].ki.wVk = VK_CONTROL;
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

        inputs[1].type = INPUT_KEYBOARD;
        inputs[1].ki.wVk = VK_SHIFT;

		inputs[2].type = INPUT_KEYBOARD;
		inputs[2].ki.wVk = VK_OEM_102;

		inputs[3].type = INPUT_KEYBOARD;
		inputs[3].ki.wVk = VK_OEM_102;
		inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

		inputs[4].type = INPUT_KEYBOARD;
		inputs[4].ki.wVk = VK_CONTROL;

        inputs[5].type = INPUT_KEYBOARD;
        inputs[5].ki.wVk = VK_SHIFT;
        inputs[5].ki.dwFlags = KEYEVENTF_KEYUP;

        sendKey(inputs, 6);
    }

}

bool isCtrlPressed = false;

void keyDown(DWORD keyCode) {
    if (keyCode == 162 && !isCtrlPressed) {
        cout << "CTRL is pressed\n" << endl;
        isCtrlPressed = true;
    }

    if (isCtrlPressed && keyCode == 188) {
		cout << "CTRL + < is pressed\n" << endl;
        triggerKey(true);
    }

    if (isCtrlPressed && keyCode == 190) {
        cout << "CTRL + > is pressed\n" << endl;
        triggerKey(false);
    }
}

void keyUp(DWORD keyCode) {
	if (keyCode == 162) {
		cout << "CTRL is released\n" << endl;
		isCtrlPressed = false;
	}
}

HHOOK _k_hook;
LRESULT __stdcall k_Callback1(int nCode, WPARAM wParam, LPARAM lParam)
{
    PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;
    
    if (wParam == WM_KEYDOWN && nCode == HC_ACTION)
    {
        keyDown(key->vkCode);
    }

	if (wParam == WM_KEYUP && nCode == HC_ACTION)
	{
		keyUp(key->vkCode);
	}

	//cout << "Key code: " << key->vkCode << endl;

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    _k_hook = SetWindowsHookExA(WH_KEYBOARD_LL, k_Callback1, NULL, 0);
	
    if (_k_hook == NULL)
	{
		puts("Failed to install hook!");
		return 1;
	}

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
		//cout << "Message: " << msg.message << endl;
    }
    
    if (_k_hook) UnhookWindowsHookEx(_k_hook);

	return 0;
}