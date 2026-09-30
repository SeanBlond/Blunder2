#ifndef CONVERSIONS
#pragma once

#include <string>

namespace smath
{
    // Mostly this just exists right now for decimal to hexadecimal conversions for the color class, so it's not going to be the most organized thing in the world
    inline std::string decToHexa(int value)
    {
        // String that values will be stored in
        std::string hexadecimal = "";

        // Converting the value by looping through each decimal
        int index = 0;
        while (value != 0)
        {
            // Getting the remainder
            int remainder = value % 16;

            // Converting the remainder to hexadecimal
            if (remainder < 10)
                hexadecimal += 48 + remainder;
            else
                hexadecimal += 55 + remainder;

            // Moving on to the next decimal place
            value /= 16;
        }

        // Reversing the string
        std::reverse(hexadecimal.begin(), hexadecimal.end());

        // Returning the ouput
        return hexadecimal;
    }
    inline std::string decToHexa(int value, int numChars)
    {
        // String that values will be stored in
        std::string hexadecimal = "";

        // If value is already 0, just convert it to a string of 0s
        if (value == 0)
        {
            for (int i = 0; i < numChars; i++)
            {
                hexadecimal += '0';
            }
            return hexadecimal;
        }

        // Converting the value by looping through each decimal
        int index = 0;
        while (value != 0)
        {
            // Getting the remainder
            int remainder = value % 16;

            // Converting the remainder to hexadecimal
            if (remainder < 10)
                hexadecimal += 48 + remainder;
            else
                hexadecimal += 55 + remainder;

            // Moving on to the next decimal place
            value /= 16;
        }

        // Adding 0s to fir char length
        for (int i = 0; i < numChars - hexadecimal.length(); i++)
        {
            hexadecimal += '0';
        }

        // Reversing the string
        std::reverse(hexadecimal.begin(), hexadecimal.end());

        // Returning the ouput
        return hexadecimal;
    }
    inline int hexaToDec(std::string value)
    {
        // Integer that values will be stored in
        int decimal = 0;

        // Looping through each character and converting it to decimal (back to front)
        for (int i = value.size() - 1; i >= 0; i--)
        {
            int tempDecimal = 0;

            // Checking for chars '0' through '9'
            if (48 <= value[i] && value[i] <= 57)
                tempDecimal = (value[i] - 48);

            // Checking for chars 'A' through 'F'
            else if (65 <= value[i] && value[i] <= 70)
                tempDecimal = (value[i] - (65 - 10));

            // Checking for chars 'a' through 'f'
            else if (97 <= value[i] && value[i] <= 102)
                tempDecimal = (value[i] - (97 - 10));

            // Char could not be converted to decimal
            else
                tempDecimal = 0;

            // Adding temp decimal to the correct decimal place
            decimal += tempDecimal * pow(16, (value.size() - i - 1));
        }

        // Returning result
        return decimal;
    }
}
#endif // !CONVERSIONS
