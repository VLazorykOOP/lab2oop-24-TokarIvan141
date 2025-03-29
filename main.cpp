#include <iostream>
#include <fstream>
#include <bitset>
#include <cstring>
using namespace std;

struct EncodedChar{
    unsigned short high : 4;
    unsigned short position : 7;
    unsigned short low : 4;
    unsigned short parity : 1;
};

union CharUnion {
    EncodedChar encoded;
    unsigned short value;
};

void encryptAndSave(const string& input, const string& filename)
{
    ofstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Error opening file for writing!" << endl;
        return;
    }

    string text = input;
    text.resize(128, ' ');

    for (size_t i = 0; i < text.size(); i++)
    {
        CharUnion cu;
        cu.encoded.high = (text[i] >> 4) & 0xF;
        cu.encoded.position = i & 0x7F;
        cu.encoded.low = text[i] & 0xF;
        cu.encoded.parity = __builtin_parity(cu.value);
        file.write(reinterpret_cast<char*>(&cu.value), sizeof(cu.value));
    }
    file.close();
}

void decryptAndDisplay(const string& filename)
{
    ifstream file(filename, ios::binary);
    if (!file)
    {
        cerr << "Error opening file for reading!" << endl;
        return;
    }

    string output(128, ' ');
    for (size_t i = 0; i < 128; i++)
    {
        CharUnion cu;
        file.read(reinterpret_cast<char*>(&cu.value), sizeof(cu.value));
        if (!file) break;
        char ch = (cu.encoded.high << 4) | cu.encoded.low;
        output[cu.encoded.position] = ch;
    }
    file.close();

    cout << "Decrypted text: " << output << endl;
}

void task1()
{
    cout << "=====================Task 1=====================" << endl << endl;
    int a, b, c, d;
    cout << "Enter values for a, b, c, d: ";
    cin >> a >> b >> c >> d;

    int result =
            (b << 5) + (b << 2) + b
            + (((d << 5) - d + (a << 3) + (a << 2)) >> 11)
            - ((c << 6) + c)
            + (d << 3) + (d << 2) + (d << 1);

    cout << "Result by bit: " << result << endl;

    int result_calc = 37 * b + ((d * 31 + 12 * a) / 2048) - 65 * c + d * 14;
    cout << "Result by calc: " << result_calc << endl;
}

void task2_3()
{
    cout << "=====================Task 2-3=====================" << endl << endl;
    int choice;
    cout << "2 - Encrypt text and save" << endl;
    cout << "3 - Decrypt text from file" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    cin.ignore();

    switch (choice)
    {
        case 2:
        {
            cout << "Enter text to encrypt: ";
            string text;
            getline(cin, text);
            encryptAndSave(text, "./encrypted.bin");
            break;
        }
        case 3:
            decryptAndDisplay("./encrypted.bin");
            break;
        default:
            cout << "Invalid choice! Try again." << endl;
    }
}

void task4()
{
    cout << "=====================Task 4=====================" << endl << endl;
    int x, y;
    cout << "Enter values for x, y: ";
    cin >> x >> y;

    int result =
            (((y << 4) + (y << 2) - (x << 6) - (x << 5) - (x << 3)) >> 5)
            + ((x + (y << 5)) >> 7)
            + ((x + y) & 15);
    cout << "Result by bit: " << result << endl;

    int result_calc = ((20 * y - x * 120) / 32) + ((x + 32 * y) / 128) + ((x + y) % 16);
    cout << "Result by calc: " << result_calc << endl;
}

int main()
{
    int choice;
    do
    {
        cout << "\n============Menu============" << endl;
        cout << "1 - Task 1" << endl;
        cout << "2-3 - Encrypt/Decrypt text" << endl;
        cout << "4 - Task 4" << endl;
        cout << "0 - Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: task1(); break;
            case 2:
            case 3: task2_3(); break;
            case 4: task4(); break;
            case 0: cout << "Exiting program..." << endl; break;
            default: cout << "Invalid choice! Try again" << endl;
        }
    } while (choice != 0);

    return 0;
}
