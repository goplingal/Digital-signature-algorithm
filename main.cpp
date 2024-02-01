#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <bitset>
#include <boost/multiprecision/cpp_int.hpp>
#include "boost/random.hpp"
#include <boost/generator_iterator.hpp>
#include <conio.h>
#include <boost/algorithm/string.hpp>

class Sign_Params
{
private:
	int q;
	int p;
	boost::multiprecision::cpp_int g;
	boost::multiprecision::cpp_int y;
	boost::multiprecision::cpp_int r;
	boost::multiprecision::cpp_int s;
public:
	Sign_Params()
	{
		this->q = 0;
		this->p = 0;
		this->g = 0;
		this->y = 0;
		this->r = 0;
		this->s = 0;
	}
	Sign_Params(int q, int p, boost::multiprecision::cpp_int g, boost::multiprecision::cpp_int y, boost::multiprecision::cpp_int r, boost::multiprecision::cpp_int s)
	{
		this->q = q;
		this->p = p;
		this->g = g;
		this->y = y;
		this->r = r;
		this->s = s;
	}
	int Get_q()
	{
		return this->q;
	}
	int Get_p()
	{
		return this->p;
	}
	boost::multiprecision::cpp_int Get_g()
	{
		return this->g;
	}
	boost::multiprecision::cpp_int Get_y()
	{
		return this->y;
	}
	boost::multiprecision::cpp_int Get_r()
	{
		return this->r;
	}
	boost::multiprecision::cpp_int Get_s()
	{
		return this->s;
	}
	void Set_q(int q)
	{
		this->q = q;
	}
	void Set_p(int p)
	{
		this->p = p;
	}
	void Set_g(boost::multiprecision::cpp_int g)
	{
		this->g = g;
	}
	void Set_y(boost::multiprecision::cpp_int y)
	{
		this->y = y;
	}
	void Set_r(boost::multiprecision::cpp_int r)
	{
		this->r = r;
	}
	void Set_s(boost::multiprecision::cpp_int s)
	{
		this->s = s;
	}
};

static unsigned short int Hash_F(std::string str)
{
	unsigned short int hash = 0;
	unsigned short int x = 0;
	for (int i = 0; i < str.length(); i++)
	{
		hash = (hash << 4) + (str[i]);
		hash &= ~x;
	}
	return hash / 55;
}

Sign_Params Make_Sign(std::string str)
{
	int H = Hash_F(str);
	if (H < 4)
	{
		return Sign_Params(0, 0, 0, 0, 0, 0);
	}
	std::bitset<16> b(H);
	std::string s = b.to_string();
	s.erase(0, s.find_first_not_of('0'));
	typedef boost::mt19937 RNGType;
	RNGType rng(time(0));
	boost::uniform_int<> range(4, 5000);
	boost::variate_generator< RNGType, boost::uniform_int<>>dice(rng, range);
	int q = 0;
	while (true)
	{
		q = dice();
		int flag = 1;
		for (int i = 2; i <= sqrt(q); i++)
		{
			if (q % i == 0) {
				flag = 0;
				break;
			}
		}
		if (q <= 2)
		{
			flag = 0;
		}
		if (flag == 1)
		{
			std::bitset <16> b1(q);
			std::string s1 = b1.to_string();
			s1.erase(0, s1.find_first_not_of('0'));
			if (s1.length() == s.length())
			{
				break;
			}
		}
		else
		{
			continue;
		}
	}
	std::bitset <16> b1(q);
	int p = 2;
	RNGType rng1(time(0));
	boost::uniform_int<> range1(1, 29000);
	boost::variate_generator< RNGType, boost::uniform_int<>>dice1(rng1, range1);
	while (true)
	{
		if ((p - 1) % q != 0)
		{
			p = dice1();
		}
		else
		{
			int flag = 1;
			for (int i = 2; i <= sqrt(p); i++)
			{
				if (p % i == 0) {
					flag = 0;
					break;
				}
			}
			if (p <= 2)
			{
				flag = 0;
			}
			if (flag == 1)
			{
				break;
			}
			else
			{
				p = dice1();
			}
		}
	}
	int tmp = 2;
	boost::multiprecision::cpp_int g = boost::multiprecision::pow(boost::multiprecision::cpp_int(tmp), (p - 1) / q);
	g = g % p;
	while (true)
	{
		if (g == 1)
		{
			tmp = dice1();
			g = boost::multiprecision::pow(boost::multiprecision::cpp_int(tmp), (p - 1) / q);
			g = g % p;
		}
		else
		{
			break;
		}
	}
	RNGType rngx(time(0));
	boost::uniform_int<> range3(1, 15000);
	boost::variate_generator< RNGType, boost::uniform_int<>>dicex(rngx, range3);
	int x = dicex();
	boost::multiprecision::cpp_int y = boost::multiprecision::pow(g, x);
	y = y % p;
	RNGType rngk(time(0));
	boost::uniform_int<> range4(1, 25000);
	boost::variate_generator< RNGType, boost::uniform_int<>>dicek(rngk, range4);
	int k = dicek();
	boost::multiprecision::cpp_int r = 0;
	boost::multiprecision::cpp_int S = 0;
	while (r == 0 || S == 0)
	{
		k = dicek();
		r = boost::multiprecision::pow(g, k);
		r = r % p;
		r = r % q;
		if (r != 0)
		{
			boost::multiprecision::cpp_int temp = boost::multiprecision::pow(boost::multiprecision::cpp_int(k), q - 2);
			temp = temp % q;
			S = temp * (H + x * r);
			S = S % q;
			if (r != 0 && S != 0)
			{
				break;
			}
		}
	}
	Sign_Params sp(q, p, g, y, r, S);
	return sp;
}

void Sign_Check(Sign_Params sp, std::string str)
{
	int H = Hash_F(str);
	if (H < 5)
	{
		return;
	}
	int q = sp.Get_q();
	int p = sp.Get_p();
	boost::multiprecision::cpp_int g = sp.Get_g();
	boost::multiprecision::cpp_int y = sp.Get_y();
	boost::multiprecision::cpp_int r = sp.Get_r();
	boost::multiprecision::cpp_int S = sp.Get_s();
	boost::multiprecision::cpp_int w = boost::multiprecision::pow(S, q - 2);
	w = w % q;
	boost::multiprecision::cpp_int u1 = H * (w % q);
	boost::multiprecision::cpp_int u2 = r * (w % q);
	boost::multiprecision::cpp_int v = boost::multiprecision::pow(g, unsigned long long(u1)) * boost::multiprecision::pow(y, unsigned long long(u2));
	v = v % p;
	v = v % q;
	if (v == r)
	{
		std::cout << "Signature is valid" << std::endl;
	}
	else
	{
		std::cout << "Signature is invalid" << std::endl;
	}
}

bool Is_Digits(std::string s)
{
    return s.find_first_not_of("0123456789") == std::string::npos;
}

bool Check_Params(std::vector < std::string> v) 
{
	for (auto next : v)
	{
		if (!Is_Digits(next) || next == "")
		{
			return false;
		}
	}
	return true;
}

void Create_File(Sign_Params sp, std::string s, std::string filename)
{
	int ind = filename.find_first_of('.');
	std::string new_filename = filename.substr(0, ind);
	std::string new_file_ext = filename.substr(ind, filename.length() - 1);
	int i = 1;
	while (i < 1000000)
	{
		std::string new_file = new_filename + std::to_string(i) + new_file_ext;
		std::fstream temp_f;
		temp_f.open(new_file, std::ios::in);
		if (temp_f.is_open())
		{
			i++;
			temp_f.close();
		}
		else
		{
			std::fstream from_f;
			std::fstream to_f;
			std::string temp;
			from_f.open(filename, std::ios::in);
			to_f.open(new_file, std::ios::out);
			while (std::getline(from_f, temp))
			{
				to_f << temp << std::endl;
			}
			to_f << '\n' + s + std::to_string(sp.Get_q()) + ":" + std::to_string(sp.Get_p()) + ":" + std::to_string(unsigned long long(sp.Get_g())) + ":" + std::to_string(unsigned long long(sp.Get_y())) + ":" + std::to_string(unsigned long long(sp.Get_r())) + ":" + std::to_string(unsigned long long(sp.Get_s()));
			std::cout << "Signature has been created" << std::endl;
			from_f.close();
			to_f.close();
			temp_f.close();
			break;
		}
	}
}

void Delete_Sign(std::string last_line, std::string filename)
{
	int ind = filename.find_first_of('.');
	std::string new_filename = filename.substr(0, ind);
	std::string new_file_ext = filename.substr(ind, filename.length() - 1);
	int i = 1;
	while (i < 1000000)
	{
		std::string new_file = new_filename + std::to_string(i) + new_file_ext;
		std::fstream temp_f;
		temp_f.open(new_file, std::ios::in);
		if (temp_f.is_open())
		{
			i++;
			temp_f.close();
		}
		else
		{
			std::fstream from_f;
			std::fstream to_f;
			std::string temp;
			from_f.open(filename, std::ios::in);
			to_f.open(new_file, std::ios::out);
			while (std::getline(from_f, temp))
			{
				if (temp != last_line)
				{
					to_f << temp << std::endl;
				}
			}
			std::cout << "Signature has been deleted" << std::endl;
			from_f.close();
			to_f.close();
			temp_f.close();
			break;
		}
	}
}

int main()
{
	Sign_Params sp;
	std::string s = "-----Digital_Signature-----:";
	std::string filename;
	std::ifstream r;
	while (true)
	{
		std::cout << "Input filename (or '.exit' to close application)" << std::endl;
		std::cin >> filename;
		if (filename == ".exit")
		{
			break;
		}
		r.open(filename);
		if (r.is_open())
		{
			r.seekg(-1, std::ios_base::end);
			while (true)
			{
				char ch;
				r.get(ch);
				if ((int)r.tellg() <= 1)
				{
					r.seekg(0);
					break;
				}
				else if (ch == '\n')
				{
					break;
				}
				else
				{
					r.seekg(-2, std::ios_base::cur);
				}
			}
			std::string last_line;
			std::getline(r, last_line);
			r.clear();
			r.seekg(0, std::ios::beg);
			std::string to_hash = "";
			if (last_line.find(s) == 0 && last_line != "")
			{
				std::string s1 = "";
				while (std::getline(r, s1))
				{
					if (s1 == last_line)
					{
						break;
					}
					to_hash += s1;
				}
				std::cout << "Signature found, validating (may take a few seconds)..." << std::endl;
				if (to_hash.length() > 2)
				{
					int ind = last_line.find_first_of(':');
					std::string temp_string = last_line.substr(ind + 1, last_line.length() - 1);
					std::vector <std::string> params;
					boost::split(params, temp_string, boost::is_any_of(":"));
					if (params.size() == 6 && Check_Params(params) == true)
					{
						sp.Set_q(stoi(params[0]));
						sp.Set_p(stoi(params[1]));
						sp.Set_g(stoi(params[2]));
						sp.Set_y(stoi(params[3]));
						sp.Set_r(stoi(params[4]));
						sp.Set_s(stoi(params[5]));
						Sign_Check(sp, to_hash);
						r.clear();
						r.close();
						std::cout << "Do you want to delete signature? y/n" << std::endl;
						std::string key;
						std::cin >> key;
						while (true)
						{
							if (key == "y")
							{
								std::string input_name;
								while (true)
								{
									std::cout << "Do you want to name your file? Otherwise it will be named by default. Input file name or '.default'" << std::endl;
									std::cin >> input_name;
									if (input_name == ".default")
									{
										Delete_Sign(last_line, filename);
										break;
									}
									else
									{
										std::ofstream input_file;
										input_file.open(input_name);
										if (input_file.is_open())
										{

											std::fstream from_f;
											std::fstream to_f;
											std::string temp;
											from_f.open(filename, std::ios::in);
											to_f.open(input_name, std::ios::out);
											while (std::getline(from_f, temp))
											{
												if (temp != last_line)
												{
													to_f << temp << std::endl;
												}
											}
											std::cout << "Signature has been deleted" << std::endl;
											from_f.close();
											to_f.close();
											input_file.close();
											break;
										}
										else
										{
											std::cout << "Invalid filename" << std::endl;
											input_file.close();
										}
									}
								}
								break;
							}
							else if (key == "n")
							{
								r.clear();
								r.close();
								break;
							}
							r.clear();
							r.close();
							std::cout << "Invalid input. Use 'y' or 'n' " << std::endl;
							std::cin >> key;
						}
					}
					else
					{
						std::cout << "Invalid signature" << std::endl;
						r.clear();
						r.close();
						std::cout << "Do you want to delete invalid signature? y/n" << std::endl;
						std::string key;
						std::cin >> key;
						while (true)
						{
							if (key == "y")
							{
								std::string input_name;
								while (true)
								{
									std::cout << "Do you want to name your file? Otherwise it will be named by default. Input file name or '.default'" << std::endl;
									std::cin >> input_name;
									if (input_name == ".default")
									{
										Delete_Sign(last_line, filename);
										break;
									}
									else
									{
										std::ofstream input_file;
										input_file.open(input_name);
										if (input_file.is_open())
										{

											std::fstream from_f;
											std::fstream to_f;
											std::string temp;
											from_f.open(filename, std::ios::in);
											to_f.open(input_name, std::ios::out);
											while (std::getline(from_f, temp))
											{
												if (temp != last_line)
												{
													to_f << temp << std::endl;
												}
											}
											std::cout << "Signature has been deleted" << std::endl;
											from_f.close();
											to_f.close();
											input_file.close();
											break;
										}
										else
										{
											std::cout << "Invalid filename" << std::endl;
											input_file.close();
										}
									}
								}
								break;
							}
							else if (key == "n")
							{
								r.clear();
								r.close();
								break;
							}
							r.clear();
							r.close();
							std::cout << "Invalid input. Use 'y' or 'n' " << std::endl;
							std::cin >> key;
						}

					}
				}
				else
				{
					std::cout << "Error: File must contain more then two symbols" << std::endl;
					r.clear();
					r.close();
				}
			}
			else
			{
				std::string s1 = "";
				r.clear();
				r.seekg(0, std::ios::beg);
				while (std::getline(r, s1))
				{
					to_hash += s1;
				}
				if (to_hash.length() > 2)
				{
					std::cout << "There is no signature" << std::endl;
					std::cout << "Do you want to add signature? y/n" << std::endl;
					std::string key;
					std::cin >> key;
					while (true)
					{
						if (key == "y")
						{
							r.clear();
							r.close();
							Sign_Params sp1 = Make_Sign(to_hash);
							if (sp1.Get_s() == 0)
							{
								break;
							}
							std::string input_name;
							while (true)
							{
								std::cout << "Do you want to name your file? Otherwise it will be named by default. Input file name or '.default'" << std::endl;
								std::cin >> input_name;
								if (input_name == ".default")
								{
									Create_File(sp1, s, filename);
									break;
								}
								else
								{
									std::ofstream input_file;
									input_file.open(input_name);
									if (input_file.is_open())
									{
										std::fstream from_f;
										std::fstream to_f;
										std::string temp;
										from_f.open(filename, std::ios::in);
										to_f.open(input_name, std::ios::out);
										while (std::getline(from_f, temp))
										{
											to_f << temp << std::endl;
										}
										to_f << '\n' + s + std::to_string(sp1.Get_q()) + ":" + std::to_string(sp1.Get_p()) + ":" + std::to_string(unsigned long long(sp1.Get_g())) + ":" + std::to_string(unsigned long long(sp1.Get_y())) + ":" + std::to_string(unsigned long long(sp1.Get_r())) + ":" + std::to_string(unsigned long long(sp1.Get_s()));
										std::cout << "Signature has been created" << std::endl;
										from_f.close();
										to_f.close();
										input_file.close();
										break;
									}
									else
									{
										std::cout << "Invalid filename" << std::endl;
										input_file.close();
									}
								}
							}
							break;
						}
						else if (key == "n")
						{
							r.clear();
							r.close();
							break;
						}
						std::cout << "Invalid input. Use 'y' or 'n' " << std::endl;
						std::cin >> key;
					}
				}
				else
				{
					std::cout << "Error: File must contain more then two symbols" << std::endl;
					r.clear();
					r.close();
				}
			}
		}
		else
		{
			std::cout << "There is no such file" << std::endl;
		}
	}
	return 0;
}