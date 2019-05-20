#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <sstream>
#include <bitset>

using namespace std;

int main()
{
	fstream fin("in.txt", fstream::in);
	fstream fout("out.txt", fstream::in | fstream::out);
	string str;
	getline(fin, str);
	
	vector<string> binary;
	char binary_str[8];
	for (int i = 0, j = 0; i < str.length(); i++) // запись символов в бинарном виде в файл 
	{
		int k = 128;
		while (k > 0) 
		{ 	
			fout << bool(str[i] & k);
			k = k >> 1;
		};
		fout << endl;
	}
	
	fout.seekg(0, std::ios::beg);
	for (int i = 0; i < str.length(); i++) // чтение из файла символов в бинарном виде в вектор
	{
		fout >> binary_str;
		binary.insert(binary.end(), binary_str);
	}
	
	char binary_bloc_str[21];
	vector<string> binary_bloc;
	int check_exponent_2 = 0;
	for (int i = 0, k = 0; i < binary.size(); ) // добавление контрольных битов
	{
		check_exponent_2 = 0;
		for (int j = 0; j < 21; j++)
		{
			if (j + 1 == pow(2, check_exponent_2))
			{
				binary_bloc_str[j] = '0';
				check_exponent_2++;
			}
			else
			{
				if (i == binary.size())
					break;
				binary_bloc_str[j] = binary[i][k];		
				k++;
				if (k == 8)
				{
					k = 0;
					i++;
				}
			}
		}
		binary_bloc.push_back(binary_bloc_str);
	}
	
	int control_bits[5] = {0, 1, 3, 7, 15};
	int check_contr_bits = 0;
	int count_true = 0;
	int k = 0;
	while (k < binary_bloc.size())
	{
		for (int i = 0; i < 5; i++) // кодирование 
		{
			count_true = 0;
			check_contr_bits = 0;
			for (int j = control_bits[i]; j < 21; j++)
			{
				if (binary_bloc[k][j] == '1')
				{
					count_true++;
				}
				check_contr_bits++;
				if (check_contr_bits == control_bits[i] + 1)
				{
					j = j + control_bits[i] + 1;
					check_contr_bits = 0;
				}
			}
			if (count_true % 2 == 0)
			{
				binary_bloc[k][control_bits[i]] = '0';
			}
			else
			{
				binary_bloc[k][control_bits[i]] = '1';
			}
		}
		k++;
	}
	
	k = 0;
	fstream fout_result("out_code.txt", fstream::out);
	while (k < binary_bloc.size())
	{
		char result_binary_str[21];
		for (int i = 0; i < 21; i++)
		{
			result_binary_str[i] = binary_bloc[k][i];
		}
		fout_result << result_binary_str << " ";
		k++;
	}
	
	
	
	char binary_bloc_str_decode[21]; // decode
	fstream fin_decode("in_decode.txt", fstream::in);
	
	vector<string> binary_bloc_decode_error;
	vector<string> binary_bloc_decode;

	while (!fin_decode.eof()) // обнуление контрольных битов
	{
		check_exponent_2 = 0;
		fin_decode >> binary_bloc_str_decode;
		binary_bloc_decode_error.push_back(binary_bloc_str_decode);
		for (int j = 0; j < 21; j++)
		{
			if (j + 1 == pow(2, check_exponent_2))
			{
				binary_bloc_str_decode[j] = '0';
				check_exponent_2++;
			}
		}
		binary_bloc_decode.push_back(binary_bloc_str_decode);
	}
	k = 0;
	while (k < binary_bloc_decode.size())
	{
		check_contr_bits = 0;
		count_true = 0;

		for (int i = 0; i < 5; i++) // пересчет котрольных битов
		{
			count_true = 0;
			for (int j = control_bits[i]; j < 21; j++)
			{
				if (binary_bloc_decode[k][j] == '1')
				{
					count_true++;
				}
				check_contr_bits++;
				if (check_contr_bits == control_bits[i] + 1)
				{
					j = j + control_bits[i] + 1;
					check_contr_bits = 0;
				}
			}
			if (count_true % 2 == 0)
			{
				binary_bloc_decode[k][control_bits[i]] = '0';
			}
			else
			{
				binary_bloc_decode[k][control_bits[i]] = '1';
			}
		}
		k++;
	}
	
	int index_error = 0;
	fout_result << "\n" << "Decode:" << endl;
	for (int k = 0; k < binary_bloc_decode.size(); k++) // поиск индекса ошибочного символа
	{
		for (int i = 0; i < 21; i++)
		{
			if (binary_bloc_decode[k][i] != binary_bloc_decode_error[k][i])
			{
				index_error = index_error + i + 1;
			}
		}
		
		fout_result << "Error in N block index: №" << k + 1 << " = " << index_error - 1 << endl;
		cout << "Error in N block index: №" << k + 1 << " = " << index_error - 1 << endl;
		if (binary_bloc_decode_error[k][index_error - 1] == '0')
			binary_bloc_decode_error[k][index_error - 1] = '1';
		else
			binary_bloc_decode_error[k][index_error - 1] = '0';
		index_error = 0;
	}

	vector<string> binary_decode;
	char binary_decode_str[8];
	check_exponent_2 = 0;
	for (int i = 0, k = 0; i < binary_bloc_decode_error.size(); i++) // удаление контрольных битов
	{
		check_exponent_2 = 0;
		for (int j = 0; j < 21; j++)
		{
			if (j + 1 == pow(2, check_exponent_2))
			{
				check_exponent_2++;
			}
			else
			{
				binary_decode_str[k] = binary_bloc_decode_error[i][j];
				k++;
				if (k == 8)
				{
					binary_decode.push_back(binary_decode_str);
					k = 0;
				}
			}
		}
	}
	string output;
	for (int i = 0; i < binary_decode.size(); i++) // перевод посл-ти битов в 10-й вид
	{
		stringstream sstream(binary_decode[i]);
		while (sstream.good())
		{
			std::bitset<8> bits;
			sstream >> bits;
			char c = char(bits.to_ulong());
			output += c;
			break;
		}
	}
	std::cout << output;
	exit(1);
}

