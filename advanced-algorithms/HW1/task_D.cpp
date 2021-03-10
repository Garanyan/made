#include <string>
#include <iostream>
#include <vector>

//Расстояние Левенштейна

//ABCDEFGH
//ACDEXGIH

int main(int argc, char **argv)
{
    std::string str1, str2;
    std::cin >> str1 >> str2;

    str1.push_back(' ');
    str2.push_back(' ');
    std::vector<std::vector<int>> dp(str1.size(), std::vector<int>(str2.size(), 0));
    for (int i = 0; i < str1.size(); ++i)
    {
        for (int j = 0; j < str2.size(); ++j)
        {
            if (i == 0)
            {
                dp[i][j] = j;
            }
            else if (j == 0)
            {
                dp[i][j] = i;
            }
            else
            {
                if (str1[i - 1] == str2[j - 1])
                {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else
                {
                    dp[i][j] = std::min(dp[i - 1][j], std::min(dp[i][j - 1], dp[i - 1][j - 1])) + 1;
                }
            }
        }
    }
    std::cout << dp.back().back() << std::endl;

    return 0;
}
